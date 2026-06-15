#include "func_giro.h"
#include "config.h"
#include <Wire.h>

/**
 * @brief Escribe un registro del MPU6050 vía I2C.
 * @param r Dirección de registro.
 * @param v Valor a escribir.
 */
void mpuW(uint8_t r, uint8_t v) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(r);
  Wire.write(v);
  Wire.endTransmission();
}

/**
 * @brief Lee la velocidad angular en el eje Z.
 * @return Valor crudo del registro (0x47).
 */
int16_t mpuGz() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x47);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, (uint8_t)2);
  return (Wire.read() << 8) | Wire.read();
}

/**
 * @brief Inicializa el MPU6050 y verifica la comunicación I2C.
 * @return true si se detecta el sensor, false en caso contrario.
 */
bool mpuInit() {
  Wire.begin(21, 22);        // SDA=21, SCL=22
  Wire.setClock(400000);
  Wire.setTimeout(25);
  
  Wire.beginTransmission(MPU_ADDR);
  if (Wire.endTransmission() != 0) return false;
  
  mpuW(0x6B, 0x00);   // Salir del modo sleep
  delay(100);
  mpuW(0x1B, 0x00);   // Configurar giroscopio a ±250°/s
  delay(50);
  return true;
}

/**
 * @brief Calibra el offset del giroscopio tomando 500 lecturas estáticas.
 */
void calibrarGyro() {
  long s = 0;
  for (int i = 0; i < 500; i++) {
    s += mpuGz();
    delay(3);
  }
  gyroZoffset = (float)s / 500.0;
}

/**
 * @brief Actualiza el rumbo integrando la velocidad angular.
 * Debe llamarse en cada iteración del loop principal.
 */
void actualizarRumbo() {
  unsigned long n = micros();
  float dt = (n - tPrev) / 1000000.0;
  tPrev = n;
  if (dt > 0.05) dt = 0.02;
  
  float gz = (mpuGz() - gyroZoffset) / 131.0;   // Conversión a °/s
  yaw += gz * dt;
  
  if (yaw >= 360) yaw -= 360;
  if (yaw < 0) yaw += 360;
}

/**
 * @brief Normaliza un ángulo al rango [-180, 180].
 * @param angulo Ángulo en grados.
 * @return Ángulo en el rango [-180, 180].
 */
float errorAngular(float angulo) {
  while (angulo > 180) angulo -= 360;
  while (angulo < -180) angulo += 360;
  return angulo;
}
