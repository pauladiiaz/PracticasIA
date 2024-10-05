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

void Grafo::BusquedaAmplitud(std::ofstream& fichero) {
  int iteracion = 1, costo = 0;
  std::queue<Nodo*> cola;
  std::vector<Nodo*> inspeccionados, generados;
  std::unordered_set<int> nodos_generados;
  nodos_generados.insert(raiz_->GetNumero());

  Nodo* nodo_actual = nullptr;
  cola.push(raiz_);
  generados.emplace_back(raiz_);

  while (true) {
    ImprimirSolucion(fichero, generados, inspeccionados, iteracion);

    nodo_actual = cola.front();
    cola.pop();

    for (int i = 1; i <= numero_vertices_; ++i) {
      if (i != nodo_actual->GetNumero()) {
        auto it = distancias_.find(std::make_pair(i, nodo_actual->GetNumero()));

        if (it != distancias_.end()) {
          if (nodos_generados.find(i) == nodos_generados.end()) {
            Nodo* nuevo_nodo = new Nodo(i);
            generados.emplace_back(nuevo_nodo);
            cola.push(nuevo_nodo);
            nuevo_nodo->SetNodoPadre(nodo_actual);
            nodos_generados.insert(i);
          } else {
            int k;
            for (k = 0; k < generados.size(); ++k) {
              if (generados[k]->GetNumero() == i) break;
            }
            Nodo* nodo_sucesor = generados[k];
            auto it2 = std::find(inspeccionados.begin(), inspeccionados.end(), nodo_sucesor);
            // if (RevisarRama(nodo_sucesor, nodo_actual) && it2 == inspeccionados.end()) {
            if (RevisarRama(nodo_sucesor, nodo_actual) && it2 == inspeccionados.end()) {
              // generados.emplace_back(nodo_sucesor); // para que se muestre en la iteración
              cola.push(nodo_sucesor);
            }          
          }
        }
      }
    }
    inspeccionados.emplace_back(nodo_actual);
    if (nodo_actual->GetNumero() == destino_) {
      ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
      break;
    }
    iteracion++;
  }
  // Generamos sus sucesores
  std::vector<Nodo*> camino;
  // Hacer funcion que recorra los nodos padre
  fichero << "Camino: ";
  GenerarCamino(nodo_actual, fichero, camino);
  fichero << std::endl;

  GenerarCoste(camino, costo);
  fichero << "Costo: " << costo << std::endl;
}

void Grafo::BusquedaProfundidad(std::ofstream& fichero) {
  int iteracion = 1, costo = 0;
  std::stack<Nodo*> pila;
  std::vector<Nodo*> inspeccionados, generados;
  std::unordered_set<int> nodos_generados;
  nodos_generados.insert(raiz_->GetNumero());

  Nodo* nodo_actual = nullptr;
  pila.push(raiz_);
  generados.emplace_back(raiz_);

  while (true) {
    ImprimirSolucion(fichero, generados, inspeccionados, iteracion);

    nodo_actual = pila.top();
    pila.pop();

    for (int i = 1; i <= numero_vertices_; ++i) {
      if (i != nodo_actual->GetNumero()) {
        auto it = distancias_.find(std::make_pair(i, nodo_actual->GetNumero()));

        if (it != distancias_.end()) {
          if (nodos_generados.find(i) == nodos_generados.end()) {
            Nodo* nuevo_nodo = new Nodo(i);
            generados.emplace_back(nuevo_nodo);
            pila.push(nuevo_nodo);
            nuevo_nodo->SetNodoPadre(nodo_actual);
            nodos_generados.insert(i);
          } else {
            int k;
            for (k = 0; k < generados.size(); ++k) {
              if (generados[k]->GetNumero() == i) break;
            }
            Nodo* nodo_sucesor = generados[k];
            auto it2 = std::find(inspeccionados.begin(), inspeccionados.end(), nodo_sucesor);
            if (RevisarRama(nodo_sucesor, nodo_actual) && it2 == inspeccionados.end()) {
              // generados.emplace_back(nodo_sucesor); // para que se muestre en la iteración
              pila.push(nodo_sucesor);
            }          
          }
        }
      }
    }
    inspeccionados.emplace_back(nodo_actual);
    if (nodo_actual->GetNumero() == destino_) {
      ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
      break;
    }
    iteracion++;
  }
  // Generamos sus sucesores
  std::vector<Nodo*> camino;
  // Hacer funcion que recorra los nodos padre
  fichero << "Camino: ";
  GenerarCamino(nodo_actual, fichero, camino);
  fichero << std::endl;

  GenerarCoste(camino, costo);
  fichero << "Costo: " << costo << std::endl;
}

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

bool Grafo::RevisarRama(Nodo* nodo_insertar, Nodo* nodo_actual) {
  Nodo* nodo_padre = nodo_actual->GetPadre();
  while (nodo_padre != nullptr) {
    if (nodo_padre == nodo_insertar) return false;  
    nodo_padre = nodo_padre->GetPadre();
  }
  return true; // Se llegó hasta la raíz sin encontrar un nodo igual
}

void Grafo::GenerarCoste(const std::vector<Nodo*>& camino, int& costo) {
  costo = 0;
  for (int i = 0; i < camino.size(); i++) {
    int j = i + 1;
    if (j == camino.size()) break;
    auto it = distancias_.find(std::make_pair(camino[i]->GetNumero(), camino[j]->GetNumero()));
    costo += it->second;
  }
}

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
