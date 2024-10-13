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

#include "laberinto.h"
#include <fstream>
#include <sstream>

Laberinto::Laberinto(const std::string& nombre_fichero, int& numero_filas, int& numero_columnas) : entrada_(nullptr), salida_(nullptr) {
  std::ifstream fichero(nombre_fichero);
  if (!fichero.is_open()) {
    throw std::runtime_error("Error al abrir el fichero.");
  }
  std::string linea;
  if (getline(fichero, linea)) {
    numero_filas = std::stoi(linea);
  } else {
    throw std::runtime_error("Error al leer el número de filas.");
  }
  if (getline(fichero, linea)) {
    numero_columnas = std::stoi(linea);
  } else {
    throw std::runtime_error("Error al leer el número de columnas.");
  }
  casillas_.resize(numero_filas, std::vector<Casilla>(numero_columnas, Casilla(0, Coordenada(0, 0))));
  for (int i = 0; i < numero_filas; ++i) {
    getline(fichero, linea);
    std::istringstream ss(linea);
    int tipo, k = 0;
    while (ss >> tipo) {
      Coordenada coordenada(i, k);
      Casilla nueva_casilla(tipo, coordenada);
      casillas_[i][k] = nueva_casilla;
      if (tipo == 3) entrada_ = &casillas_[i][k];
      if (tipo == 4) salida_ = &casillas_[i][k];
      k++;      
    }
  }
  if (entrada_ == nullptr || salida_ == nullptr) throw std::runtime_error("Error: no se encontró una entrada y salida para el laberinto");
}

Laberinto& Laberinto::operator=(const Laberinto& other) {
    if (this != &other) { // Protección contra auto-asignación
        casillas_ = other.casillas_;
        // Actualizar entrada_ y salida_ para que apunten a las casillas correspondientes en este objeto
        if (other.entrada_ != nullptr) {
            int entradaX = other.entrada_->GetCoordenada().GetX();
            int entradaY = other.entrada_->GetCoordenada().GetY();
            entrada_ = &casillas_[entradaX][entradaY];
        } else {
            entrada_ = nullptr;
        }
        if (other.salida_ != nullptr) {
            int salidaX = other.salida_->GetCoordenada().GetX();
            int salidaY = other.salida_->GetCoordenada().GetY();
            salida_ = &casillas_[salidaX][salidaY];
        } else {
            salida_ = nullptr;
        }
    }
    return *this;
}

std::vector<Casilla*> Laberinto::GetCasillasAdyacentes(Casilla* casilla) {
  if (casilla == nullptr) throw std::runtime_error("Error: la casilla no puede ser nula");
  std::vector<Casilla*> adyacentes;
  int x = casilla->GetCoordenada().GetX();
  int y = casilla->GetCoordenada().GetY();
  int maxX = casillas_.size();
  int maxY = casillas_[0].size();

  // Direcciones: arriba, abajo, izquierda, derecha, y las cuatro diagonales
  int direcciones[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (auto& direccion : direcciones) {
    int newX = x + direccion[0];
    int newY = y + direccion[1];
    // Verificar si la nueva posición está dentro de los límites del laberinto
    if ((newX >= 0 && newX < maxX && newY >= 0 && newY < maxY) && casillas_[newX][newY].EsTransitable()) {
      adyacentes.emplace_back(&casillas_[newX][newY]);
    }
  }

  return adyacentes;
}

void Laberinto::ImprimirLaberinto() {
  for (int i = 0; i < casillas_.size(); ++i) {
    for (int j = 0; j < casillas_[i].size(); ++j) {
      std::cout << casillas_[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void Laberinto::CambiarEntrada(Coordenada nueva_entrada) {
  if (nueva_entrada != entrada_->GetCoordenada()) { // Si la nueva entrada es distinta a la actual
    if (nueva_entrada.GetX() < 0 || nueva_entrada.GetX() >= casillas_.size() || nueva_entrada.GetY() < 0 || nueva_entrada.GetY() >= casillas_[0].size()) 
      throw std::runtime_error("Error: la entrada no puede estar fuera del laberinto");
    if ((nueva_entrada.GetY() > 0 && nueva_entrada.GetY() < casillas_[0].size() - 1) && (nueva_entrada.GetX() > 0 && nueva_entrada.GetX() < casillas_.size() - 1))
      throw std::runtime_error("Error: la entrada no puede estar en medio del laberinto");
    if ((nueva_entrada.GetX() > 0 && nueva_entrada.GetX() < casillas_.size() - 1) && (nueva_entrada.GetY() > 0 && nueva_entrada.GetY() < casillas_[0].size() - 1))
      throw std::runtime_error("Error: la entrada no puede estar en medio del laberinto");
    if (nueva_entrada != salida_->GetCoordenada()) { // Si la nueva entrada no es igual a la salida
      entrada_->SetTipo(1);
      entrada_ = &casillas_[nueva_entrada.GetX()][nueva_entrada.GetY()];
      entrada_->SetTipo(3);
    } else {
      throw std::runtime_error("Error: la entrada no puede ser igual a la salida");
    }
  }
}

void Laberinto::CambiarSalida(Coordenada nueva_salida) {
  if (nueva_salida != entrada_->GetCoordenada()) { // Si la nueva entrada es distinta a la actual
    if (nueva_salida.GetX() < 0 || nueva_salida.GetX() >= casillas_.size() || nueva_salida.GetY() < 0 || nueva_salida.GetY() >= casillas_[0].size()) 
      throw std::runtime_error("Error: la entrada no puede estar fuera del laberinto");
    if ((nueva_salida.GetY() > 0 && nueva_salida.GetY() < casillas_[0].size() - 1) && (nueva_salida.GetX() > 0 && nueva_salida.GetX() < casillas_.size() - 1))
      throw std::runtime_error("Error: la entrada no puede estar en medio del laberinto");
    if ((nueva_salida.GetX() > 0 && nueva_salida.GetX() < casillas_.size() - 1) && (nueva_salida.GetY() > 0 && nueva_salida.GetY() < casillas_[0].size() - 1))
      throw std::runtime_error("Error: la entrada no puede estar en medio del laberinto");
    if (nueva_salida != salida_->GetCoordenada()) { // Si la nueva entrada no es igual a la salida
      salida_->SetTipo(1);
      salida_ = &casillas_[nueva_salida.GetX()][nueva_salida.GetY()];
      salida_->SetTipo(4);
    } else {
      throw std::runtime_error("Error: la salida no puede ser igual a la entrada");
    }
  }
}