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
#pragma once
#include <string>
#include <iostream>
#include <fstream>

int CheckParams(int argc, char* argv[], std::string&);
int CheckErrors(const int&);
void Help();
void Menu();
