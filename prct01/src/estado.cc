#include "estado.h"

bool Estado::EsIgual(Estado estado) {
  if (GetFila() == estado.GetFila() && GetColumna() == estado.GetColumna()) {
    return true;
  }
  return false;
}

bool Estado::operator<(Estado estado) const {
  
}