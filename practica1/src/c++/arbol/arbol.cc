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

Arbol::Arbol(Nodo* raiz, const std::string& nombre_fichero, const int& origen, const int& destino) : raiz_(raiz), origen_(origen), 
        destino_(destino) {
  // Leer los datos desde el fichero
  nodos_.emplace_back(raiz_);
  std::ifstream fichero(nombre_fichero);
  if (!fichero.is_open()) {
    throw std::runtime_error("Error al abrir el fichero.");
  }
  std::string linea;
  getline(fichero, linea);
  numero_vertices_ = std::stoi(linea);
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

void Arbol::BusquedaAmplitud(std::ofstream& fichero) {
  int iteracion = 1, costo = 0;
  std::queue<Nodo*> cola;
  std::vector<Nodo> inspeccionados, generados;
  Nodo* nodo_actual = nullptr;
  cola.push(raiz_);
  generados.emplace_back(*raiz_);
  // ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
  while (!cola.empty()) {
    ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
    nodo_actual = cola.front();
    cola.pop();
    if (nodo_actual->GetNumero() == destino_) { // si el nodo actual es el nodo destino
      iteracion++;
      inspeccionados.emplace_back(*nodo_actual);
      ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
      break;
    }
    inspeccionados.emplace_back(*nodo_actual);
    // Si el nodo no es destino
    for (int i = 1; i <= numero_vertices_; ++i) { // Buscamos sus sucesores en su mismo nivel
      if (i != nodo_actual->GetNumero()) { // Evitamos que se busque a sí mismo como sucesor
        auto ite1 = distancias_.find(std::make_pair(i, nodo_actual->GetNumero()));
        auto ite2 = distancias_.find(std::make_pair(nodo_actual->GetNumero(), i));

        Nodo* nuevo_nodo = new Nodo(i);
        auto it_nodos = std::find(nodos_.begin(), nodos_.end(), nuevo_nodo);
        auto it = std::find(inspeccionados.begin(), inspeccionados.end(), *nuevo_nodo);

        if ((ite1 != distancias_.end() || ite2 != distancias_.end()) && it == inspeccionados.end()) {// Si i es un sucesor y no ha sido inspeccionado
          if (it_nodos != nodos_.end()) { // Ya ha sido creado en memoria
            nodo_actual->NuevoSucesor(*it_nodos, ite1->second);
            cola.push(*it_nodos);
          } else {     
            generados.emplace_back(*nuevo_nodo);
            cola.push(nuevo_nodo);
            nodos_.emplace_back(nuevo_nodo);
            nodo_actual->NuevoSucesor(nuevo_nodo, ite1->second);
            nuevo_nodo->SetNodoPadre(nodo_actual);
          }
        }
      }
    }
    iteracion++;
  }
  // Hacer funcion que recorra los nodos padre
  fichero << "Camino: ";
  GenerarCamino(nodo_actual, fichero, costo);
  fichero << "Costo: " << costo << std::endl;
}

void Arbol::ImprimirSolucion(std::ofstream& fichero, const std::vector<Nodo>& nodos_generados, 
            const std::vector<Nodo>& nodos_inspeccionados, const int& iteracion) {
  fichero << "Iteración " << iteracion << std::endl;
  fichero << "Nodos generados: ";
  int it = 1;
  for (auto nodo : nodos_generados) {
    fichero << nodo.GetNumero();
    if (it < nodos_generados.size()) fichero << ", ";
    it++;
  }
  fichero << std::endl << "Nodos inspeccionados: ";
  if (nodos_inspeccionados.empty()) {
    fichero << "-" << std::endl;
  } else {
    it = 1;
    for (auto nodo : nodos_inspeccionados) {
      fichero << nodo.GetNumero();
      if (it < nodos_inspeccionados.size()) fichero << ", ";
      it++;
    }
  }
  fichero << std::endl << "---------------------------------------------" << std::endl;
}

void Arbol::GenerarCamino(Nodo* nodo_final, std::ofstream& fichero, int& costo) {
  std::vector<Nodo*> camino;
  camino.emplace_back(nodo_final);
  while (nodo_final != raiz_) {
    costo += distancias_[std::make_pair(nodo_final->GetNumero(), nodo_final->GetPadre()->GetNumero())];
    nodo_final = nodo_final->GetPadre();
    camino.emplace_back(nodo_final);
  }
  std::reverse(camino.begin(), camino.end());
  int it = 1;
  for (const auto nodo : camino) {
    fichero << nodo->GetNumero();
    if (it < camino.size()) {
      fichero << " - ";
    }
    it++;
  }
  fichero << std::endl;
}


