#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==================================================================
// PARÁMETROS AJUSTABLES
// ==================================================================

  extern const float FRENTE_ANILLO; // Ángulo exacto donde el frente del chasis ataca la pelota
  extern const float TOL_APUNTADO;  // *** AUMENTADO A 40 PARA PRUEBAS CON LED ***
  extern const unsigned long T_QUIETO_MS; // Milisegundos de congelamiento inicial para afinar MPU
  extern const int N_CAPTURA; // Cantidad de sensores iluminados que confirman posesión
  extern const float YAW_PORTERIA;  // Grados hacia la red del oponente
  extern const float TOL_PORTERIA;  // Margen de desviación permitido al avanzar a la portería

 /* ===================== PINES DE CONTROL DE MOTORES ========================== */
  constexpr uint8_t STBY = 4; // Pin de habilitación (Standby) de los chips TB6612FNG

  constexpr uint8_t FL_PWM = 25; // Control de velocidad Motor Frontal Izquierdo
  constexpr uint8_t FL_IN1 = 26; // Pin lógico de dirección 1 Frontal Izquierdo
  constexpr uint8_t FL_IN2 = 27; // Pin lógico de dirección 2 Frontal Izquierdo

  constexpr uint8_t RL_PWM = 33; // Control de velocidad Motor Trasero Izquierdo
  constexpr uint8_t RL_IN1 = 32; // Pin lógico de dirección 1 Trasero Izquierdo
  constexpr uint8_t RL_IN2 = 14; // Pin lógico de dirección 2 Trasero Izquierdo

  constexpr uint8_t FR_PWM = 13; // Control de velocidad Motor Frontal Derecho
  constexpr uint8_t FR_IN1 = 23; // Pin lógico de dirección 1 Frontal Derecho
  constexpr uint8_t FR_IN2 = 2;  // Pin lógico de dirección 2 Frontal Derecho

  constexpr uint8_t RR_PWM = 19; // Control de velocidad Motor Trasero Derecho
  constexpr uint8_t RR_IN1 = 18; // Pin lógico de dirección 1 Trasero Derecho
  constexpr uint8_t RR_IN2 = 5;  // Pin lógico de dirección 2 Trasero Derecho

  constexpr bool INVERTIR_FL = false;  // Parámetro para invertir giro rueda frontal izquierda
  constexpr bool INVERTIR_FR = false;  // Parámetro para invertir giro rueda frontal derecha
  constexpr bool INVERTIR_RL = false;  // Parámetro para invertir giro rueda trasera izquierda
  constexpr bool INVERTIR_RR = true;  // Parámetro para invertir giro rueda trasera derecha

  constexpr uint32_t PWM_FREQ = 20000; // Frecuencia de los motores en Hercios (20kHz)
  constexpr uint8_t PWM_RES = 8;     // Definición a 8 bits (Valores disponibles de 0 a 255)

  extern const int PWM_MAX;     // Tope absoluto de PWM para no quemar motores Faulhaber
  extern const int VEL_AVANCE;  // Potencia constante de ataque a la pelota
  extern const int VEL_GIRO;     // Potencia para rotar en el propio eje
  extern const int VEL_BUSCAR;   // Potencia para movimientos exploratorios

  extern int velAvanceActual;     // Variable que almacena en qué velocidad va la aceleración
  extern const int RAMPA_PASO;    // Puntos de velocidad que suma por cada ciclo (aceleración suave)
  extern unsigned long tSenalEstable; // Cronómetro para descartar brillos falsos rápidos
  extern const unsigned long T_CONFIRMA_MS; // Requisito de medio segundo de visión antes de atacar

/* ===================== COMUNICACIÓN UART =========================== */
  constexpr uint8_t RX_PIN = 34; // Pin GPIO de entrada conectado al cable que viene del anillo
  constexpr uint8_t TX_PIN = 17; // Pin de salida serial (Desactivado físicamente)

  extern HardwareSerial Enlace; // Inicia el puerto serial 2 por hardware nativo
  extern char uartBuffer[64];      // Memoria de 64 caracteres para guardar la frase que llega
  extern int bufIndex;         // Indicador de la posición actual dentro de la memoria

  extern float anguloIR;    // Variable que recibe los grados de la pelota
  extern int estadoIR;         // Variable que recibe el estado (Pelota sí o Pelota no)
  extern int nIR;              // Variable que recibe el conteo de sensores iluminados
  extern unsigned long ultimoDato; // Temporizador para apagar el robot si se corta el cable

/* ===================== NAVEGACIÓN MPU ====================== */
  extern const uint8_t MPU_ADDR; // Dirección de red I2C del sensor de movimiento
  extern float yaw;               // Orientación actual del robot en la cancha
  extern float gyroZoffset;       // Basura electrónica que genera el MPU estando quieto (se calibra)
  extern unsigned long tPrev;       // Diferencial de tiempo para integrar ángulos matemáticos

/* ===================== MÁQUINA DE ESTADOS COMPLEJA ===================== */
  enum EstadoRobot { ESPERANDO_PELOTA, BUSCANDO, PERSIGUIENDO, FRENANDO, REGRESANDO }; // Fases del partido
  extern EstadoRobot estadoActual; // Arranca en fase de bloqueo inicial
  extern unsigned long tFrenoIniciado;            // Marca de inicio del frenado de protección
  extern unsigned long tUltimaVezPelota;          // Marca del instante donde se vio la bola por última vez

// --- NUEVAS VARIABLES PARA LA SECUENCIA DE BÚSQUEDA ---
  extern int pasoBusqueda;            // Guarda el paso actual (0 al 7) del patrón exploratorio
  extern unsigned long tBusqueda;     // Temporizador exclusivo para medir las pausas y avances del patrón
  extern bool pelotaPerdidaReciente; // Bandera para saber si acabamos de perder la pelota

#endif
