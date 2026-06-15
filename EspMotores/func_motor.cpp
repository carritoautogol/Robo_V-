#include "func_motor.h"
#include "config.h"

/**
 * @brief Inicializa un pin PWM usando el periférico ledc.
 */
void pwmInit(int pin) {
  ledcAttach(pin, PWM_FREQ, PWM_RES);
}

/**
 * @brief Escribe el valor de PWM en un pin.
 */
void pwmWrite(int pin, int d) {
  ledcWrite(pin, d);
}

/**
 * @brief Controla una rueda con dirección y velocidad.
 * @param pwm Pin PWM.
 * @param in1, in2 Pines de dirección.
 * @param v Velocidad (-PWM_MAX..PWM_MAX).
 * @param inv Si true, invierte el sentido.
 */
void rueda(int pwm, int in1, int in2, int v, bool inv) {
  if (inv) v = -v;
  v = constrain(v, -PWM_MAX, PWM_MAX);
  
  if (v > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    pwmWrite(pwm, v);
  } else if (v < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    pwmWrite(pwm, -v);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    pwmWrite(pwm, 0);
  }
}

// Wrappers para cada rueda
void FL(int v) { rueda(FL_PWM, FL_IN1, FL_IN2, v, INVERTIR_FL); }
void FR(int v) { rueda(FR_PWM, FR_IN1, FR_IN2, v, INVERTIR_FR); }
void RL(int v) { rueda(RL_PWM, RL_IN1, RL_IN2, v, INVERTIR_RL); }
void RR(int v) { rueda(RR_PWM, RR_IN1, RR_IN2, v, INVERTIR_RR); }
void ruedas(int fl, int fr, int rl, int rr) {
  FL(fl); FR(fr); RL(rl); RR(rr);
}

/**
 * @brief Detiene el robot y reinicia la rampa de aceleración.
 */
void detener() {
  if (velAvanceActual > 0) velAvanceActual -= RAMPA_PASO;
  if (velAvanceActual <= 0) velAvanceActual = 0;
  ruedas(velAvanceActual, velAvanceActual, velAvanceActual, velAvanceActual);
}

/**
 * @brief Gira sobre su propio eje.
 * @param v Velocidad (positivo = derecha, negativo = izquierda).
 */
void girarEnSitio(int v) {
  ruedas(v, -v, v, -v);
}

/**
 * @brief Avanza con aceleración suave y corrección angular.
 * @param objetivo Velocidad final deseada.
 * @param corr Corrección para girar (positivo = derecha).
 */
void avanzarSuave(int objetivo, int corr) {
  if (velAvanceActual < objetivo) velAvanceActual += RAMPA_PASO;
  if (velAvanceActual > objetivo) velAvanceActual = objetivo;
  ruedas(velAvanceActual + corr, velAvanceActual - corr,
         velAvanceActual + corr, velAvanceActual - corr);
}
