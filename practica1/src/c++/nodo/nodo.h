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

class Nodo {
 public:
  Nodo() = default;
  Nodo(int numero_vertice) : numero_vertice_(numero_vertice) {}
  std::vector<Nodo*> GetSucesores() const { return sucesores_; }
  int GetNumero() const { return numero_vertice_; }
  
 private:
  int numero_vertice_;
  std::vector<Nodo*> sucesores_;
  Nodo* nodo_padre_;
};