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
#include "../coordenada/coordenada.h"

class Casilla {
 public:
  Casilla() {}
  Casilla(int tipo, Coordenada coordenadas) : tipo_(tipo), coordenadas_(coordenadas) {}
  friend std::ostream& operator<<(std::ostream& os, const Casilla& casilla) {
    os << casilla.tipo_;
    return os;
  }
  bool EsTransitable() const { return tipo_ == 0; }

  void SetCoordenadas(const Coordenada& coordenadas) { coordenadas_ = coordenadas; }
  void SetTipo(int tipo) { tipo_ = tipo; }

  int GetTipo() const { return tipo_; }
  Coordenada GetCoordenada() const { return coordenadas_; }

 private:
  int tipo_;
  Coordenada coordenadas_;
};