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

/**
 * @brief Método que implementa la búsqueda A*
 * @param fichero Fichero de salida en el que se imprimirá la solución
*/

#include "grafo.h"
#include <queue>
#include <unordered_set>

struct CompareNodo {
    bool operator()(Nodo* const& n1, Nodo* const& n2) {
        return n1->GetValorF() > n2->GetValorF();
    }
};

#include "grafo.h"
#include <queue>
#include <unordered_set>

struct ComparaNodo {
    bool operator()(Nodo* const& n1, Nodo* const& n2) {
        return n1->GetValorF() > n2->GetValorF();
    }
};

void Grafo::BusquedaA() {
  std::ofstream fichero("saves/solucion.txt");
  Nodo* nodo_actual = raiz_;
  std::vector<Nodo*> generados, inspeccionados;
  std::priority_queue<Nodo*, std::vector<Nodo*>, ComparaNodo> abiertos;
  std::unordered_set<Nodo*> cerrados;

  abiertos.push(nodo_actual);
  int iteracion = 0;
  raiz_->SetValorF(FuncionHManhattan(raiz_->GetCasilla(), laberinto_.GetSalida()));
  int vertice = 1;

  while (!abiertos.empty()) {
    if (iteracion == 2) break;
    nodo_actual = abiertos.top();
    abiertos.pop();
    cerrados.insert(nodo_actual);
    generados.push_back(nodo_actual);

    if (nodo_actual->GetCasilla()->GetTipo() == 4) {
      std::vector<Nodo*> camino;
      GenerarCamino(nodo_actual, fichero, camino);
      fichero << std::endl << "Coste total: " << nodo_actual->GetCostoAcumulado() << std::endl;
      fichero.close();
      return;
    }
    // Comprobaciones
    std::cout << "Coordenada nodo actual: " << nodo_actual->GetCasilla()->GetCoordenada() << std::endl;
    std::cout << "Adyacentes: " << std::endl;
    auto adyacentes = laberinto_.GetCasillasAdyacentes(nodo_actual->GetCasilla());
    for (auto adyacente : adyacentes) {
      vertice++;
      std::cout << adyacente->GetCoordenada() << std::endl;
      Nodo* nodo_adyacente = new Nodo(vertice, adyacente);
      nodo_adyacente->SetNodoPadre(nodo_actual);
      int valor_g = FuncionG(nodo_adyacente);
      nodo_adyacente->SetCostoAcumulado(valor_g);
      nodo_adyacente->SetValorF(valor_g + FuncionHManhattan(adyacente, laberinto_.GetSalida()));
      std::cout << nodo_adyacente->GetValorF() << std::endl;

      if (cerrados.find(nodo_adyacente) == cerrados.end()) {
        abiertos.push(nodo_adyacente);
      }
    }
    inspeccionados.push_back(nodo_actual);
    iteracion++;
  }
}

int Grafo::FuncionHManhattan(Casilla* actual, Casilla* destino) {
  const int w = 3;
  return (abs(actual->GetCoordenada().GetX() - destino->GetCoordenada().GetX()) + 
        abs(actual->GetCoordenada().GetY() - destino->GetCoordenada().GetY())) * w;
}

int Grafo::FuncionG(Nodo* nodo_actual) {
  Nodo* nodo_padre = nodo_actual->GetPadre();
  int g = (nodo_padre->GetCasilla()->GetCoordenada().GetX() == nodo_actual->GetCasilla()->GetCoordenada().GetX() || 
        nodo_padre->GetCasilla()->GetCoordenada().GetY() == nodo_actual->GetCasilla()->GetCoordenada().GetY() ? 5 : 7);
  while (nodo_padre != nullptr) {
    g += nodo_padre->GetCostoAcumulado();
    nodo_padre = nodo_padre->GetPadre();
  }
  return g;
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
      casillas[casilla->GetCoordenada().GetY()][casilla->GetCoordenada().GetX()].SetTipo(-1); // Usamos -1 para marcar el camino
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
