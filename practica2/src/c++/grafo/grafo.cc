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

#include "grafo.h"

struct ComparaNodo {
  bool operator()(Nodo* a, Nodo* b) const {
    return a->GetValorF() > b->GetValorF();
  }
};

/**
 * @brief Método que implementa la búsqueda A*
 * @param fichero Fichero de salida en el que se imprimirá la solución
*/

void Grafo::BusquedaA(std::ofstream& fichero) {
  std::vector<Nodo*> A, C; // Lista de nodos abiertos (A) y cerrados (C)
  std::vector<Nodo*> nodos_generados, nodos_inspeccionados;
  std::vector<Casilla*> casillas_generadas;
  std::string iteraciones = "";
  int vertice = 2, iteracion = 1;

  // Inicializar el nodo inicial (raíz) con g(n) = 0 y f(n) = h(n)
  Nodo* nodo_actual = raiz_;
  nodo_actual->SetValorG(0); // g(n) inicial es 0
  nodos_generados.emplace_back(nodo_actual);
  nodo_actual->SetValorH(FuncionHManhattan(nodo_actual->GetCasilla(), laberinto_.GetSalida())); // h(n) inicial
  nodo_actual->SetValorF(nodo_actual->GetValorG() + nodo_actual->GetValorH()); // f(n) = g(n) + h(n)
  
  A.push_back(nodo_actual); // Agregar el nodo inicial a la lista abierta
  bool encontrado = false;

  while (!A.empty()) {
    ImprimirSolucion(iteraciones, nodos_generados, nodos_inspeccionados, iteracion);
    // Seleccionar el nodo de menor coste f(n) en la lista A
    std::sort(A.begin(), A.end(), [](const Nodo* a, const Nodo* b) { return a->GetValorF() < b->GetValorF(); });
    for (auto nodo : A) {
      std::cout << nodo->GetCasilla()->GetCoordenada() << " ";
    }
    std::cout << std::endl;
    nodo_actual = A.front();
    A.erase(A.begin()); // Quitarlo de A
    C.push_back(nodo_actual); // Agregarlo a la lista de cerrados C
    nodos_inspeccionados.emplace_back(nodo_actual);

    // Si se llega a la salida
    if (nodo_actual->GetCasilla()->GetTipo() == 4) { // Tipo 4 es la salida
      encontrado = true;
      break;
    }
    std::cout << "Nodo actual: " << nodo_actual->GetCasilla()->GetCoordenada() << " " << nodo_actual->GetValorF() << " " << nodo_actual->GetValorG() << " " << nodo_actual->GetValorH() << std::endl;
    // Obtener nodos adyacentes
    std::vector<Casilla*> adyacentes = laberinto_.GetCasillasAdyacentes(nodo_actual->GetCasilla());
    for (auto adyacente : adyacentes) {
      Nodo* nodo_adyacente;
      // Crear nodo si no existe
      if (adyacente->GetNodo() != nullptr) {
        nodo_adyacente = adyacente->GetNodo();
      } else {
        nodo_adyacente = new Nodo(vertice, adyacente);
        nodos_generados.emplace_back(nodo_adyacente);
        vertice++;
        casillas_generadas.emplace_back(adyacente);
        adyacente->SetNodo(nodo_adyacente);
      }
      // Si el nodo no está en C (lista cerrada)
      if (std::find(C.begin(), C.end(), nodo_adyacente) == C.end()) {
        Nodo* padre_original = nodo_adyacente->GetPadre();
        nodo_adyacente->SetNodoPadre(nodo_actual);
        int nuevo_g = FuncionG(nodo_adyacente);
        auto it = std::find(A.begin(), A.end(), nodo_adyacente);
        if (it == A.end()) {
          // Si el nodo no está en A (lista abierta), añadirlo
          nodo_adyacente->SetValorG(nuevo_g);
          nodo_adyacente->SetValorH(FuncionHManhattan(adyacente, laberinto_.GetSalida()));
          nodo_adyacente->SetValorF(nuevo_g + nodo_adyacente->GetValorH());
          A.push_back(nodo_adyacente);
        } else if (nuevo_g < nodo_adyacente->GetValorG()) {
          // Si el nodo ya está en A y encontramos un camino mejor, actualizar g(n) y f(n)
          nodo_adyacente->SetValorG(nuevo_g);
          nodo_adyacente->SetValorF(nuevo_g + nodo_adyacente->GetValorH());
        } else if (nuevo_g >= nodo_adyacente->GetValorG()) {
          nodo_adyacente->SetNodoPadre(padre_original);
        }
        std::cout << "Revisado: " << nodo_adyacente->GetCasilla()->GetCoordenada() << "f(n)= " << nodo_adyacente->GetValorF() << " g(n)= " << nodo_adyacente->GetValorG() << " h(n)= " << nodo_adyacente->GetValorH() << std::endl; 
      }
    }
    iteracion++;
  }
  
  if (!encontrado) {
    std::cout << "No se ha encontrado un camino" << std::endl;
  } else {
    std::cout << "Camino encontrado" << std::endl;
    std::vector<Nodo*> camino;
    int numero_generados = nodos_generados.size(), numero_inspeccionados = nodos_inspeccionados.size();
    GenerarCamino(laberinto_.GetSalida()->GetNodo(), fichero, camino, numero_generados, numero_inspeccionados); // Generar el camino a la salida
  }
  fichero << iteraciones << std::endl;

  // Liberar memoria de los nodos creados dinámicamente
  for (auto casilla : casillas_generadas) {
    if (casilla->GetNodo() != nullptr) casilla->SetNodo(nullptr);
  }
  for (auto& nodo : nodos_generados) {
    if (nodo) delete nodo;
  }
}


