#include "estado.h"

bool Estado::EsIgual(const Estado& estado) const {
  if (fila_ == estado.fila_ && columna_ == estado.columna_) {
    return true;
  }
  return false;
}

bool Estado::operator<(const Estado& estado) const {
  if(coste_f_ != estado.coste_f_) {
    return coste_f_ < estado.coste_f_;
  }
  return coste_h_ < estado.coste_h_; // En caso de empate, priorizamos el estado con menor heurística (más cerca del destino)
}