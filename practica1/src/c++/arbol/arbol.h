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
#include <map>
#include <queue>
#include <algorithm>
#include "../nodo/nodo.h"

class Arbol {
 public:
  Arbol() : raiz_(nullptr) {}
  Arbol(Nodo* raiz, const std::string& nombre_fichero, const int&, const int&);

  void BusquedaAmplitud(std::ofstream&);
  void BusquedaProfundidad();

  void ImprimirSolucion(std::ofstream&, const std::vector<Nodo*>&, const std::vector<Nodo*>&, const int&);
  bool RevisarRama(Nodo*, Nodo*);
  void GenerarCoste(const std::vector<Nodo*>&, int&);
  void GenerarCamino(Nodo*, std::ofstream&, std::vector<Nodo*>&);

  Nodo* GetRaiz() const { return raiz_; }
  int GetNumeroVertices() const { return numero_vertices_; }
  int GetNumeroAristas() const { return numero_aristas_; }
  const std::map<std::pair<int, int>, int>& GetDistancias() const { return distancias_; }

 private:
  Nodo* raiz_; // nodo origen
  int numero_vertices_, numero_aristas_;
  std::map<std::pair<int, int>, int> distancias_;
  int origen_, destino_;
};