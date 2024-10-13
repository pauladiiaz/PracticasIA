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
#include <iostream>
#include <sstream>
#include "../casilla/casilla.h"


class Laberinto {
 public:
  Laberinto() {}
  Laberinto(const std::string&, int&, int&);

  Laberinto& operator=(const Laberinto& other);
  Casilla* GetEntrada() const { return entrada_; }
  Casilla* GetSalida() const { return salida_; }
  std::vector<std::vector<Casilla>> GetCasillas() const { return casillas_; }

  std::vector<Casilla*> GetCasillasAdyacentes(Casilla*);

  void CambiarEntrada(Coordenada);
  void CambiarSalida(Coordenada);
  void ImprimirLaberinto();

 private:
 std::vector<std::vector<Casilla>> casillas_;
 Casilla* entrada_;
 Casilla* salida_;
};