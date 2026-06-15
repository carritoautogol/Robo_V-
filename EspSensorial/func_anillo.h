#ifndef FUNC_ANILLO_H
#define FUNC_ANILLO_H

/**
 * @brief Encuentra el bloque contiguo más largo de sensores activos
 *        y calcula el ángulo mediante centroide.
 * @return Número de sensores activos consecutivos más largo.
 */
int ubicarPelota();

/**
 * @brief Lee todos los canales del multiplexor y actualiza el vector 'activo[]'.
 * @param totalActivos Referencia donde se almacena la cantidad de sensores activos.
 */
void fotorreceptoresActivos(int& totalActivos);

#endif // FUNC_ANILLO_H
