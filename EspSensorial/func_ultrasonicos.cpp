#include "func_ultrasonicos.h"
#include "config.h"
#include <Arduino.h>

/**
 * @brief Mide la distancia usando un sensor HC-SR04.
 * @param trigPin Pin de trigger.
 * @param echoPin Pin de echo.
 * @return Distancia en cm, o 999 si no hay eco (obstáculo lejano).
 */
int medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Timeout de 15 ms (~2.5 m)
  long duracion = pulseIn(echoPin, HIGH, 15000);
  if (duracion == 0) return 999;
  int distanciaCm = duracion * 0.034 / 2;
  return distanciaCm;
}

/**
 * @brief Tarea FreeRTOS que ejecuta el escaneo de los 4 sensores ultrasónicos.
 * Se ejecuta en el núcleo 0 de forma independiente.
 */
void TareaUltrasonicos(void * pvParameters) {
  pinMode(TRIG_F, OUTPUT); pinMode(ECHO_F, INPUT);
  pinMode(TRIG_B, OUTPUT); pinMode(ECHO_B, INPUT);
  pinMode(TRIG_L, OUTPUT); pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, OUTPUT); pinMode(ECHO_R, INPUT);
  
  for (;;) {
    distFrente = medirDistancia(TRIG_F, ECHO_F);
    distAtras  = medirDistancia(TRIG_B, ECHO_B);
    distIzq    = medirDistancia(TRIG_L, ECHO_L);
    distDer    = medirDistancia(TRIG_R, ECHO_R);
    vTaskDelay(30 / portTICK_PERIOD_MS);  // ~33 Hz
  }
}

/**
 * @brief Inicia la tarea de ultrasonidos en el núcleo 0.
 */
void iniciarUltrasonicos() {
  xTaskCreatePinnedToCore(
    TareaUltrasonicos,   // Función de la tarea
    "Lectura_HCSR04",    // Nombre
    2048,                // Tamaño de pila (bytes)
    NULL,                // Parámetros
    1,                   // Prioridad
    NULL,                // Handle (no necesario)
    0                    // Núcleo 0
  );
}
