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

void Grafo::BusquedaA() {
  std::ofstream fichero("saves/solucion.txt");
  if (!fichero.is_open()) {
    std::cerr << "Error al abrir el archivo de solución." << std::endl;
    return;
  }
  std::vector<Nodo*> A, C;
  int vertice = 2, iteracion = 1;

  Nodo* nodo_actual = raiz_;
  raiz_->SetValorF(FuncionHManhattan(nodo_actual->GetCasilla(), laberinto_.GetSalida()));
  A.push_back(nodo_actual);
  bool encontrado = false;

  while (!A.empty()) {
      nodo_actual = A.front();
      std::cout << nodo_actual->GetCasilla()->GetCoordenada() << std::endl;
      A.erase(A.begin());
      C.push_back(nodo_actual);

      if (nodo_actual->GetCasilla()->GetTipo() == 4) { // Se ha llegado a la salida
        encontrado = true;
        std::vector<Nodo*> camino;
        GenerarCamino(nodo_actual, fichero, camino);
        break;
      } else {
        std::vector<Casilla*> adyacentes = laberinto_.GetCasillasAdyacentes(nodo_actual->GetCasilla());
        for (auto adyacente : adyacentes) {
          Nodo* nodo_adyacente;
          if (adyacente->GetNodo() != nullptr) {
            nodo_adyacente = adyacente->GetNodo();
          } else {
            nodo_adyacente = new Nodo(vertice, adyacente);
            vertice++;
            adyacente->SetNodo(nodo_adyacente);
          }
          auto it = std::find_if(A.begin(), A.end(), [nodo_adyacente](const Nodo* n) { return *n == *nodo_adyacente; });
          if (it == A.end() && std::find(C.begin(), C.end(), nodo_adyacente) == C.end()) {
            nodo_adyacente->SetNodoPadre(nodo_actual);
            //nodo_adyacente->SetCostoAcumulado(FuncionG(nodo_adyacente));
            nodo_adyacente->SetValorF(FuncionHManhattan(adyacente, laberinto_.GetSalida()) + FuncionG(nodo_adyacente));
            A.push_back(nodo_adyacente);
          } else if (it != A.end()) {
            Nodo* nodo_padre_original = nodo_adyacente->GetPadre();
            std::cout << "Padre original " << nodo_adyacente->GetPadre() << std::endl;
            nodo_adyacente->SetNodoPadre(nodo_actual);
            std::cout << "Padre temporal " << nodo_adyacente->GetPadre() << std::endl;
            int nuevoValorF = FuncionHManhattan(adyacente, laberinto_.GetSalida()) + FuncionG(nodo_adyacente);
            std::cout << nodo_actual->GetCasilla()->GetCoordenada() << " "; 
            std::cout << nodo_adyacente->GetValorF() << " " << FuncionF(FuncionHManhattan(adyacente, laberinto_.GetSalida()), FuncionG(nodo_adyacente)) << std::endl;
            if (nuevoValorF < nodo_adyacente->GetValorF()) {
              nodo_adyacente->SetValorF(nuevoValorF);
              nodo_adyacente->SetNodoPadre(nodo_actual);
            } else {
              nodo_adyacente->SetNodoPadre(nodo_padre_original);
            }
          }
          // std::cout << adyacente->GetCoordenada() << " " << nodo_adyacente->GetValorF() << std::endl;
        }
      }
      iteracion++;
      std::sort(A.begin(), A.end(), [](const Nodo* a, const Nodo* b) { return a->GetValorF() < b->GetValorF(); });
      for (auto nodo : A) {
        std::cout << nodo->GetCasilla()->GetCoordenada() << " " << nodo->GetValorF();
      }
      std::cout << std::endl;
  }
  if (!encontrado) std::cout << "No se ha encontrado un camino" << std::endl;
  fichero.close();

  // Liberar memoria de los nodos creados dinámicamente
  for (auto& nodo : C) {
    if (nodo) delete nodo;
  }
}

int Grafo::FuncionHManhattan(Casilla* actual, Casilla* destino) {
  const int w = 3;
  return (abs(actual->GetCoordenada().GetX() - destino->GetCoordenada().GetX()) + 
        abs(actual->GetCoordenada().GetY() - destino->GetCoordenada().GetY())) * w;
}

int Grafo::FuncionG(Nodo* nodo_actual) {
  // Caso base: Si no hay padre, el costo es 0
  if (nodo_actual->GetPadre() == nullptr) return 0;
  bool esDiagonal = (nodo_actual->GetPadre()->GetCasilla()->GetCoordenada().GetX() != nodo_actual->GetCasilla()->GetCoordenada().GetX()) && 
                    (nodo_actual->GetPadre()->GetCasilla()->GetCoordenada().GetY() != nodo_actual->GetCasilla()->GetCoordenada().GetY());
  int costo_movimiento = esDiagonal ? 7 : 5;

  // Llamada recursiva para calcular el costo desde el inicio hasta el padre
  int costo_hasta_padre = FuncionG(nodo_actual->GetPadre());

  return costo_movimiento + costo_hasta_padre;
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
void Grafo::ImprimirSolucion(std::ofstream& fichero, const std::vector<Nodo*>& nodos_generados, 
            const std::vector<Nodo*>& nodos_inspeccionados, const int& iteracion) {
  fichero << "Iteración " << iteracion << std::endl;
  fichero << "Nodos generados: ";
  int it = 1;
  for (auto nodo : nodos_generados) {
    fichero << nodo->GetNumero();
    if (it < nodos_generados.size()) fichero << ", ";
    it++;
  }
  fichero << std::endl << "Nodos inspeccionados: ";
  if (nodos_inspeccionados.empty()) {
    fichero << "-";
  } else {
    it = 1;
    for (auto nodo : nodos_inspeccionados) {
      fichero << nodo->GetNumero();
      if (it < nodos_inspeccionados.size()) fichero << ", ";
      it++;
    }
  }
  fichero << std::endl << "---------------------------------------------" << std::endl;
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
 * @brief Método que recorre el camino y genera su coste
 * @param camino Camino generado desde el nodo origen al nodo destino
 * @param costo Costo total 
 */
// void Grafo::GenerarCoste(const std::vector<Nodo*>& camino, int& costo) {
//   costo = 0;
//   for (int i = 0; i < camino.size(); i++) {
//     int j = i + 1;
//     if (j == camino.size()) break;
//     auto it = distancias_.find(std::make_pair(camino[i]->GetNumero(), camino[j]->GetNumero()));
//     costo += it->second;
//   }
// }

/**
 * @brief Método que recorre desde el nodo destino los padres para generar el camino hasta el nodo origen
 * @param nodo_actual Nodo destino
 * @param fichero Fichero de salida
 * @param camino Camino generado
 */
void Grafo::GenerarCamino(Nodo* nodo_actual, std::ofstream& fichero, std::vector<Nodo*>& camino) {
  Nodo* nodo_padre = nodo_actual->GetPadre();
  camino.emplace_back(nodo_actual);
  while (nodo_padre != nullptr) {
    camino.emplace_back(nodo_padre);
    nodo_padre = nodo_padre->GetPadre();
  }
  std::reverse(camino.begin(), camino.end());

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
        fichero << "0 "; // Transitable
      } else if (casilla.GetTipo() == 1) {
        fichero << "1 "; // No transitable
      } else if (casilla.GetTipo() == 3) {
        fichero << "E "; // Entrada
      } else if (casilla.GetTipo() == 4) {
        fichero << "S "; // Salida
      }
    }
    fichero << std::endl;
  }
}
