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
#include <unordered_set>
#include <map>
#include "../casilla/casilla.h"
#include "../coordenada/coordenada.h"

class Nodo {
 public:
  Nodo() : nodo_padre_(nullptr) {}
  Nodo(int numero_vertice) : numero_vertice_(numero_vertice), nodo_padre_(nullptr) {}
  Nodo(int numero_vertice, Casilla* casilla_asociada) : numero_vertice_(numero_vertice), 
      casilla_asociada_(casilla_asociada), nodo_padre_(nullptr), costo_acumulado_(0), valor_f_(0) {}
  int GetNumero() const { return numero_vertice_; }
  Nodo* GetPadre() const { return nodo_padre_; }
  Casilla* GetCasilla() const { return casilla_asociada_; }
  int GetCostoAcumulado() const { return costo_acumulado_; }
  int GetValorF() const { return valor_f_; }

  void SetValorF(int valor) { valor_f_ = valor; }
  void SetCostoAcumulado(int costo) { costo_acumulado_ = costo; }
  void SetNodoPadre(Nodo* padre);
  bool operator<(const Nodo& otro) const { return this->GetNumero() < otro.GetNumero(); }
  bool operator==(const Nodo& other) const { return numero_vertice_ == other.numero_vertice_; }
  
 private:
  int numero_vertice_;
  Casilla* casilla_asociada_;
  Nodo* nodo_padre_;
  int costo_acumulado_;
  int valor_f_;
};