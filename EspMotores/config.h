#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ====================== COMUNICACIÓN UART ======================
extern HardwareSerial Enlace;
constexpr int RX_PIN = 34;     // Recepción desde el anillo (Serial2)
constexpr int TX_PIN = 17;     // No se usa en este lado

// ====================== PINES DE MOTORES (TB6612FNG) ======================
// Driver 1 (ruedas izquierdas)
constexpr uint8_t STBY      = 4;

// Rueda Delantera Izquierda (FL)
constexpr uint8_t FL_PWM    = 25;
constexpr uint8_t FL_IN1    = 26;
constexpr uint8_t FL_IN2    = 27;

// Rueda Trasera Izquierda (RL)
constexpr uint8_t RL_PWM    = 33;
constexpr uint8_t RL_IN1    = 32;
constexpr uint8_t RL_IN2    = 14;

// Driver 2 (ruedas derechas)
// Rueda Delantera Derecha (FR)
constexpr uint8_t FR_PWM    = 13;
constexpr uint8_t FR_IN1    = 23;
constexpr uint8_t FR_IN2    = 2;

// Rueda Trasera Derecha (RR)
constexpr uint8_t RR_PWM    = 19;
constexpr uint8_t RR_IN1    = 18;
constexpr uint8_t RR_IN2    = 5;

// Inversión de giro por orientación física de los motores
constexpr bool INVERTIR_FL  = false;
constexpr bool INVERTIR_FR  = false;
constexpr bool INVERTIR_RL  = false;
constexpr bool INVERTIR_RR  = true;   // El trasero derecho está invertido

// Configuración PWM
constexpr uint32_t PWM_FREQ = 20000;  // 20 kHz (evita ruido audible)
constexpr uint8_t  PWM_RES  = 8;      // 8 bits (0-255)

// ====================== PARÁMETROS DE VELOCIDAD ======================
constexpr int PWM_MAX        = 182;   // Límite seguro para motores Faulhaber
constexpr int VEL_AVANCE     = 120;   // Velocidad de ataque
constexpr int VEL_GIRO       = 85;    // Velocidad de giro sobre el eje
constexpr int VEL_BUSCAR     = 80;    // Velocidad durante búsqueda
constexpr int RAMPA_PASO     = 8;     // Incremento de aceleración
constexpr int MAX_CORRECCION = 50;    // Corrección angular máxima

constexpr float KP_GIRO       = 2.5;  // Ganancia proporcional para giro puro
constexpr float KP_CORRECCION = 1.2;  // Ganancia para corrección en avance

// ====================== PARÁMETROS DE NAVEGACIÓN ======================
constexpr float FRENTE_ANILLO      = 0.0f;     // Ángulo frontal del robot
constexpr float TOL_APUNTADO       = 40.0f;    // Tolerancia para dar por alineado
constexpr float YAW_PORTERIA       = 0.0f;     // Ángulo de la portería propia
constexpr float TOL_PORTERIA       = 15.0f;    // Tolerancia al ir a portería
constexpr float DISTANCIA_SEGURA_CM = 25.0f;   // Distancia a pared para detenerse

constexpr unsigned long TIMEOUT_PELOTA_MS = 300;   // Tiempo sin datos para reiniciar búsqueda
constexpr unsigned long TIEMPO_DETENCION_MS = 500; // Pausa tras detenerse
constexpr unsigned long T_QUIETO_MS           = 4000; // Calibración inicial del MPU

constexpr int N_CAPTURA = 3;   // Mínimo de sensores activos para considerar captura
constexpr int LOOP_DELAY_MS = 20;  // 50 Hz

// ====================== VARIABLES GLOBALES ======================
extern volatile float yaw;
extern volatile int estadoIR;
extern volatile float anguloIR;
extern volatile int nIR;
extern volatile unsigned long tUltimaVezPelota;
extern volatile int distFrente, distAtras, distIzq, distDer;
extern volatile int velAvanceActual;
extern volatile unsigned long tFrenoIniciado;
extern volatile unsigned long tSenalEstable;

extern HardwareSerial Enlace;
extern char uartBuffer[64];
extern volatile int bufIndex;
extern volatile unsigned long ultimoDato;

// Máquina de estados
enum EstadoRobot { ESPERANDO_PELOTA, ALINEANDO_PELOTA, AVANZANDO_PELOTA, IR_A_NORTE, DETENIDO };
extern volatile EstadoRobot estadoActual;

// Variables auxiliares
extern volatile int pasoBusqueda;
extern volatile unsigned long tBusqueda;
extern volatile bool pelotaPerdidaReciente;
extern String recepVecinos;

extern volatile float gyroZoffset;

extern volatile unsigned long tPrev;

#endif // CONFIG_H
