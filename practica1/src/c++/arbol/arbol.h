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
#pragma once
#include <iostream>
#include "../nodo/nodo.h"

class Arbol {
 public:
  Arbol() : raiz_(nullptr) {}
  Arbol(Nodo* raiz) : raiz_(raiz) {}

  void BusquedaAmplitud();

  Nodo* GetRaiz() const { return raiz_; }
 private:
  Nodo* raiz_;
};