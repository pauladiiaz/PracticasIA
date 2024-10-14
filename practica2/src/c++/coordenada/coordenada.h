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
#include <iostream>

class Coordenada {
 public:
  Coordenada() = default;
  Coordenada(int x, int y) {
    if (x < 0 || y < 0) throw std::runtime_error("Cooordenada no válida");
    x_ = x;
    y_ = y;
  }
  void SetX(const int& x) { 
    if (x < 0) throw std::runtime_error("Cooordenada no válida");
    else x_ = x;
  }
  void SetY(const int& y) {
    if (y < 0) throw std::runtime_error("Cooordenada no válida");
    else y_ = y;
  }
  void SetCoordenada(const Coordenada& coordenada) {
    if (coordenada.x_ < 0 || coordenada.y_ < 0) throw std::runtime_error("Cooordenada no válida");
    x_ = coordenada.x_;
    y_ = coordenada.y_;
  }
  int GetX() const { return x_; }
  int GetY() const { return y_; }

  bool operator==(const Coordenada& other) const {
    return x_ == other.x_ && y_ == other.y_;
  }

  bool operator!=(const Coordenada& other) const {
    return x_ != other.x_ || y_ != other.y_;
  }

  Coordenada& operator=(const Coordenada& other) {
    if (other.x_ < 0 || other.y_ < 0) {
      throw std::runtime_error("Coordenada no válida");
    }
    x_ = other.x_;
    y_ = other.y_;
    return *this;
  }
  friend std::ostream& operator<<(std::ostream& os, const Coordenada& coordenada) {
    os << "(" << coordenada.x_ << ", " << coordenada.y_ << ")";
    return os;
  }
 private:
  int x_, y_;
};