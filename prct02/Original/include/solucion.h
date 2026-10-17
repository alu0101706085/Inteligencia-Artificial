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
  void Set_Makespan(int makespan) { makespan_ = makespan; }
  void Set_Permutacion(std::vector<int> permutacion) { permutacion_ = std::move(permutacion); makespan_ = -1; }
  int Evaluar(const InstanciaPFSP& instancia);
  bool EsFactible(int n_tareas) const;
  bool operator<(const Solucion& otro) const;
 private:
  int makespan_ = -1;
  std::vector<int> permutacion_;
};

#endif