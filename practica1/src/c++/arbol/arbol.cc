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
        distancias_.insert(std::make_pair(std::make_pair(i, j), std::stof(linea))); // tener en cuenta la simetría
        distancias_.insert(std::make_pair(std::make_pair(j, i), std::stof(linea)));
        if (std::stof(linea) != -1) numero_aristas_++;
      } else {
        throw std::runtime_error("Error al leer el fichero.");
      }
    }
  }
  
}

void Arbol::BusquedaAmplitud(std::ofstream& fichero) {
  int iteracion = 1;
  std::vector<Nodo*> nodos_generados, nodos_inspeccionados;
  nodos_generados.push_back(raiz_);
  Nodo* nodo_actual = raiz_;
  int costo = 0;
  while (true) {
    fichero << "Iteración " << iteracion << std::endl;
    fichero << "Nodos generados: ";
    for (int i = 0; i < nodos_generados.size(); ++i) {
      fichero << nodos_generados[i]->GetNumero();
      if (i != nodos_generados.size() - 1) fichero << ", ";
    }
    fichero << std::endl << "Nodos inspeccionados: " << std::endl;
    for (int i = 0; i < nodos_inspeccionados.size(); ++i) {
      if (nodos_inspeccionados.empty()) {
        std::cout << "-" << std::endl;
      } else {
        fichero << nodos_inspeccionados[i]->GetNumero();
        if (i != nodos_inspeccionados.size() - 1) fichero << ", ";
      }
    }
    if (nodo_actual->GetNumero() == destino_) break;
    // Buscar los sucesores en amplitud
    for (int i = nodo_actual->GetNumero() + 1; i <= numero_aristas_; ++i) {
      auto it1 = distancias_.find(std::make_pair(nodo_actual->GetNumero(), i));
      auto it2 = distancias_.find(std::make_pair(i, nodo_actual->GetNumero()));
      if (it1 != distancias_.end() || it2 != distancias_.end()) { // Comprobamos si es sucesor
        // Comprobar si el nodo ya ha sido creado
        Nodo* nodo_nuevo = new Nodo(i);
        auto it = std::find(nodos_.begin(), nodos_.end(), nodo_nuevo);
        if (it != nodos_.end()) {
          nodo_actual->NuevoSucesor(nodos_[std::distance(nodos_.begin(), it)]);
          nodos_inspeccionados.emplace_back(nodos_[std::distance(nodos_.begin(), it)]);
        } else {
          nodo_nuevo->SetNodoPadre(nodo_actual);
          nodo_actual->NuevoSucesor(nodo_nuevo);
          nodos_.emplace_back(nodo_nuevo);
          nodos_inspeccionados.emplace_back(nodo_nuevo);
        }
      }
    }
    
  }
  
}


