#include "config.h" 

// ==================================================================
// PARÁMETROS AJUSTABLES
// ==================================================================

const float FRENTE_ANILLO = 0.0;         // Ángulo exacto donde el frente del chasis ataca la pelota
const float TOL_APUNTADO = 40.0;         // *** AUMENTADO A 40 PARA PRUEBAS CON LED ***
const unsigned long T_QUIETO_MS = 4000;  // Milisegundos de congelamiento inicial para afinar MPU
const int N_CAPTURA = 3;                 // Cantidad de sensores iluminados que confirman posesión
const float YAW_PORTERIA = 0.0;          // Grados hacia la red del oponente
const float TOL_PORTERIA = 15.0;         // Margen de desviación permitido al avanzar a la portería

const int PWM_MAX = 182;     // Tope absoluto de PWM para no quemar motores Faulhaber
const int VEL_AVANCE = 120;  // Potencia constante de ataque a la pelota
const int VEL_GIRO = 85;     // Potencia para rotar en el propio eje
const int VEL_BUSCAR = 80;   // Potencia para movimientos exploratorios

int velAvanceActual = 0;                    // Variable que almacena en qué velocidad va la aceleración
const int RAMPA_PASO = 4;               // Puntos de velocidad que suma por cada ciclo (aceleración suave)
unsigned long tSenalEstable = 0;        // Cronómetro para descartar brillos falsos rápidos
const unsigned long T_CONFIRMA_MS = 500;  // Requisito de medio segundo de visión antes de atacar

/* ===================== COMUNICACIÓN UART =========================== */

HardwareSerial Enlace(2);  // Inicia el puerto serial 2 por hardware nativo
char uartBuffer[64];    // Memoria de 64 caracteres para guardar la frase que llega
int bufIndex = 0;           // Indicador de la posición actual dentro de la memoria

float anguloIR = -1.0;            // Variable que recibe los grados de la pelota
int estadoIR = 0;              // Variable que recibe el estado (Pelota sí o Pelota no)
int nIR = 0;                   // Variable que recibe el conteo de sensores iluminados
unsigned long ultimoDato = 0;  // Temporizador para apagar el robot si se corta el cable

/* ===================== NAVEGACIÓN MPU ====================== */
const uint8_t MPU_ADDR = 0x68;  // Dirección de red I2C del sensor de movimiento
float yaw = 0.0;               // Orientación actual del robot en la cancha
float gyroZoffset = 0.0;       // Basura electrónica que genera el MPU estando quieto (se calibra)
unsigned long tPrev = 0;     // Diferencial de tiempo para integrar ángulos matemáticos

/* ===================== MÁQUINA DE ESTADOS COMPLEJA ===================== */

EstadoRobot estadoActual = ESPERANDO_PELOTA;        // Arranca en fase de bloqueo inicial
unsigned long tFrenoIniciado = 0;    // Marca de inicio del frenado de protección
unsigned long tUltimaVezPelota = 0;  // Marca del instante donde se vio la bola por última vez

// --- NUEVAS VARIABLES PARA LA SECUENCIA DE BÚSQUEDA ---
int pasoBusqueda = 0;            // Guarda el paso actual (0 al 7) del patrón exploratorio
unsigned long tBusqueda = 0;     // Temporizador exclusivo para medir las pausas y avances del patrón
bool pelotaPerdidaReciente = false;  // Bandera para saber si acabamos de perder la pelota
