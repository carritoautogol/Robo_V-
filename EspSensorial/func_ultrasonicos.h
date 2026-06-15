#ifndef FUNC_ULTRASONICOS_H
#define FUNC_ULTRASONICOS_H

/**
 * @brief Mide la distancia con un sensor HC-SR04.
 * @param trigPin Pin de trigger.
 * @param echoPin Pin de echo.
 * @return Distancia en centímetros, o 999 si no hay eco.
 */
int medirDistancia(int trigPin, int echoPin);

/**
 * @brief Tarea de FreeRTOS que actualiza periódicamente las distancias.
 */
void TareaUltrasonicos(void * pvParameters);

/**
 * @brief Inicia la tarea de ultrasonidos en el núcleo 0.
 */
void iniciarUltrasonicos();

#endif // FUNC_ULTRASONICOS_H
