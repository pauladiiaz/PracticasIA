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

#include "arbol.h"

/**
 * @brief Constructor de la clase Grafo. Lee las distancias entre los nodos y las guarda en un mapa con claves nodo-nodo
 * @param raiz Nodo raiz del árbol de búsqueda
 * @param nombre_fichero Nombre del fichero del que se van a leer los nodos sucesores
 * @param origen Número del nodo origen
 * @param destino Número del nodo destino
 */
Grafo::Grafo(Nodo* raiz, const std::string& nombre_fichero, const int& origen, const int& destino) : raiz_(raiz), origen_(origen), 
        destino_(destino) {
  // Leer los datos desde el fichero
  std::ifstream fichero(nombre_fichero);
  if (!fichero.is_open()) {
    throw std::runtime_error("Error al abrir el fichero.");
  }
  std::string linea;
  getline(fichero, linea);
  numero_vertices_ = std::stoi(linea);
  numero_aristas_ = 0;
  for (int i = 1; i <= numero_vertices_; ++i) {
    for (int j = i + 1; j <= numero_vertices_; ++j) { // Como las distancias son simétricas, se empieza un vértice por delante
      if (getline(fichero, linea)) {
        if (std::stof(linea) != -1) {
          distancias_.insert(std::make_pair(std::make_pair(i, j), std::stof(linea))); // tener en cuenta la simetría
          distancias_.insert(std::make_pair(std::make_pair(j, i), std::stof(linea)));  
          numero_aristas_++;
        }
      } else {
        throw std::runtime_error("Error al leer el fichero.");
      }
    }
  }
  
}

void Grafo::BusquedaAmplitudModificada(std::ofstream& fichero) {
  int iteracion = 1, costo = 0;
  std::queue<Nodo*> cola;
  std::vector<Nodo*> inspeccionados, generados;

  Nodo* nodo_actual = nullptr;
  generados.emplace_back(raiz_);
  ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
  iteracion++;
  std::vector<Nodo*> sucesores;
  for (int i = 1; i <= numero_vertices_; ++i) {
    if (i != raiz_->GetNumero()) {
      auto it = distancias_.find(std::make_pair(i, raiz_->GetNumero()));
      if (it != distancias_.end()) {
        Nodo* nuevo_sucesor = new Nodo(i);
        generados.emplace_back(nuevo_sucesor);
        nuevo_sucesor->SetNodoPadre(raiz_);
        sucesores.emplace_back(nuevo_sucesor);
      }
    }
  }
  inspeccionados.emplace_back(raiz_);
  srand(time(0));
  int j;
  for (j = 1; j <= 10; ++j) {
    cola.push(sucesores[rand() % sucesores.size()]);
    bool camino_encontrado = false;
    while (!cola.empty()) {
      nodo_actual = cola.front();
      cola.pop();

      ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
      if (nodo_actual->GetNumero() == destino_) {
        camino_encontrado = true;
        iteracion++;
        inspeccionados.emplace_back(nodo_actual);
        ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
        break;
      }
      for (int i = 1; i <= numero_vertices_; ++i) {
        if (i != nodo_actual->GetNumero()) {
          auto it = distancias_.find(std::make_pair(i, nodo_actual->GetNumero()));
          if (it != distancias_.end()) {
            Nodo* nuevo_nodo = new Nodo(i);
            if (RevisarRama(nuevo_nodo, nodo_actual)) {
              generados.emplace_back(nuevo_nodo);
              nuevo_nodo->SetNodoPadre(nodo_actual);
              cola.push(nuevo_nodo);
            }          
          }
        }
      }
      inspeccionados.emplace_back(nodo_actual);
      iteracion++;      
    }
    if (camino_encontrado == true) break;
  }
  if (j > 10) {
    fichero << "No se ha encontrado un camino" << std::endl;
  } else {
    std::vector<Nodo*> camino;
    fichero << "Camino: ";
    GenerarCamino(nodo_actual, fichero, camino);
    fichero << std::endl;

    GenerarCoste(camino, costo);
    fichero << "Costo: " << costo << std::endl; 
  }
}

/**
 * @brief Método que implementa la búsqueda en amplitud e imprime el camino del nodo origen al destino y el costo total
 * @param fichero Fichero de salida en el que se imprimirá la solución
 */
