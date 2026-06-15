#ifndef FUNC_MOTOR_H
#define FUNC_MOTOR_H

void pwmInit(int pin);
void pwmWrite(int pin, int d);

// Control individual de cada rueda
void rueda(int pwm, int in1, int in2, int v, bool inv);
void FL(int v);
void FR(int v);
void RL(int v);
void RR(int v);
void ruedas(int fl, int fr, int rl, int rr);

// Comandos de alto nivel
void detener();
void girarEnSitio(int v);
void avanzarSuave(int objetivo, int corr);

#endif // FUNC_MOTOR_H
