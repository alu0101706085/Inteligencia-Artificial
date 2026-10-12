#ifndef SOLUCION_H
#define SOLUCION_H

#include <vector>

#include "instancia_PFSP.h"

class Solucion {
 public:
  Solucion() = default;
  Solucion(std::vector<int> permutacion) : permutacion_(permutacion) {}
  int Get_Makespan() const { return makespan_; }
  std::vector<int> Get_Permutacion() const { return permutacion_; }
  int Evaluar(const InstanciaPFSP& instancia);
  bool operator<(const Solucion& otro) const;
 private:
  int makespan_ = -1;
  std::vector<int> permutacion_;
};

#endif