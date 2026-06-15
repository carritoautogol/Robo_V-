#ifndef FUNC_GIRO_H
#define FUNC_GIRO_H

#include <Arduino.h>

#define MPU_ADDR 0x68

// Funciones de escritura/lectura del MPU6050
void mpuW(uint8_t r, uint8_t v);
int16_t mpuGz();

// Inicialización y calibración
bool mpuInit();
void calibrarGyro();

// Actualización del rumbo (llamar periódicamente)
void actualizarRumbo();

// Utilidad: normaliza un ángulo a [-180, 180]
float errorAngular(float angulo);

#endif // FUNC_GIRO_H
