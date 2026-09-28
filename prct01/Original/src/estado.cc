#include "estado.h"

/**
 * @brief Comprueba si el estado actual es espacialmente igual a otro estado.
 */
bool Estado::EsIgual(const Estado& estado) const {
  if (fila_ == estado.fila_ && columna_ == estado.columna_) {
    return true;
  }
  return false;
}

/**
 * @brief Sobrecarga del operador menor que (<) para evaluar la prioridad entre dos estados.
 * 
 * Compara los estados basándose en su coste total estimado (F = G + H). 
 * En caso de que ambos estados tengan el mismo coste total, se desempata 
 * priorizando aquel que tenga un menor coste heurístico (H), es decir, 
 * el que se estime más cercano al destino.
 */
bool Estado::operator<(const Estado& estado) const {
  if(coste_f_ != estado.coste_f_) {
    return coste_f_ < estado.coste_f_;
  }
  return coste_h_ < estado.coste_h_; // En caso de empate, priorizamos el estado con menor heurística (más cerca del destino)
}

/**
 * @brief Sobrecarga del operador mayor que (>) para su uso en colas de prioridad.
 * 
 * Fundamental para que estructuras como std::priority_queue configuradas con 
 * std::greater ordenen los nodos extrayendo primero aquellos con menor coste. 
 * Sigue la misma lógica de desempate por heurística (H) que el operador menor que (<).
 */
bool Estado::operator>(const Estado& otro) const {
  if (coste_f_ != otro.coste_f_) {
    return coste_f_ > otro.coste_f_;
  }
  // En caso de empate en coste_f, priorizamos el que tenga menor heurística h
  return coste_h_ > otro.coste_h_;
}