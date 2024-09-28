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
#include <vector>
#include <map>

class Nodo {
 public:
  Nodo() = default;
  Nodo(int numero_vertice) : numero_vertice_(numero_vertice) {}
  const std::vector<std::pair<Nodo*, int>>& GetSucesores() const { return sucesores_; }
  int GetNumero() const { return numero_vertice_; }
  void SetSucesores(const std::map<std::pair<int, int>, int>& distancias);
  void NuevoSucesor(Nodo* nuevo_sucesor);
  void SetNodoPadre(Nodo* padre);
  
 private:
  int numero_vertice_;
  Nodo* nodo_padre_;
  std::vector<std::pair<Nodo*, int>> sucesores_;
};