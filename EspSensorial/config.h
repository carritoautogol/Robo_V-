#ifndef CONFIG_H
#define CONFIG_H

#define TRIG_F 13 // Frente
#define ECHO_F 12

#define TRIG_B 14 // Atrás
#define ECHO_B 27

#define TRIG_L 32 // Izquierda
#define ECHO_L 33

#define TRIG_R 22 // Derecha
#define ECHO_R 23

// ============================================================
//  NAVEGACION — ULTRASONICOS — variables mutables
// ============================================================

int distFrente = 999;
int distAtras  = 999;
int distIzq    = 999;
int distDer    = 999;

#endif
