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

#include "grafo.h"

struct NodoHash {
  std::size_t operator()(const Nodo* nodo) const {
    return std::hash<int>()(nodo->GetNumero()); 
  }
};

struct NodoEqual {
  bool operator()(const Nodo* lhs, const Nodo* rhs) const {
    return lhs->GetNumero() == rhs->GetNumero();
  }
};
/**
 * @brief Método que implementa la búsqueda A*
 * @param fichero Fichero de salida en el que se imprimirá la solución
*/
void Grafo::BusquedaA(const std::string& nombre_fichero, const std::string& instancia) {
  struct ComparadorNodo {
    bool operator()(const Nodo* a, const Nodo* b) const {
      return a->GetValorF() > b->GetValorF();
    }
  };
  std::ofstream fichero(nombre_fichero);
  std::vector<Nodo*> A;
  std::unordered_set<Nodo*> C;
  std::vector<Nodo*> nodos_generados, nodos_inspeccionados;
  std::vector<Casilla*> casillas_generadas; // Para desvincular los nodos luego
  std::string iteraciones = "";
  int vertice = 2, iteracion = 1;

  fichero << "-----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  fichero << "Instancia      n    m    S   E                  Camino         Coste  Número de nodos generados   Número de nodos inspeccionados" << std::endl;
  fichero << "-----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  fichero << "    " << instancia << "    " << GetNumeroFilas() << "   " << GetNumeroColumnas() << "  " << GetLaberinto().GetEntrada()->GetCoordenada();
  fichero << "  " << GetLaberinto().GetSalida()->GetCoordenada() << "  ";

  // Inicializar el nodo inicial (raíz) con g(n) = 0 y f(n) = h(n)
  Nodo* nodo_actual = raiz_;
  nodo_actual->SetValorG(0); // g(n) inicial es 0
  nodo_actual->SetValorH(FuncionHManhattan(nodo_actual->GetCasilla(), laberinto_.GetSalida())); // h(n) inicial
  nodo_actual->SetValorF(nodo_actual->GetValorG() + nodo_actual->GetValorH()); // f(n) = g(n) + h(n)
  nodos_generados.emplace_back(nodo_actual);
  
  A.push_back(nodo_actual); // Agregar el nodo inicial a la lista abierta
  bool encontrado = false;

  while (!A.empty() && !encontrado) {
    ImprimirSolucion(iteraciones, nodos_generados, nodos_inspeccionados, iteracion);
    std::sort(A.begin(), A.end(), [](Nodo* a, Nodo* b) {
      return a->GetValorF() < b->GetValorF();
    });
    // Metemos los tres primeros
    if (A.size() >= 3) {
      std::srand(std::time(0));
      std::vector<Nodo*> tres_primeros(A.begin(), A.begin() + 3);
      int elegido = std::rand() % 3;
      nodo_actual = tres_primeros[elegido]; // Cogemos el nodo con menor f(n)
      A.erase(A.begin() + elegido);
    } else if (A.size() == 2) {
      std::srand(std::time(0));
      std::vector<Nodo*> tres_primeros(A.begin(), A.begin() + 2);
      int elegido = std::rand() % 2;
      nodo_actual = tres_primeros[elegido]; // Cogemos el nodo con menor f(n)    
      A.erase(A.begin() + elegido);    
    } else {
      nodo_actual = A.front();
      A.erase(A.begin());
    }
    C.insert(nodo_actual); // Agregarlo a la lista de cerrados C

    // Si se llega a la salida
    if (nodo_actual->GetCasilla()->GetTipo() == 4) { 
      encontrado = true;
      break;
    }
    nodos_inspeccionados.emplace_back(nodo_actual);
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
      if (C.find(nodo_adyacente) == C.end()) {
        Nodo* padre_original = nodo_adyacente->GetPadre();
        nodo_adyacente->SetNodoPadre(nodo_actual);
        auto it = std::find(A.begin(), A.end(), nodo_adyacente);
        if (it == A.end()) {
          // Si el nodo no está en A (lista abierta), añadirlo
          nodo_adyacente->SetValorG(FuncionG(nodo_adyacente));
          nodo_adyacente->SetValorH(FuncionHManhattan(adyacente, laberinto_.GetSalida()));
          nodo_adyacente->SetValorF(nodo_adyacente->GetValorG() + nodo_adyacente->GetValorH());
          A.emplace_back(nodo_adyacente);
        } else { 
          int nuevo_g = FuncionG(nodo_adyacente);
          if (nuevo_g < nodo_adyacente->GetValorG()) {
            nodo_adyacente->SetValorG(nuevo_g);
            nodo_adyacente->SetValorF(nodo_adyacente->GetValorG() + nodo_adyacente->GetValorH());
          } else {
            nodo_adyacente->SetNodoPadre(padre_original);
          }
        }
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
    if (nodo && nodo != raiz_) delete nodo;
  }
  fichero.close();
}

/**
 * @brief Método que implementa la función heurística basada en la distancia de Manhattan
 * @param actual La casilla actual
 * @param destino La casilla de destino
 * @return El valor de la función heurística
*/
int Grafo::FuncionHManhattan(Casilla* actual, Casilla* destino) {
  const int w = 3;
  return (abs(actual->GetCoordenada().GetX() - destino->GetCoordenada().GetX()) + 
        abs(actual->GetCoordenada().GetY() - destino->GetCoordenada().GetY())) * w;
}

/**
 * @brief Método que implementa una función heurística variante de la distancia de Chebyshev. Calcula dx y dy para luego usar el mayor de estos valores para estimar el costo.
 *        Se ha ajustado para reflehar la diferencia de costo entre ir recto e ir en diagonal
 * @param actual Casilla actual
 * @param destino Casilla destino
 * @return El valor de la función heurística
*/
int Grafo::FuncionHAdaptada(Casilla* actual, Casilla* destino) {
  int dx = abs(actual->GetCoordenada().GetX() - destino->GetCoordenada().GetX());
  int dy = abs(actual->GetCoordenada().GetY() - destino->GetCoordenada().GetY());
  int movimientosDiagonales = std::min(dx, dy);
  int movimientosRectos = std::max(dx, dy) - movimientosDiagonales;
  return movimientosDiagonales * 7 + movimientosRectos * 5;
}

/**
 * @brief Método que implementa una función heurística realiza una combinación ponderada la distancia de Manhattan y la distancia euclidiana
 *        La idea es ponderar la distancia de Manhattan para reflejar el costo de los movimientos rectos y agregar una pequeña proporción de la distancia euclidiana
 *        para ajustar el costo adicional de los movimientos diagonales
 * @param actual Casilla actual
 * @param destino Casilla destino
 * @return El valor de la función heurística
*/
int Grafo::FuncionHCombinada(Casilla* actual, Casilla* destino) {
  int dx = std::abs(actual->GetCoordenada().GetX() - destino->GetCoordenada().GetX());
  int dy = std::abs(actual->GetCoordenada().GetY() - destino->GetCoordenada().GetY());
  int dManhattan = (dx + dy) * 5; // Costo de movimiento recto
  double dEuclidiana = std::sqrt(dx*dx + dy*dy);
  
  // Estimación del componente diagonal
  double componenteDiagonal = std::max(0.0, dEuclidiana - std::max(dx, dy)) * (7 - 5);
  
  return dManhattan + static_cast<int>(componenteDiagonal);
}

/**
 * @brief Método que implementa la función G, el costo acumulado
 * @param nodo_actual 
 * @return El costo acumulado desde la raíz
*/
int Grafo::FuncionG(Nodo* nodo_actual) {
  int costo = 0;
  Nodo* nodo_iter = nodo_actual;

  while (nodo_iter->GetPadre() != nullptr) {
    Nodo* nodo_padre = nodo_iter->GetPadre();
    int dx = std::abs(nodo_iter->GetCasilla()->GetCoordenada().GetX() - nodo_padre->GetCasilla()->GetCoordenada().GetX());
    int dy = std::abs(nodo_iter->GetCasilla()->GetCoordenada().GetY() - nodo_padre->GetCasilla()->GetCoordenada().GetY());

    if (dx + dy == 1) costo += 5;
    else if (dx == 1 && dy == 1) costo += 7;

    nodo_iter = nodo_padre;
  }
  return costo;
}

/**
* @brief Método que imprime cada iteración de la búsuqeda
* @param iteraciones String que almacena las iteraciones realizadas en la búsqueda
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
 * @brief Método que recorre desde el nodo destino los padres para generar el camino hasta el nodo origen e imprime la solución del laberinto
 * @param nodo_actual Nodo destino
 * @param fichero Fichero de salida
 * @param camino Camino generado
 * @param numero_generados Número de nodos generados
 * @param numero_inspeccionados Número de nodos inspeccionados
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
