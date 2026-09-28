#ifndef MAPA_H
#define MAPA_H

#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include "estado.h"

class Mapa {
 public:
  // Constructores
  Mapa() = default;
  // Getters
  int GetNFilas() const { return n_filas_; }
  int GetNColumnas() const { return n_columnas_; }
  std::pair<int, int> GetOrigen() const { return origen_; }
  std::pair<int, int> GetDestino() const { return destino_; }
  std::vector<std::vector<int>> GetMatriz() const { return matriz_; }
  // Setters
  void SetNFilas(int n_filas) { n_filas_ = n_filas; }
  void SetNColumnas(int n_columnas) { n_columnas_ = n_columnas; }
  void SetFilas(const int filas) { n_filas_ = filas; }
  void SetColumnas(const int colmunas) { n_columnas_ = colmunas; }
  void SetOrigen(const int linea, const int columna);
  void SetDestino(const int linea, const int columna);
  // Funciones públicas
  void GuardarEnMatriz(const std::vector<int> fila) { matriz_.emplace_back(fila); }
  // Comportamiento mapa
  bool CargarFichero(const std::string& fichero);
  bool EsMovimientoValido(int fila, int columna);
  void GuardarFichero(const std::string& fichero, const std::vector<Estado>& camino);
  int ObtenerCoste(int fila, int columna);
 private:
  int n_filas_;
  int n_columnas_;
  std::vector<std::vector<int>> matriz_;
  std::pair<int, int> origen_;
  std::pair<int, int> destino_;
};

#endif