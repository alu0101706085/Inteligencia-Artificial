#ifndef ALGORITMOGENETICO_H
#define ALGORITMOGENETICO_H

#include <vector>
#include <random>

#include "instancia_PFSP.h"
#include "solucion.h"

class AlgoritmoGenetico {
 public:
  AlgoritmoGenetico(const InstanciaPFSP& instancia, std::mt19937& gen, int tam_poblacion,
                    double prob_cruce, double prob_mutacion, int tam_torneo, int presupuesto_max, int tipo_reemplazo) :
                    instancia_(instancia), gen_(gen), tam_poblacion_(tam_poblacion), prob_cruce_(prob_cruce), 
                    prob_mutacion_(prob_mutacion), tam_torneo_(tam_torneo), presupuesto_max_(presupuesto_max), 
                    n_evaluaciones_makespan_(0), solucion_(), tipo_reemplazo_(tipo_reemplazo) {}
  Solucion GetSolucion() const { return solucion_; }
  void InicializarPoblacion();
  Solucion SeleccionTorneo();
  std::pair<Solucion, Solucion> CruceOX(const Solucion& solucion1, const Solucion& solucion2);
  void Mutacion(Solucion& solucion);
  void Reemplazo(std::vector<Solucion> lista);
  void Ejecutar(bool guardar_convergencia = false);
  void ReemplazoModificacion(std::vector<Solucion> lista); // <-- NUEVO M5 
 private:
  const InstanciaPFSP& instancia_;
  std::mt19937& gen_;
  int tam_poblacion_;
  std::vector<Solucion> poblacion_;
  double prob_cruce_;
  double prob_mutacion_;
  int tam_torneo_;
  int presupuesto_max_;
  int n_evaluaciones_makespan_;
  Solucion solucion_;
  int tipo_reemplazo_; // <-- NUEVO M5 
};

#endif