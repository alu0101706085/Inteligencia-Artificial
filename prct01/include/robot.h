#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <vector>

#include "estado.h"
#include "mapa.h"

class Robot {
 public:
  // Constructor
  Robot(int pos_x, int pos_y, Mapa& mapa) : pos_actual_(pos_x, pos_y), mapa_(mapa) {}
  // Getters
  std::pair<int, int> GetPosActual() const { return pos_actual_; }
  // Funciones publicas
  std::vector<Estado> AlgoritmoEstrella() {}
  std::vector<Estado> Moverse() {}
  int CalcularHeuristica(int fila, int columna) {} 
  std::vector<Estado> ReconstruirCamino(Estado* estado) {}
  void MostrarResultados(std::string id_instancia, int coste_total) {}
  void GuardarResultadosEnFichero(std::string ruta_salida) {}
 private:
  std::pair<int, int> pos_actual_;
  Mapa& mapa_;
  int nodos_generados_;
  int nodos_inspeccionados_;
};

#endif