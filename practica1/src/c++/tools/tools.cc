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
  switch (error) {
    case -1: {
      std::cerr << "Error: Se ha introducido un número de parámetros no esperado" << std::endl;
      Help();
      break;
    }
    case -2: {
      std::cerr << "Error al abrir el archivo de salida" << std::endl;
      break;
    }
    case -3: {
      std::cerr << "Error: No se ha introducido correctamente el tipo de búsqueda" << std::endl;
      break;
    }
    case -4: {
      std::cerr << "Error: El nodo origen no existte" << std::endl;
      break;
    }
    case -5: {
      std::cerr << "Error: El nodo destino no existe" << std::endl;
      break;
    }
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
