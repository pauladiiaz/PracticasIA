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

  std::cout << "Introduzca el vértice de origen y de destino: " << std::endl;
  int origen, destino;
  std::cout << "Origen: ";
  std::cin >> origen; // El vértice origen es el nodo raíz
  std::cout << "Destino: ";
  std::cin >> destino;
  std::cout << "Introduzca si desea una búsqueda en profundidad (p) o en anchura (a) ";
  char opcion;
  std::cin >> opcion;
  if (opcion != 'p' && opcion != 'a') {
    CheckErrors(-3);
    exit(EXIT_FAILURE);
  }
  Nodo* nodo_raiz = new Nodo(origen); // El nodo raiz es la arista origen
  Grafo grafo(nodo_raiz, nombre_fichero, origen, destino);
  // std::cout << "Distancias: " << std::endl;
  // for (const auto& par : arbol.GetDistancias()) {
  //   std::cout << "(" << par.first.first << ", " << par.first.second << ") Coste: " << par.second << std::endl;
  // }

  std::ofstream fichero_salida("saves/salida.txt");
  if (!fichero_salida) {
    CheckErrors(-2);
    exit(EXIT_FAILURE);
  }
  fichero_salida << "---------------------------------------------" << std::endl;
  fichero_salida << "Número de nodos del grafo: " << grafo.GetNumeroVertices() << std::endl;
  fichero_salida << "Número de aristas del grafo: " << grafo.GetNumeroAristas() << std::endl;
  fichero_salida << "Vértice origen: " << origen << std::endl;
  fichero_salida << "Vértice destino: " << destino << std::endl;
  fichero_salida << "---------------------------------------------" << std::endl;

  if (opcion == 'a') {
    grafo.BusquedaAmplitud(fichero_salida);
  } else {
    grafo.BusquedaProfundidad(fichero_salida);
  }
  
  std::cout << "La salida se ha guardado en saves/salida.txt" << std::endl;
  fichero_salida.close();
}