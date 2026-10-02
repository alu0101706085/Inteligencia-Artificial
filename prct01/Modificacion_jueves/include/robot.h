#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <functional>

#include "estado.h"
#include "mapa.h"

class Robot {
 public:
  // Constructor
  Robot(int pos_x, int pos_y, Mapa& mapa) : pos_actual_(pos_x, pos_y), mapa_(mapa) {}
  // Getters
  std::pair<int, int> GetPosActual() const { return pos_actual_; }
  // Setters
  void SetNodosGenerados(int nodos_generados) { nodos_generados_ = nodos_generados; }
  void SetNodosInspeccionados(int nodos_inspeccionados) { nodos_inspeccionados_ = nodos_inspeccionados; }
  // Funciones publicas
  std::vector<Estado> AlgoritmoEstrella(std::string& mensaje);
  std::vector<Estado> ObtenerVecinosValidos(Estado* estado);
  int CalcularHeuristica(int fila, int columna);
  std::vector<Estado> ReconstruirCamino(Estado* estado);
  std::string MostrarResultados(int iteracion, 
                                std::priority_queue<Estado, std::vector<Estado>, std::greater<Estado>> copia_abiertos, 
                                const std::vector<std::pair<int, int>>& cerrados);
  void GuardarResultadosEnFicheros(std::string ruta_salida1, std::string ruta_salida2, std::string iteraciones, std::vector<Estado> camino);
 private:
  std::pair<int, int> pos_actual_;
  Mapa& mapa_;
  int nodos_generados_;
  int nodos_inspeccionados_;
};

#endif