#include "config.h"

// ============================================================
//  PARÁMETROS DE VELOCIDAD — variables mutables
// ============================================================

int           velAvanceActual = 0;  // Arranca en 0; la rampa lo lleva hasta VEL_AVANCE
unsigned long tSenalEstable   = 0;  // Se resetea cada vez que el encuadre se rompe

// ============================================================
//  COMUNICACIÓN UART — objeto y variables mutables
// ============================================================

HardwareSerial Enlace(2);  // Instancia del puerto Serial 2 por hardware nativo de la ESP32
char uartBuffer[64];       // Buffer de recepción de tramas; se pisa con cada mensaje nuevo
int  bufIndex = 0;         // Índice de escritura dentro del buffer

float         anguloIR   = -1.0f;  // -1.0 indica que aún no se ha recibido ningún dato
int           estadoIR   = 0;
int           nIR        = 0;
unsigned long ultimoDato = 0;

// ============================================================
//  NAVEGACIÓN — variables mutables
// ============================================================

float         yaw         = 0.0f;  // El norte se fija en 0 al terminar la calibración
float         gyroZoffset = 0.0f;  // Se sobreescribe por calibrarGyro() en el setup
unsigned long tPrev       = 0;

// ============================================================
//  NAVEGACION — ULTRASONICOS — variables mutables
// ============================================================

int distFrente = 999;
int distAtras  = 999;
int distIzq    = 999;
int distDer    = 999;

// ============================================================
//  MÁQUINA DE ESTADOS — variables mutables
// ============================================================

EstadoRobot estadoActual = ESPERANDO_PELOTA;  // Estado inicial al encender

unsigned long tFrenoIniciado   = 0;
unsigned long tUltimaVezPelota = 0;

int           pasoBusqueda         = 0;
unsigned long tBusqueda            = 0;
bool          pelotaPerdidaReciente = false;
