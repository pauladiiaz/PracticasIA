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

#include "arbol.h"

Arbol::Arbol(Nodo* raiz, const std::string& nombre_fichero) : raiz_(raiz) {
  // Leer los datos desde el fichero
  std::ifstream fichero(nombre_fichero);
  if (!fichero.is_open()) {
    throw std::runtime_error("Error al abrir el fichero.");
  }
  std::string linea;
  getline(fichero, linea);
  numero_vertices_ = std::stoi(linea);
  distancias_.resize(numero_vertices_, std::vector<float>(numero_vertices_, 0.0f));
  for (int i = 0; i < numero_vertices_; ++i) {
    for (int j = i + 1; j < numero_vertices_; ++j) { // Como las distancias son simétricas, se empieza un vértice por delante
      if (getline(fichero, linea)) {
        distancias_[i][j] = std::stof(linea);
        distancias_[j][i] = std::stof(linea);
        if (distancias_[i][j] != -1) numero_aristas_++;
      } else {
        throw std::runtime_error("Error al leer el fichero.");
      }
    }
  }
}