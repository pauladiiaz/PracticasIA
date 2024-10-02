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
  Nodo() : nodo_padre_(nullptr) {}
  Nodo(int numero_vertice) : numero_vertice_(numero_vertice), nodo_padre_(nullptr) {}
  const std::vector<std::pair<Nodo*, int>>& GetSucesores() const { return sucesores_; }
  int GetNumero() const { return numero_vertice_; }
  Nodo* GetPadre() const { return nodo_padre_; }
  void NuevoSucesor(Nodo* nuevo_sucesor, const int& coste);
  void SetNodoPadre(Nodo* padre);
  bool operator<(const Nodo& otro) const { return this->GetNumero() < otro.GetNumero(); }

  bool operator==(const Nodo& other) const { return numero_vertice_ == other.numero_vertice_; }
  
 private:
  int numero_vertice_;
  Nodo* nodo_padre_;
  std::vector<std::pair<Nodo*, int>> sucesores_;
};