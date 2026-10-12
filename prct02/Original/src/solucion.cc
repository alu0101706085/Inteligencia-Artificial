#include "solucion.h"

int Solucion::Evaluar(const InstanciaPFSP& instancia) {
  if (Get_Makespan() == -1) {
    makespan_ = instancia.CalcularMakespan(Get_Permutacion());
    return Get_Makespan();
  }
  return Get_Makespan();
}

bool Solucion::operator<(const Solucion& otro) const {
  return makespan_ < otro.makespan_;
}