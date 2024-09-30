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

void Nodo::NuevoSucesor(Nodo* nuevo_sucesor, const int& coste) {
  sucesores_.emplace_back(std::make_pair(nuevo_sucesor, coste));
}

void Nodo::SetNodoPadre(Nodo* padre) {
  nodo_padre_ = padre;
}