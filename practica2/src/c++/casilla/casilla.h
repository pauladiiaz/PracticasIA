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
#include "../coordenada/coordenada.h"
class Nodo;
#include "../nodo/nodo.h"

class Casilla {
 public:
  Casilla() : nodo_asociado_(nullptr) {}
  Casilla(int tipo, Coordenada coordenadas) : tipo_(tipo), coordenadas_(coordenadas), nodo_asociado_(nullptr) {}
  Casilla(int tipo, Coordenada coordenadas, Nodo* nodo_asociado) : tipo_(tipo), coordenadas_(coordenadas), nodo_asociado_(nodo_asociado) {}
  friend std::ostream& operator<<(std::ostream& os, const Casilla& casilla) {
    os << casilla.tipo_;
    return os;
  }
  bool EsTransitable() const { return tipo_ == 0 || tipo_ == 4; }

  void SetCoordenadas(const Coordenada& coordenadas) { coordenadas_ = coordenadas; }
  void SetTipo(int tipo) { tipo_ = tipo; }
  void SetNodo(Nodo* nodo) { nodo_asociado_ = nodo; }

  int GetTipo() const { return tipo_; }
  Coordenada GetCoordenada() const { return coordenadas_; }
  Nodo* GetNodo() const { return nodo_asociado_; }

 private:
  int tipo_;
  Coordenada coordenadas_;
  Nodo* nodo_asociado_;
};