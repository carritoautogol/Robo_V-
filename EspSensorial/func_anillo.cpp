#include "func_anillo.h"
#include "config.h"
#include "func_multiplexor.h"
#include <math.h>

/**
 * @brief Implementación del algoritmo de centroide para determinar el ángulo
 *        de la pelota.
 */
int ubicarPelota() {
  int mejorInicio = -1, mejorLargo = 0;
  int iniActual = -1, largoActual = 0;
  
  // Buscar el segmento más largo de sensores activos (considerando circularidad)
  for (int k = 0; k < totalSensores * 2; k++) {
    int i = k % totalSensores;
    if (activo[i]) {
      if (largoActual == 0) iniActual = i;
      largoActual++;
      if (largoActual > mejorLargo) {
        mejorLargo = largoActual;
        mejorInicio = iniActual;
      }
    } else {
      largoActual = 0;
    }
  }
  
  if (mejorLargo > totalSensores) mejorLargo = totalSensores;
  angulo = -1.0;
  
  if (mejorLargo > 0) {
    float sx = 0, sy = 0;
    for (int j = 0; j < mejorLargo; j++) {
      int idx = (mejorInicio + j) % totalSensores;
      float rad = radians(idx * GRADOS_POR_SENSOR);
      sx += cos(rad);
      sy += sin(rad);
    }
    angulo = degrees(atan2(sy, sx));
    if (angulo < 0) angulo += 360.0;
  }
  return mejorLargo;
}

/**
 * @brief Escanea los 16 canales del multiplexor y actualiza el vector 'activo[]'.
 */
void fotorreceptoresActivos(int& totalActivos) {
  totalActivos = 0;
  for (int i = 0; i < totalSensores; i++) {
    seleccionarCanal(i);
    delayMicroseconds(100);   // Tiempo de estabilización
    activo[i] = (digitalRead(pinSIG) == LOW);   // TSSP58038 activo en LOW
    if (activo[i]) totalActivos++;
  }
}