void Grafo::BusquedaAmplitud(std::ofstream& fichero) {
  int iteracion = 1, costo = 0;
  std::queue<Nodo*> cola;
  std::vector<Nodo*> inspeccionados, generados;

  Nodo* nodo_actual = nullptr;
  cola.push(raiz_);
  generados.emplace_back(raiz_);
  while (true) {
    nodo_actual = cola.front();
    cola.pop();

    ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
    if (nodo_actual->GetNumero() == destino_) { // Si no hay que mostrarlo solo dejo que se interrumpa el bucle
      iteracion++;
      inspeccionados.emplace_back(nodo_actual);
      ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
      break;
    }

    for (int i = 1; i <= numero_vertices_; ++i) {
      if (i != nodo_actual->GetNumero()) {
        auto it = distancias_.find(std::make_pair(i, nodo_actual->GetNumero()));
        if (it != distancias_.end()) {
          Nodo* nuevo_nodo = new Nodo(i);
          if (RevisarRama(nuevo_nodo, nodo_actual)) {
            generados.emplace_back(nuevo_nodo);
            nuevo_nodo->SetNodoPadre(nodo_actual);
            cola.push(nuevo_nodo);
          }          
        }
      }
    }
    inspeccionados.emplace_back(nodo_actual);
    iteracion++;
  }
  std::vector<Nodo*> camino;
  fichero << "Camino: ";
  GenerarCamino(nodo_actual, fichero, camino);
  fichero << std::endl;

  GenerarCoste(camino, costo);
  fichero << "Costo: " << costo << std::endl;
}



/**
 * @brief Método que implementa la búsqueda en profundidad e imprime el camino del nodo origen al destino y el costo total
 * @param fichero Fichero de salida en el que se imprimirá la solución
 */
void Grafo::BusquedaProfundidad(std::ofstream& fichero) {
  int iteracion = 1, costo = 0;
  std::stack<Nodo*> pila;
  std::vector<Nodo*> inspeccionados, generados;

  Nodo* nodo_actual = nullptr;
  pila.push(raiz_);
  generados.emplace_back(raiz_);
  while (true) {
    nodo_actual = pila.top();
    pila.pop();

    ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
    if (nodo_actual->GetNumero() == destino_) { // Si no hay que mostrarlo solo dejo que se interrumpa el bucle
      iteracion++;
      inspeccionados.emplace_back(nodo_actual);
      ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
      break;
    }

    std::vector<Nodo*> sucesores;
    for (int i = 1; i <= numero_vertices_; ++i) {
      if (i != nodo_actual->GetNumero()) {
        auto it = distancias_.find(std::make_pair(i, nodo_actual->GetNumero()));
        if (it != distancias_.end()) {
          Nodo* nuevo_nodo = new Nodo(i);
          if (RevisarRama(nuevo_nodo, nodo_actual)) {
            generados.emplace_back(nuevo_nodo);
            nuevo_nodo->SetNodoPadre(nodo_actual);
            sucesores.emplace_back(nuevo_nodo);
          }          
        }
      }
    }
    std::reverse(sucesores.begin(), sucesores.end());
    for (int i = 0; i < sucesores.size(); ++i) {
      pila.push(sucesores[i]);
    }
    inspeccionados.emplace_back(nodo_actual);
    iteracion++;
  }
  std::vector<Nodo*> camino;
  fichero << "Camino: ";
  GenerarCamino(nodo_actual, fichero, camino);
  fichero << std::endl;

  GenerarCoste(camino, costo);
  fichero << "Costo: " << costo << std::endl;
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
void Grafo::GenerarCoste(const std::vector<Nodo*>& camino, int& costo) {
  costo = 0;
  for (int i = 0; i < camino.size(); i++) {
    int j = i + 1;
    if (j == camino.size()) break;
    auto it = distancias_.find(std::make_pair(camino[i]->GetNumero(), camino[j]->GetNumero()));
    costo += it->second;
  }
}

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
  fichero << camino[0]->GetNumero();
  for (int i = 1; i < camino.size(); i++) {
    fichero << ", " << camino[i]->GetNumero();
  }
}
