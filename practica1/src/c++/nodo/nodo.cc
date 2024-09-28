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

void Nodo::SetSucesores(const std::map<std::pair<int, int>, int>& distancias) {
  bool terminado = false;
  int it = numero_vertice_ + 1;
  while (!terminado) {
    sucesores_.emplace_back(new Nodo(it));
  }
}

void Nodo::NuevoSucesor(Nodo* nuevo_sucesor) {
  sucesores_.emplace_back(nuevo_sucesor);
}

void Nodo::SetNodoPadre(Nodo* padre) {
  nodo_padre_ = padre;
}