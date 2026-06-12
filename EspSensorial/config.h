#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
//  COMUNICACIÓN — UART — Pines de comunicacion
// ============================================================

constexpr RX_PIN 26 
constexpr TX_PIN 25 
HardwareSerial Enlace(1); 

// ============================================================
//  MULTIPLEXOR — Pines de multiplexor
// ============================================================

constexpr int pinS0 = 19; 
constexpr int pinS1 = 18; 
constexpr int pinS2 = 17; 
constexpr int pinS3 = 16; 
constexpr int pinSIG = 4; 

constexpr int totalSensores = 16; 
constexpr float GRADOS_POR_SENSOR = 22.5; 

// ============================================================
//  ULTRASONICOS — Pines de ultrasonicos
// ============================================================

constexpr TRIG_F 13 // Frente
constexpr ECHO_F 12

constexpr TRIG_B 14 // Atrás
constexpr ECHO_B 27

constexpr TRIG_L 32 // Izquierda
constexpr ECHO_L 33

constexpr TRIG_R 22 // Derecha
constexpr ECHO_R 23

// ============================================================
//  NAVEGACION — ULTRASONICOS
// ============================================================

extern int distFrente;
extern int distAtras;
extern int distIzq;
extern int distDer;

#endif
