/**
 * @file Anillo-TX.ino
 * @brief Transmisión de datos del anillo de sensores y radar ultrasónico.
 * 
 * Lee 16 sensores IR a través de un multiplexor CD74HC4067 y 4 sensores
 * ultrasónicos HC-SR04, calcula el ángulo donde se encuentra la pelota
 * mediante centroide y envía la trama por UART a la ESP32 de motores.
 * 
 * @author Robo_V Team
 * @version v10 (Centroide IR + Radar)
 */

#include <Arduino.h>
#include "config.h"
#include "func_ultrasonicos.h"
#include "func_multiplexor.h"
#include "func_anillo.h"

void setup() {
  Serial.begin(115200);
  Enlace.begin(38400, SERIAL_8N1, RX_PIN, TX_PIN);
  
  // Configurar pines del multiplexor
  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinSIG, INPUT_PULLUP);
  
  // Iniciar la tarea de ultrasonidos en el núcleo 0
  iniciarUltrasonicos();
  
  Serial.println("\n=== ESP32 ANILLO (TX) v10 Listo ===");
}

void loop() {
  // Leer el estado de los 16 sensores IR
  int totalActivos = 0;
  fotorreceptoresActivos(totalActivos);
  
  // Calcular ángulo por centroide
  int mejorLargo = ubicarPelota();
  int estado = (mejorLargo > 0) ? 1 : 0;
  
  // Construir trama UART
  Enlace.print("A:");
  Enlace.print(angulo, 1);
  Enlace.print(" C:");
  Enlace.print(estado);
  Enlace.print(" N:");
  Enlace.print(totalActivos);
  Enlace.print(" F:");
  Enlace.print(distFrente);
  Enlace.print(" B:");
  Enlace.print(distAtras);
  Enlace.print(" L:");
  Enlace.print(distIzq);
  Enlace.print(" R:");
  Enlace.print(distDer);
  
  for (int i = 0; i < 16; i++) {
    Enlace.print(activo[i]);
    Enlace.print(",");
  }
  Enlace.println();
  
  // Depuración local
  Serial.printf("TX -> IR[A:%.1f C:%d] RADAR[F:%d B:%d L:%d R:%d]\n",
                angulo, estado, distFrente, distAtras, distIzq, distDer);
  
  delay(50);  // 20 Hz
}