int Grafo::FuncionHManhattan(Casilla* actual, Casilla* destino) {
  const int w = 3;
  return (abs(actual->GetCoordenada().GetX() - destino->GetCoordenada().GetX()) + 
        abs(actual->GetCoordenada().GetY() - destino->GetCoordenada().GetY())) * w;
}

int Grafo::FuncionG(Nodo* nodo_actual) {
  // Si no tiene un padre, entonces es la raíz, g(n) = 0
  if (nodo_actual->GetPadre() == nullptr) return 0;

  bool esDiagonal = (nodo_actual->GetPadre()->GetCasilla()->GetCoordenada().GetX() != nodo_actual->GetCasilla()->GetCoordenada().GetX()) && 
                    (nodo_actual->GetPadre()->GetCasilla()->GetCoordenada().GetY() != nodo_actual->GetCasilla()->GetCoordenada().GetY());
  int costo_movimiento = esDiagonal ? 7 : 5;

  return nodo_actual->GetPadre()->GetValorG() + costo_movimiento;
}

int Grafo::FuncionF(int funcion_g, int funcion_h) {
  return funcion_g + funcion_h;
}

/**
* @brief Método que imprime cada iteración de la búsuqeda seleccionada
* @param fichero Fichero de salida en el que se imprimirá la solución
* @param nodos_generados 
* @param nodos_inspeccionados
* @param iteracion
*/
void Grafo::ImprimirSolucion(std::string& iteraciones, const std::vector<Nodo*>& nodos_generados, 
            const std::vector<Nodo*>& nodos_inspeccionados, const int& iteracion) {
  iteraciones += "Iteración ";
  iteraciones += std::to_string(iteracion); // Convertir iteracion a string
  iteraciones += '\n';
  iteraciones += "Nodos generados: ";
  int it = 1;
  for (auto nodo : nodos_generados) {
    iteraciones += std::to_string(nodo->GetNumero()); // Convertir el número del nodo a string
    if (it < nodos_generados.size()) iteraciones += ", ";
    it++;
  }
  iteraciones += '\n';
  iteraciones += "Nodos inspeccionados: ";
  if (nodos_inspeccionados.empty()) {
    iteraciones += "-";
  } else {
    it = 1;
    for (auto nodo : nodos_inspeccionados) {
      iteraciones += std::to_string(nodo->GetNumero()); // Convertir el número del nodo a string
      if (it < nodos_inspeccionados.size()) iteraciones += ", ";
      it++;
    }
  }
  iteraciones += '\n';
  iteraciones += "---------------------------------------------";
  iteraciones += '\n';
}

/**
 * @brief Método que revisa todos los nodos de una rama sean distintos del nodo a insertar
 * @param nodo_insertar
 * @param nodo_actual
 * @return true si no hay otro nodo igual en la rama, false si sí
 */
bool Grafo::RevisarRama(Nodo* nodo_insertar, Nodo* nodo_actual) {
  Nodo* nodo_padre = nodo_actual->GetPadre();
  while (nodo_padre != nullptr) {
    if (nodo_padre->GetNumero() == nodo_insertar->GetNumero()) return false;  
    nodo_padre = nodo_padre->GetPadre();
  }
  return true; // Se llegó hasta la raíz sin encontrar un nodo igual
}

/**
 * @brief Método que recorre desde el nodo destino los padres para generar el camino hasta el nodo origen
 * @param nodo_actual Nodo destino
 * @param fichero Fichero de salida
 * @param camino Camino generado
 */
void Grafo::GenerarCamino(Nodo* nodo_actual, std::ofstream& fichero, std::vector<Nodo*>& camino, int numero_generados, int numero_inspeccionados) {
  Nodo* nodo_padre = nodo_actual->GetPadre();
  camino.emplace_back(nodo_actual);
  while (nodo_padre != nullptr) {
    camino.emplace_back(nodo_padre);
    nodo_padre = nodo_padre->GetPadre();
  }
  std::reverse(camino.begin(), camino.end());
  for (auto nodo : camino) {
    fichero << nodo->GetVertice() << " ";
  }
  fichero << "  ";
  fichero << nodo_actual->GetValorF() << "        " << numero_generados << "                          " << numero_inspeccionados << std::endl;

  // Obtener una copia de las casillas del laberinto
  auto casillas = laberinto_.GetCasillas();

  // Marcar el camino en la copia del laberinto, excepto la entrada y la salida
  for (auto& nodo : camino) {
    auto casilla = nodo->GetCasilla();
    if (casilla != laberinto_.GetEntrada() && casilla != laberinto_.GetSalida()) {
      casillas[casilla->GetCoordenada().GetX()][casilla->GetCoordenada().GetY()].SetTipo(-1); // Usamos -1 para marcar el camino
    }
  }

  // Imprimir el laberinto marcado en el fichero
  for (auto& fila : casillas) {
    for (auto& casilla : fila) {
      if (casilla.GetTipo() == -1) {
        fichero << "* "; // Camino
      } else if (casilla.GetTipo() == 0) {
        fichero << "  "; // Transitable
      } else if (casilla.GetTipo() == 1) {
        fichero << "■ "; // No transitable
      } else if (casilla.GetTipo() == 3) {
        fichero << "S "; // Entrada
      } else if (casilla.GetTipo() == 4) {
        fichero << "E "; // Salida
      }
    }
    fichero << std::endl;
  }
}
