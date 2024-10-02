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
  std::vector<Nodo*> inspeccionados, generados;
  Nodo* nodo_actual = nullptr;

  cola.push(raiz_);
  // Añadimos el nodo raíz al conjunto
  generados.emplace_back(raiz_);

  while (true) {
    ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
        
    nodo_actual = cola.front();
    cola.pop();

    if (nodo_actual->GetNumero() == destino_) {
      inspeccionados.emplace_back(nodo_actual);
      iteracion++;
      ImprimirSolucion(fichero, generados, inspeccionados, iteracion);
      break;      
    } else {
      // Generamos sus sucesores
      auto it = std::find(inspeccionados.begin(), inspeccionados.end(), nodo_actual);
      if (it == inspeccionados.end()) inspeccionados.emplace_back(nodo_actual);
      for (int i = 1; i <= numero_vertices_; ++i) { // Buscamos sus sucesores 
        if (i != nodo_actual->GetNumero()) { // Evitamos que se busque a sí mismo como sucesor
          auto ite1 = distancias_.find(std::make_pair(i, nodo_actual->GetNumero()));
                    
          // Si i es un sucesor
          if (ite1 != distancias_.end()) {
            bool generado = false;
            // Comprobamos si el nuevo nodo ya ha sido creado
            int k;
            for (k = 0; k < generados.size(); ++k) {
              if (generados[k]->GetNumero() == i) {
                generado = true;
                break;
              }
            }
            if (!generado) { // Si no ha sido generado, lo añadimos
              Nodo* nuevo_nodo = new Nodo(i);
              generados.emplace_back(nuevo_nodo);
              cola.push(nuevo_nodo);
              nuevo_nodo->SetNodoPadre(nodo_actual);                
            } else {
              Nodo* nodo_sucesor = generados[k];
              if (RevisarRama(nodo_sucesor, nodo_actual)) {
                cola.push(nodo_sucesor);
              }
            }
          }
        }
      }
      iteracion++;
    }
  }
  std::vector<Nodo*> camino;
  // Hacer funcion que recorra los nodos padre
  fichero << "Camino: ";
  GenerarCamino(nodo_actual, fichero, camino);
  fichero << std::endl;

  GenerarCoste(camino, costo);
  fichero << "Costo: " << costo << std::endl;

}


// void Arbol::BusquedaProfundidad(std::ofstream& fichero) {
//   int iteracion = 1, costo = 0;

// }

void Arbol::ImprimirSolucion(std::ofstream& fichero, const std::vector<Nodo*>& nodos_generados, 
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

bool Arbol::RevisarRama(Nodo* nodo_insertar, Nodo* nodo_actual) {
  Nodo* nodo_padre = nodo_actual->GetPadre();
  while (nodo_padre != nullptr) {
    if (nodo_padre == nodo_insertar) return false;  
    nodo_padre = nodo_padre->GetPadre();
  }
  return true; // Se llegó hasta la raíz sin encontrar un nodo igual
}

void Arbol::GenerarCoste(const std::vector<Nodo*>& camino, int& costo) {
  costo = 0;
  for (int i = 0; i < camino.size(); i++) {
    int j = i + 1;
    if (j == camino.size()) break;
    auto it = distancias_.find(std::make_pair(camino[i]->GetNumero(), camino[j]->GetNumero()));
    costo += it->second;
  }
}

void Arbol::GenerarCamino(Nodo* nodo_actual, std::ofstream& fichero, std::vector<Nodo*>& camino) {
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