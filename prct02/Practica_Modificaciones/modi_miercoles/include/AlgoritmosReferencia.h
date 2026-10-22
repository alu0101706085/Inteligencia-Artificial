#ifndef ALGORITMOSREFERENCIAS_H
#define ALGORITMOSREFERENCIAS_H

#include <random>

#include "instancia_PFSP.h"
#include "solucion.h"

class AlgoritmosReferencias {
 public:
  AlgoritmosReferencias() = default;
  static Solucion Alg_Aleatorio(const InstanciaPFSP& instancia, std::mt19937& n_aleatorio, int limite = 50000);
  static Solucion Alg_NEH(const InstanciaPFSP& instancia);
};

#endif