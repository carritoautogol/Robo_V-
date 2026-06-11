/* ==================================================================
   ESP32 ANILLO (TX) v10 - Centroide IR + Radar Ultrasónico
   ================================================================== */
#include <Arduino.h> 
#include <math.h>    
#include "Ultrasonicos.h" // Vincula la pestaña del radar

// --- PINES DEL MULTIPLEXOR ---
const int pinS0 = 19; 
const int pinS1 = 18; 
const int pinS2 = 17; 
const int pinS3 = 16; 
const int pinSIG = 4; 

const int totalSensores = 16; 
const float GRADOS_POR_SENSOR = 22.5; 

// --- COMUNICACIÓN UART ---
#define RX_PIN 26 
#define TX_PIN 25 
HardwareSerial Enlace(1); 

void seleccionarCanal(int canal) {
  digitalWrite(pinS0, bitRead(canal, 0)); 
  digitalWrite(pinS1, bitRead(canal, 1)); 
  digitalWrite(pinS2, bitRead(canal, 2)); 
  digitalWrite(pinS3, bitRead(canal, 3)); 
}

void setup() {
  Serial.begin(115200); 
  Enlace.begin(38400, SERIAL_8N1, RX_PIN, TX_PIN); 
  
  pinMode(pinS0, OUTPUT); pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT); pinMode(pinS3, OUTPUT);
  pinMode(pinSIG, INPUT_PULLUP);   
  
  // Enciende el radar en el Núcleo 0 de forma paralela
  iniciarUltrasonicos(); 
  
  Serial.println("\n=== ESP32 ANILLO (TX) v10 Listo ==="); 
}

void loop() {
  // LÓGICA DE DETECCIÓN INFRARROJA (Corriendo en el Núcleo 1)
  bool activo[16];      
  int totalActivos = 0; 

  for (int i = 0; i < totalSensores; i++) { 
    seleccionarCanal(i); 
    delayMicroseconds(100); 
    activo[i] = (digitalRead(pinSIG) == LOW); 
    if (activo[i]) totalActivos++; 
  }

  int mejorInicio = -1, mejorLargo = 0; 
  int iniActual = -1, largoActual = 0; 
  
  for (int k = 0; k < totalSensores * 2; k++) { 
    int i = k % totalSensores; 
    if (activo[i]) { 
      if (largoActual == 0) iniActual = i; 
      largoActual++; 
      if (largoActual > mejorLargo) { 
        mejorLargo = largoActual; 
        mejorInicio = iniActual; 
      }
    } else { largoActual = 0; }
  }
  if (mejorLargo > totalSensores) mejorLargo = totalSensores; 

  float angulo = -1.0; 
  if (mejorLargo > 0) { 
    float sx = 0, sy = 0; 
    for (int j = 0; j < mejorLargo; j++) { 
      int idx = (mejorInicio + j) % totalSensores; 
      float a = radians(idx * GRADOS_POR_SENSOR); 
      sx += cos(a); 
      sy += sin(a); 
    }
    angulo = degrees(atan2(sy, sx)); 
    if (angulo < 0) angulo += 360.0; 
  }

  int estado = (mejorLargo > 0) ? 1 : 0; 
  
  // ENSAMBLE DEL PAQUETE UART (Añadiendo F, B, L, R para las paredes)
  Enlace.print("A:"); Enlace.print(angulo, 1); 
  Enlace.print(" C:"); Enlace.print(estado); 
  Enlace.print(" N:"); Enlace.print(totalActivos);
  Enlace.print(" F:"); Enlace.print(distFrente);
  Enlace.print(" B:"); Enlace.print(distAtras);
  Enlace.print(" L:"); Enlace.print(distIzq);
  Enlace.print(" R:"); Enlace.println(distDer);

  // Monitor serie local para verificar que los ultrasónicos envían datos reales
  Serial.printf("TX -> IR[A:%.1f C:%d] RADAR[F:%d B:%d L:%d R:%d]\n", 
                 angulo, estado, distFrente, distAtras, distIzq, distDer); 
  
  delay(50); 
}
