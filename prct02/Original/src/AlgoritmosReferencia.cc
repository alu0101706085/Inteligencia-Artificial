#include "AlgoritmosReferencia.h"

#include <algorithm>
#include <vector>
#include <numeric>
#include <random>
#include <functional>

Solucion AlgoritmosReferencias::Alg_Aleatorio(const InstanciaPFSP& instancia, std::mt19937& n_aleatorio, int limite) {
  Solucion mejor_opcion;
  mejor_opcion.Set_Makespan(999999);
  std::vector<int> tareas(instancia.Get_NTareas());
  std::iota(tareas.begin(), tareas.end(), 1); // Vector con n tareas ordenados (1, 2, 3...)
  for (int i{0}; i < limite; i++) {
    std::shuffle(tareas.begin(), tareas.end(), n_aleatorio);
    std::vector<int> nueva_permutacion = tareas;
    Solucion nueva_opcion(nueva_permutacion);
    nueva_opcion.Evaluar(instancia);
    if (nueva_opcion < mejor_opcion) mejor_opcion = nueva_opcion;
  }
  return mejor_opcion;
}

Solucion AlgoritmosReferencias::Alg_NEH(const InstanciaPFSP& instancia) {
  std::vector<std::pair<int, int>> costes;
  for (int i{0}; i < instancia.Get_NTareas(); i++) {
    int coste_tarea{0};
    for (int j{0}; j < instancia.Get_NMaquinas(); j++) {
      coste_tarea += instancia.Get_MatrizTiempos()[j][i];
    }
    costes.emplace_back(i + 1, coste_tarea); // Las permutaciones trabajan con id de 1 a n
  }
  std::sort(costes.begin(), costes.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.second > b.second; });
  // Manejo 2 primeros valores de la permutacion
  std::vector<int> opcion_ganadora;
  std::vector<int> combinacion_A = {costes[0].first, costes[1].first};
  std::vector<int> combinacion_B = {costes[1].first, costes[0].first};
  Solucion opcion_A(combinacion_A);
  Solucion opcion_B(combinacion_B);
  opcion_A.Evaluar(instancia);
  opcion_B.Evaluar(instancia);
  if (opcion_A < opcion_B) opcion_ganadora = opcion_A.Get_Permutacion();
  else opcion_ganadora = opcion_B.Get_Permutacion(); 
  
  // Manejo resto de valores de la permutacion
  std::vector<int> temporal;
  for (size_t i{2}; i < costes.size(); i++) {
    int makespan_mas_bajo{999999};
    std::vector<int> mejor_permutacion;
    for (size_t j{0}; j <= opcion_ganadora.size(); j++) {
      temporal = opcion_ganadora;
      temporal.insert(temporal.begin() + j, costes[i].first);
      Solucion opcion_X(temporal);
      opcion_X.Evaluar(instancia);
      if (opcion_X.Get_Makespan() < makespan_mas_bajo) {
        makespan_mas_bajo = opcion_X.Get_Makespan();
        mejor_permutacion = opcion_X.Get_Permutacion();
      }
    }
    opcion_ganadora = mejor_permutacion;
  }
  Solucion ganador(opcion_ganadora);
  ganador.Evaluar(instancia);
  return ganador;
}