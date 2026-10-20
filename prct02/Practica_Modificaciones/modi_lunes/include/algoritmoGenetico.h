#ifndef ALGORITMOGENETICO_H
#define ALGORITMOGENETICO_H

#include <vector>
#include <random>

#include "instancia_PFSP.h"
#include "solucion.h"

class AlgoritmoGenetico {
 public:
  AlgoritmoGenetico(const InstanciaPFSP& instancia, std::mt19937& gen, int tam_poblacion,
                    double prob_cruce, double prob_mutacion, double prob_busqueda_local, // <-- NUEVO
                    int tam_torneo, int presupuesto_max) :
                    instancia_(instancia), gen_(gen), tam_poblacion_(tam_poblacion), prob_cruce_(prob_cruce), 
                    prob_mutacion_(prob_mutacion), prob_busqueda_local_(prob_busqueda_local), // <-- NUEVO
                    tam_torneo_(tam_torneo), presupuesto_max_(presupuesto_max), 
                    n_evaluaciones_makespan_(0), solucion_() {}
  Solucion GetSolucion() const { return solucion_; }
  void InicializarPoblacion();
  Solucion SeleccionTorneo();
  std::pair<Solucion, Solucion> CruceOX(const Solucion& solucion1, const Solucion& solucion2);
  void Mutacion(Solucion& solucion);
  
  // <-- NUEVAS FUNCIONES -->
  void BusquedaLocal(Solucion& solucion); 
  void BusquedaLocalForzada(Solucion& solucion, int posicion_forzada); 
  // <---------------------->

  void Reemplazo(std::vector<Solucion> lista);
  void Ejecutar(bool guardar_convergencia = false);
 private:
  const InstanciaPFSP& instancia_;
  std::mt19937& gen_;
  int tam_poblacion_;
  std::vector<Solucion> poblacion_;
  double prob_cruce_;
  double prob_mutacion_;
  double prob_busqueda_local_; // <-- NUEVO
  int tam_torneo_;
  int presupuesto_max_;
  int n_evaluaciones_makespan_;
  Solucion solucion_;
};
#endif