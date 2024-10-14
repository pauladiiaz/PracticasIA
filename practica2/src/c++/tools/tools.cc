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

void Menu() {
  std::cout << "Menú" << std::endl;
  std::cout << "  [1] Cambiar entrada y salida" << std::endl << "  [2] Cambiar fichero" << std::endl << "  [3] Generar camino" 
            << std::endl << "  [4] Salir del programa" << std::endl;
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
      std::cerr << "Error: Opción no válida" << std::endl;
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
  std::cout << "Modo de empleo: ./bin/BusquedasInformadas [fichero]" << std::endl;
  std::cout << "Programa que implementa la búsqueda A* para encontrar el camino mínimo desde la entrada hasta la salida de un laberinto" << std::endl;
  std::cout << "Formato del fichero:" << std::endl;
  std::cout << "  El laberinto representado mediante:" << std::endl;
  std::cout << "    3: Casilla de entrada del laberinto" << std::endl;
  std::cout << "    4: Casilla de salida del laberinto" << std::endl;
  std::cout << "    1: Obstáculo. La casilla no es transitable" << std::endl;
  std::cout << "    0: Casillas de paso" << std::endl;
}
