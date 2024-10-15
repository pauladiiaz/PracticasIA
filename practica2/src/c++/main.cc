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

#include <vector>
#include "tools/tools.h"
#include "grafo/grafo.h"

int main(int argc, char* argv[]) {
  std::srand(std::time(0));
  std::string nombre_fichero_entrada = "";
  int error = CheckErrors(CheckParams(argc, argv, nombre_fichero_entrada));
  if (error != 0) exit(EXIT_FAILURE);
  size_t pos_inicio = nombre_fichero_entrada.find_last_of('/') + 1; // +1 para omitir el '/'
  size_t pos_fin = nombre_fichero_entrada.find_last_of('.');
  std::string instancia = nombre_fichero_entrada.substr(pos_inicio, pos_fin - pos_inicio);
  std::string nombre_fichero_salida("saves/salida.txt");

  int opcion;
  Grafo grafo(nombre_fichero_entrada);
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
      std::cin >> nombre_fichero_entrada;
      pos_inicio = nombre_fichero_entrada.find_last_of('/') + 1; // +1 para omitir el '/'
      pos_fin = nombre_fichero_entrada.find_last_of('.');
      instancia = nombre_fichero_entrada.substr(pos_inicio, pos_fin - pos_inicio);
      grafo = Grafo(nombre_fichero_entrada);
      break;
    }
    case 3: { // Buscar solución
      grafo.BusquedaA(nombre_fichero_salida, instancia);
      break;
    }
    default: {
      std::cout << "Fin del programa" << std::endl;
      break;
    }
    }
  } while (opcion != 4);
}