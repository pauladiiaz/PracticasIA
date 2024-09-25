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

#include <vector>
#include "tools/tools.h"
#include "arbol/arbol.h"

int main(int argc, char* argv[]) {
  std::string nombre_fichero = "";
  int error = CheckErrors(CheckParams(argc, argv, nombre_fichero));
  if (error != 0) exit(EXIT_FAILURE);
//   std::cout << numero_vertices << std::endl;
//   for (int i = 0; i < distancias.size(); ++i) {
//     for (int j = 0; j < distancias[i].size(); ++j) {
//         std::cout << distancias[i][j] << std::endl;
//     }
//   } 
  std::cout << "Introduzca el vértice de origen y de destino: " << std::endl;
  int origen, destino;
  std::cout << "Origen: ";
  std::cin >> origen; // El vértice origen es el nodo raíz
  std::cout << "Destino: ";
  std::cin >> destino;
  Nodo* nodo_raiz = new Nodo(origen);
  Arbol arbol(nodo_raiz, nombre_fichero);


  std::ofstream fichero_salida("saves/salida.txt");
  if (!fichero_salida) {
    CheckErrors(-2);
    exit(EXIT_FAILURE);
  }
  fichero_salida << "---------------------------------------------" << std::endl;
  fichero_salida << "Número de nodos del grafo: " << arbol.GetNumeroVertices() << std::endl;
  fichero_salida << "Número de aristas del grafo: " << arbol.GetNumeroAristas() << std::endl;
  fichero_salida << "Vértice origen: " << origen << std::endl;
  fichero_salida << "Vértice destino: " << destino << std::endl;
  fichero_salida << "---------------------------------------------" << std::endl;
  fichero_salida.close();
}