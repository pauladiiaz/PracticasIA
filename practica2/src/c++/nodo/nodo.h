/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * 3º Curso en Grado en Ingeniería Informática
 * Inteligencia Artificial
 * Práctica 2: Búsquedas informadas
 *
 * @author Paula Díaz Jorge alu0101540863@ull.edu.es
 * @date 10 oct 2024
*/
#pragma once
#include <vector>
#include <unordered_set>
#include <map>
class Casilla;
#include "../casilla/casilla.h"
#include "../coordenada/coordenada.h"

class Nodo {
 public:
  Nodo() : nodo_padre_(nullptr) {}
  Nodo(int numero_vertice) : numero_vertice_(numero_vertice), nodo_padre_(nullptr) {}
  Nodo(int numero_vertice, Casilla* casilla_asociada) : numero_vertice_(numero_vertice), 
      casilla_asociada_(casilla_asociada), nodo_padre_(nullptr), valor_f_(0) {}

  int GetNumero() const { return numero_vertice_; }
  Nodo* GetPadre() const { return nodo_padre_; }
  Casilla* GetCasilla() const { return casilla_asociada_; }
  int GetValorF() const { return valor_f_; }
  int GetValorH() const { return valor_h_; }
  int GetValorG() const { return valor_g_; }
  int GetVertice() const { return numero_vertice_; }

  void SetValorF(int valor) { valor_f_ = valor; }
  void SetValorG(int valor) { valor_g_ = valor; }
  void SetValorH(int valor) { valor_h_ = valor; }
  void SetNodoPadre(Nodo* padre) { nodo_padre_ = padre; };
  void SetCasilla(Casilla* casilla) { casilla_asociada_ = casilla; }

  bool operator<(const Nodo& otro) const { return this->GetNumero() < otro.GetNumero(); }
  bool operator==(const Nodo& other) const { return this->casilla_asociada_ == other.casilla_asociada_; }
  
 private:
  int numero_vertice_;
  Casilla* casilla_asociada_;
  Nodo* nodo_padre_;
  int valor_f_, valor_g_, valor_h_;
};