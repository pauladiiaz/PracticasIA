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
#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <algorithm>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include "../nodo/nodo.h"
#include "../laberinto/laberinto.h"

class Grafo {
 public:
  Grafo() : raiz_(nullptr) {}
  Grafo(const std::string& nombre_fichero) : laberinto_(nombre_fichero, numero_filas_, numero_columnas_) {
    raiz_ = new Nodo(1, laberinto_.GetEntrada());
  }
  Grafo(Nodo* raiz, const std::string& nombre_fichero) : raiz_(raiz), laberinto_(nombre_fichero, numero_filas_, numero_columnas_) {}
  
  void BusquedaA(std::ofstream&);
  int FuncionHManhattan(Casilla*, Casilla*);
  int FuncionG(Nodo*);
  int FuncionF(int, int);

  void CambiarEntrada(Coordenada nueva_entrada) { 
    laberinto_.CambiarEntrada(nueva_entrada); 
    raiz_->SetCasilla(laberinto_.GetEntrada());
    };
  void CambiarSalida(Coordenada nueva_salida) { laberinto_.CambiarSalida(nueva_salida); };

  void ImprimirSolucion(std::string&, const std::vector<Nodo*>&, const std::vector<Nodo*>&, const int&);
  bool RevisarRama(Nodo*, Nodo*);
  void GenerarCoste(const std::vector<Nodo*>&, int&);
  void GenerarCamino(Nodo*, std::ofstream&, std::vector<Nodo*>&, int, int);
  bool EsDiagonal(Nodo*);

  Nodo* GetRaiz() const { return raiz_; }
  Laberinto GetLaberinto() const { return laberinto_; }
  int GetNumeroFilas() const { return numero_filas_; }
  int GetNumeroColumnas() const { return numero_columnas_; }

  Grafo& operator=(const Grafo& other) {
    this->laberinto_ = other.laberinto_;
    delete this->raiz_; // si falla al cambiar de fichero borrar esto
    delete other.raiz_;
    this->raiz_ = new Nodo(1, this->laberinto_.GetEntrada());
    this->numero_filas_ = other.numero_filas_;
    this->numero_columnas_ = other.numero_columnas_;
    return *this;
  }

 private:
  Nodo* raiz_; // nodo origen
  int numero_filas_, numero_columnas_;
  Laberinto laberinto_;
};