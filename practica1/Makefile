# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# 3º Curso en Grado en Ingeniería Informática
# Inteligencia Artificial
# Práctica 1: Búsquedas no informadas
#
# @author Paula Díaz Jorge alu0101540863@ull.edu.es
# @date 24 sep 2024
# @brief Makefile del proyecto. Permite compilar y hacer make clean para borrar los binarios

CXX = g++
CXXFLAGS = -Wall -std=c++11
files = src/c++/*/*cc

all: programa

programa: $(files)
	@echo "======================================================"
	@echo "compilando..."
	@echo "======================================================"
	@$(CXX) $(CXXFLAGS) -o bin/BusquedasNoInformadas $(files) src/c++/*cc

.PHONY: clean
clean:
	@echo "cleaning up the directory..."
	@rm bin/BusquedasNoInformadas