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
#include "grafo/grafo.h"

int main(int argc, char* argv[]) {
  std::string nombre_fichero = "";
  int error = CheckErrors(CheckParams(argc, argv, nombre_fichero));
  if (error != 0) exit(EXIT_FAILURE);
  size_t pos_inicio = nombre_fichero.find_last_of('/') + 1; // +1 para omitir el '/'
  size_t pos_fin = nombre_fichero.find_last_of('.');
  std::string instancia = nombre_fichero.substr(pos_inicio, pos_fin - pos_inicio);
  std::string nombre_fichero_salida = std::string("saves/salida_") + instancia + std::string(".txt");
  std::cout << nombre_fichero_salida << std::endl;
  std::ofstream fichero_salida(nombre_fichero_salida);
  
  if (!fichero_salida) throw std::runtime_error("Error: No se ha podido abrir el fichero de salida");

  int opcion;
  Grafo grafo(nombre_fichero);
  do {
    grafo.GetLaberinto().ImprimirLaberinto();
    std::cout << grafo.GetLaberinto().GetEntrada()->GetCoordenada() << std::endl;
    std::cout << grafo.GetLaberinto().GetSalida()->GetCoordenada() << std::endl;
    std::cout << grafo.GetNumeroFilas() << " " << grafo.GetNumeroColumnas() << std::endl;
    // Crear el laberinto a partir del fichero
    Menu();
    std::cin >> opcion;
    if (opcion < 1 || opcion > 4) {
      CheckErrors(-3);
      exit(EXIT_FAILURE);
    }
    switch (opcion)
    {
    case 1: {// Cambiar entrada y salida
      std::cout << "Introduzca las coordenadas de la nueva entrada (se empiezan desde 0): " << std::endl;
      std::cout << "Fila: ";
      int x, y;
      std::cin >> x;
      std::cout << "Columna: ";
      std::cin >> y;
      Coordenada nueva_entrada(x, y);
      grafo.CambiarEntrada(nueva_entrada);
      std::cout << "Introduzca las coordenadas de la nueva salida: " << std::endl;  
      std::cout << "Fila: ";
      std::cin >> x;
      std::cout << "Columna: ";
      std::cin >> y;
      Coordenada nueva_salida(x, y);
      grafo.CambiarSalida(nueva_salida);
      break;
    }
    case 2: { // Cambiar fichero
      std::cout << "Introduzca el nombre del fichero: ";
      std::cin >> nombre_fichero;
      pos_inicio = nombre_fichero.find_last_of('/') + 1; // +1 para omitir el '/'
      pos_fin = nombre_fichero.find_last_of('.');
      std::string instancia = nombre_fichero.substr(pos_inicio, pos_fin - pos_inicio);
      grafo = Grafo(nombre_fichero);
      fichero_salida.close();
      fichero_salida = std::ofstream(std::string("saves/salida_") + instancia + std::string(".txt"));
      break;
    }
    case 3: { // Buscar solución
      fichero_salida << "-----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      fichero_salida << "Instancia      n    m    S   E                  Camino         Coste  Número de nodos generados   Número de nodos inspeccionados" << std::endl;
      fichero_salida << "-----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
      fichero_salida << "    " << instancia << "    " << grafo.GetNumeroFilas() << "   " << grafo.GetNumeroColumnas() << "  " << grafo.GetLaberinto().GetEntrada()->GetCoordenada();
      fichero_salida << "  " << grafo.GetLaberinto().GetSalida()->GetCoordenada() << "  ";

      grafo.BusquedaA(fichero_salida);
      break;
    }
    default: {
      std::cout << "Fin del programa" << std::endl;
      fichero_salida.close();
      break;
    }
    }
  } while (opcion != 4);
}