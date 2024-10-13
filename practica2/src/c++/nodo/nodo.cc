/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 3º Curso en Grado en Ingeniería Informática
 * Inteligencia Artificial
 * Práctica 1: Búsquedas no informadas
 *
 * @author Paula Díaz Jorge alu0101540863@ull.edu.es
 * @date 24 sep 2024
*/
#include "nodo.h"

/**
 * @brief Método para asignarle un nodo padre al nodo
 * @param padre Nodo que será el nodo padre del actual
 */
void Nodo::SetNodoPadre(Nodo* padre) {
  if (nodo_padre_ == nullptr) nodo_padre_ = padre;
}
