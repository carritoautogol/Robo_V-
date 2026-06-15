#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ====================== COMUNICACIÓN UART ======================
constexpr int RX_PIN = 26;   // No usado en transmisión
constexpr int TX_PIN = 25;   // Envío hacia ESP32 #1
extern HardwareSerial Enlace;

// ====================== MULTIPLEXOR ======================
constexpr int pinS0 = 19;
constexpr int pinS1 = 18;
constexpr int pinS2 = 17;
constexpr int pinS3 = 16;
constexpr int pinSIG = 4;      // Entrada analógica/digital
constexpr int totalSensores = 16;
constexpr float GRADOS_POR_SENSOR = 22.5;   // 360° / 16

// ====================== ULTRASONIDOS ======================
// Frontal
constexpr int TRIG_F = 13;
constexpr int ECHO_F = 12;
// Trasero
constexpr int TRIG_B = 14;
constexpr int ECHO_B = 27;
// Izquierdo
constexpr int TRIG_L = 32;
constexpr int ECHO_L = 33;
// Derecho
constexpr int TRIG_R = 21;
constexpr int ECHO_R = 26;

// ====================== VARIABLES GLOBALES ======================
extern volatile int distFrente, distAtras, distIzq, distDer;
extern volatile float angulo;
extern volatile bool activo[16];

#endif // CONFIG_H
