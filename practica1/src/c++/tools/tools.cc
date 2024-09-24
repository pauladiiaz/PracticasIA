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

#include "tools.h"

/**
 * @brief Función que revisa que el número de parámetros instroducido es correcto
 * @param argc Número de parámetros
 * @param argv Parámetros introducidos
 * @param nombre_fichero Nombre del fichero de entrada
 * @return El código de error
*/
int CheckParams(int argc, char* argv[], std::string& nombre_fichero) {
  if (argc != 2) {
    return -1;
  } else {
    nombre_fichero = argv[1];
    return 0;
  }
}

/**
 * @brief Función que controla los errores y sus mensajes
 * @param error El código de error de la comprobación de parámetros
 * @return El código de error
*/
int CheckErrors(const int& error) {
  if (error == -1) {
    std::cerr << "Error: Se ha introducido un número de parámetros no esperado" << std::endl;
    Help();
  }
  return error;
}

/**
 * @brief Función que imprime información sobre el programa
*/
void Help() {
  std::cout << "Modo de empleo: ./bin/BusquedasNoInformadas [fichero]" << std::endl;
  std::cout << "Programa que implementa búsquedas no informadas para encontrar un camino entre dos vértices de un grafo" << std::endl;
  std::cout << "Formato del fichero:" << std::endl;
  std::cout << "  Primera fila: número de vértices" << std::endl;
  std::cout << "  Siguientes filas: se enumeran las distancias entre los pares de vértices" << std::endl;
  std::cout << "Si no hay una arista que conecte el vértice i con el vértice j, la distancia es -1" << std::endl;
}

void LeerFichero(const std::string& nombre_fichero, int& numero_vertices, int& numero_aristas, std::vector<std::vector<float>>& distancias) {
  std::ifstream fichero(nombre_fichero);
  if (!fichero.is_open()) {
    throw std::runtime_error("Error al abrir el fichero.");
  }
  std::string linea;
  getline(fichero, linea);
  numero_vertices = std::stoi(linea);
  distancias.resize(numero_vertices, std::vector<float>(numero_vertices, 0.0f));
  for (int i = 0; i < numero_vertices; ++i) {
    for (int j = i + 1; j < numero_vertices; ++j) { // Como las distancias son simétricas, se empieza un vértice por delante
      if (getline(fichero, linea)) {
        distancias[i][j] = std::stof(linea);
        distancias[j][i] = std::stof(linea);
        if (distancias[i][j] != -1) numero_aristas++;
      } else {
        throw std::runtime_error("Error al leer el fichero.");
      }
    }
  }
}
