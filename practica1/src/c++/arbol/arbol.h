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
#include <fstream>
#include "../nodo/nodo.h"

class Arbol {
 public:
  Arbol() : raiz_(nullptr) {}
  Arbol(Nodo* raiz, const std::string& nombre_fichero);

  void BusquedaAmplitud();
  void BusquedaProfundidad();

  Nodo* GetRaiz() const { return raiz_; }
  int GetNumeroVertices() const { return numero_vertices_; }
  int GetNumeroAristas() const { return numero_aristas_; }
  const std::vector<std::vector<float>>& GetDistancias() const { return distancias_; }

 private:
  Nodo* raiz_; // nodo origen
  int numero_vertices_, numero_aristas_;
  std::vector<std::vector<float>> distancias_;
};