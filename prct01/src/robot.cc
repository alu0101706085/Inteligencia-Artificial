#include <iostream>
#include <cmath>
#include <algorithm>

#include "robot.h"

std::vector<Estado> Robot::AlgoritmoEstrella() {

}

std::vector<Estado> Robot::ObtenerVecinosValidos(Estado* estado_actual) {
  std::vector<Estado> vecinos_validos;
  int fila = estado_actual->GetFila();
  int columna = estado_actual->GetColumna();
  if (mapa_.EsMovimientoValido(fila + 1, columna)) {
    int coste_g = estado_actual->GetCosteG() + mapa_.ObtenerCoste(fila + 1, columna);
    int coste_h = CalcularHeuristica(fila + 1, columna);
    vecinos_validos.emplace_back(Estado(fila + 1, columna, coste_g, coste_h, estado_actual));
  } 
  if (mapa_.EsMovimientoValido(fila - 1, columna)) {
    int coste_g = estado_actual->GetCosteG() + mapa_.ObtenerCoste(fila - 1, columna);
    int coste_h = CalcularHeuristica(fila - 1, columna);
    vecinos_validos.emplace_back(Estado(fila - 1, columna, coste_g, coste_h, estado_actual));
  } 
  if (mapa_.EsMovimientoValido(fila, columna + 1)) {
    int coste_g = estado_actual->GetCosteG() + mapa_.ObtenerCoste(fila, columna + 1);
    int coste_h = CalcularHeuristica(fila, columna + 1);
    vecinos_validos.emplace_back(Estado(fila, columna + 1, coste_g, coste_h, estado_actual));
  } 
  if (mapa_.EsMovimientoValido(fila, columna - 1)) {
    int coste_g = estado_actual->GetCosteG() + mapa_.ObtenerCoste(fila, columna - 1);
    int coste_h = CalcularHeuristica(fila, columna - 1);
    vecinos_validos.emplace_back(Estado(fila, columna - 1, coste_g, coste_h, estado_actual));
  }
  return vecinos_validos;
}

int Robot::CalcularHeuristica(int fila, int columna) {
  std::pair<int, int> destino = mapa_.GetDestino();
  int distancia_rectilinea = std::abs(destino.first - fila) + std::abs(destino.second - columna);
  return 2 * distancia_rectilinea;
} 

std::vector<Estado> Robot::ReconstruirCamino(Estado* estado) {
  std::vector<Estado> camino_optimo;
  while(estado->GetEstadoAnterior() != nullptr) {
    // Nota: Incluir "*estado" obliga a ir a dicha direccion de memoria y devolver el valor alli guardado
    camino_optimo.emplace_back(*estado); 
    estado = estado->GetEstadoAnterior();
  }
  camino_optimo.emplace_back(*estado);
  std::reverse(camino_optimo.begin(), camino_optimo.end()); // Ordenar el camino final
  return camino_optimo;
}


void Robot::MostrarResultados(std::string id_instancia, int coste_total) {

}

void Robot::GuardarResultadosEnFichero(std::string ruta_salida) {

}