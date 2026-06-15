/**
 * @file ESP32-MOTORES-RX-V1.ino
 * @brief Control principal del robot (motores y giroscopio)
 * 
 * Este programa es el "cerebro" del robot. Recibe datos del anillo de sensores
 * por UART, controla los motores mediante cinemática Mecanum, actualiza el rumbo
 * con el giroscopio MPU6050 y envía información de depuración por WiFi.
 * 
 * @author Robo_V Team
 * @version v12 (Debug WiFi + Estados optimizados)
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "func_giro.h"
#include "func_motor.h"
#include "config.h"

// ====================== DEBUG POR WiFi ======================
const char* ssid = "ESP32_DEBUG";
const char* password = "12345678";
WebServer server(80);
String debugInfo = "=== ESP32 MOTORES v12 (Depuración WiFi) ===\n\n";

void handleRoot() {
  server.send(200, "text/plain", debugInfo);
}

void actualizarDebugInfo() {
  debugInfo = "=== Estado Actual ===\n";
  debugInfo += "Estado: " + String(estadoActual) + "\n";
  debugInfo += "Yaw: " + String(yaw, 2) + "°\n";
  debugInfo += "Ángulo IR: " + String(anguloIR, 1) + "°\n";
  debugInfo += "Sensores activos: " + String(nIR) + "\n";
  debugInfo += "Distancias (cm) - F:" + String(distFrente) + 
               " B:" + String(distAtras) + " L:" + String(distIzq) + 
               " R:" + String(distDer) + "\n";
  debugInfo += "Velocidad actual: " + String(velAvanceActual) + "\n";
  debugInfo += "Tiempo sin pelota: " + String(millis() - tUltimaVezPelota) + " ms\n";
  debugInfo += "=====================================\n";
}

void setupWiFi() {
  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("WiFi AP iniciado: " + String(ssid));
  Serial.println("IP: " + WiFi.softAPIP().toString());
}

// ====================== COMUNICACIÓN UART ======================
void leerUART() {
  while (Enlace.available()) {
    char c = Enlace.read();
    if (c == '\n') {
      uartBuffer[bufIndex] = '\0';
      float a;
      int c_st, n, df, db, dl, dr;
      char rV[16];

      // Parseo de la trama: "A:123.4 C:1 N:5 F:30 B:20 L:25 R:22 0,1,0,1,..."
      if (sscanf(uartBuffer, "A:%f C:%d N:%d F:%d B:%d L:%d R:%d", 
                 &a, &c_st, &n, &df, &db, &dl, &dr) >= 7) {
        // Extraer vector de vecinos (16 bits) después de "R:"
        char* ptr = strstr(uartBuffer, "R:");
        if (ptr != NULL) {
          ptr = strchr(ptr, ' ');
          if (ptr != NULL) {
            ptr++;
            for (int i = 0; i < 16; i++) {
              rV[i] = atoi(ptr);
              ptr = strchr(ptr, ',');
              if (ptr != NULL) ptr++;
            }
          }
        }
        
        // Actualizar variables globales
        anguloIR = a;
        estadoIR = c_st;
        nIR = n;
        distFrente = df;
        distAtras = db;
        distIzq = dl;
        distDer = dr;
        // recepVecinos = rV; (si se necesita)
        tUltimaVezPelota = millis();
      }
      bufIndex = 0;
    } else if (c != '\r' && bufIndex < 127) {
      uartBuffer[bufIndex++] = c;
    }
  }
}

// ====================== CONFIGURACIÓN INICIAL ======================
void setup() {
  Serial.begin(115200);
  Enlace.begin(38400, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(300);
  
  Serial.println("\n=== ESP32 MOTORES v12 (Búsqueda Táctica) ===");
  
  // Inicializar drivers de motor
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, LOW);  // Seguro activado inicialmente
  
  // Configurar pines de dirección como salidas
  int pd[] = { FL_IN1, FL_IN2, FR_IN1, FR_IN2, RL_IN1, RL_IN2, RR_IN1, RR_IN2 };
  for (int p : pd) pinMode(p, OUTPUT);
  
  pwmInit(FL_PWM);
  pwmInit(FR_PWM);
  pwmInit(RL_PWM);
  pwmInit(RR_PWM);
  
  detener();
  digitalWrite(STBY, HIGH);  // Quitar seguro
  
  // Inicializar giroscopio
  if (mpuInit()) {
    Serial.println("MPU6050 detectado, calibrando...");
    delay(T_QUIETO_MS);
    calibrarGyro();
    Serial.println("Calibración completada.");
  } else {
    Serial.println("ERROR: MPU6050 no detectado. Verifique conexiones.");
  }
  
  // Inicializar debug WiFi
  setupWiFi();
  
  Serial.println("Sistema listo.");
}

// ====================== BUCLE PRINCIPAL ======================
void loop() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate < LOOP_DELAY_MS) return;
  lastUpdate = millis();
  
  // 1. Leer datos del anillo
  leerUART();
  
  // 2. Actualizar rumbo con el giroscopio
  actualizarRumbo();
  
  // 3. Actualizar página de debug
  actualizarDebugInfo();
  server.handleClient();
  
  // 4. Máquina de estados principal
  switch (estadoActual) {
    case ESPERANDO_PELOTA:
      // Buscar girando en el lugar
      girarEnSitio(VEL_BUSCAR);
      if (estadoIR == 1) {  // Pelota detectada
        estadoActual = ALINEANDO_PELOTA;
        Serial.println("Pelota detectada. Alineando...");
      }
      break;
      
    case ALINEANDO_PELOTA:
      {
        float error = errorAngular(anguloIR - yaw);
        if (abs(error) <= TOL_APUNTADO) {
          estadoActual = AVANZANDO_PELOTA;
          Serial.println("Alineado. Avanzando...");
          break;
        }
        int giro = constrain(error * KP_GIRO, -VEL_GIRO, VEL_GIRO);
        girarEnSitio(giro);
      }
      break;
      
    case AVANZANDO_PELOTA:
      {
        float error = errorAngular(anguloIR - yaw);
        int correccion = constrain(error * KP_CORRECCION, -MAX_CORRECCION, MAX_CORRECCION);
        avanzarSuave(VEL_AVANCE, correccion);
        
        // Verificar captura (suficientes sensores activos)
        if (nIR >= N_CAPTURA) {
          detener();
          estadoActual = IR_A_NORTE;
          tFrenoIniciado = millis();
          Serial.println("Pelota capturada. Giro al norte.");
        }
        // Timeout: si no recibe datos de la pelota
        else if (millis() - tUltimaVezPelota > TIMEOUT_PELOTA_MS) {
          estadoActual = ESPERANDO_PELOTA;
          Serial.println("Pelota perdida. Reanudando búsqueda.");
        }
      }
      break;
      
    case IR_A_NORTE:
      {
        float error = errorAngular(YAW_PORTERIA - yaw);
        if (abs(error) <= TOL_PORTERIA && distFrente < DISTANCIA_SEGURA_CM) {
          detener();
          estadoActual = ESPERANDO_PELOTA;
          Serial.println("Área de penalización alcanzada. Esperando nueva pelota.");
          break;
        }
        int correccion = constrain(error * KP_CORRECCION, -MAX_CORRECCION, MAX_CORRECCION);
        avanzarSuave(VEL_AVANCE, correccion);
      }
      break;
      
    case DETENIDO:
      detener();
      if (millis() - tFrenoIniciado > TIEMPO_DETENCION_MS) {
        estadoActual = ESPERANDO_PELOTA;
      }
      break;
  }
  
  // Depuración por serie (opcional)
  static unsigned long lastSerial = 0;
  if (millis() - lastSerial > 500) {
    Serial.printf("Yaw: %.2f | IR Ang: %.1f | Estado: %d | nIR: %d | Vel: %d\n",
                  yaw, anguloIR, estadoActual, nIR, velAvanceActual);
    lastSerial = millis();
  }
}
