#ifndef ALGORITMOGENETICO_H
#define ALGORITMOGENETICO_H

#include <vector>
#include <random>

#include "instancia_PFSP.h"
#include "solucion.h"

class AlgoritmoGenetico {
 public:
  AlgoritmoGenetico(const InstanciaPFSP& instancia, std::mt19937& gen, int tam_poblacion,
                    double prob_cruce, double prob_mutacion, int tipo_cruce, // <-- NUEVO (tipo_cruce)
                    int tam_torneo, int presupuesto_max) :
                    instancia_(instancia), gen_(gen), tam_poblacion_(tam_poblacion), prob_cruce_(prob_cruce), 
                    prob_mutacion_(prob_mutacion), tipo_cruce_(tipo_cruce), // <-- NUEVO
                    tam_torneo_(tam_torneo), presupuesto_max_(presupuesto_max), 
                    n_evaluaciones_makespan_(0), solucion_() {}
  Solucion GetSolucion() const { return solucion_; }
  void InicializarPoblacion();
  Solucion SeleccionTorneo();
  std::pair<Solucion, Solucion> CruceOX(const Solucion& solucion1, const Solucion& solucion2);
  
  // <-- NUEVAS FUNCIONES M2 -->
  std::pair<Solucion, Solucion> CrucePMX(const Solucion& solucion1, const Solucion& solucion2);
  static void EjecutarEjemploComprobacionPMX(); 
  // <------------------------->

  void Mutacion(Solucion& solucion);
  void Reemplazo(std::vector<Solucion> lista);
  void Ejecutar(bool guardar_convergencia = false);
 private:
  const InstanciaPFSP& instancia_;
  std::mt19937& gen_;
  int tam_poblacion_;
  std::vector<Solucion> poblacion_;
  double prob_cruce_;
  double prob_mutacion_;
  int tipo_cruce_; // <-- NUEVO (0 = OX, 1 = PMX)
  int tam_torneo_;
  int presupuesto_max_;
  int n_evaluaciones_makespan_;
  Solucion solucion_;
};

#endif