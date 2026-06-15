  #include "config.h"

  // ====================== COMUNICACIÓN UART ======================
  HardwareSerial Enlace(2);   // Serial2 para comunicación con el anillo
  char uartBuffer[64];        // Buffer de recepción
  volatile int bufIndex = 0;
  volatile unsigned long ultimoDato = 0;

  // ====================== VARIABLES DE GIROSCOPIO ======================
  volatile float yaw = 0.0f;
  volatile float gyroZoffset = 0.0f;
  volatile unsigned long tPrev = 0;

  // ====================== VARIABLES DEL ANILLO ======================
  volatile float anguloIR = -1.0f;
  volatile int estadoIR = 0;
  volatile int nIR = 0;

  // ====================== VARIABLES DE ULTRASONIDOS ======================
  volatile int distFrente = 999;
  volatile int distAtras  = 999;
  volatile int distIzq    = 999;
  volatile int distDer    = 999;

  // ====================== VARIABLES DE MOTORES ======================
  volatile int velAvanceActual = 0;
  volatile unsigned long tSenalEstable = 0;

  // ====================== VARIABLES DE ESTADO ======================
  volatile EstadoRobot estadoActual = ESPERANDO_PELOTA;
  volatile unsigned long tFrenoIniciado = 0;
  volatile unsigned long tUltimaVezPelota = 0;

  volatile int pasoBusqueda = 0;
  volatile unsigned long tBusqueda = 0;
  volatile bool pelotaPerdidaReciente = false;
  String recepVecinos;
