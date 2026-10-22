#ifndef ALGORITMOGENETICO_H
#define ALGORITMOGENETICO_H

#include <vector>
#include <random>

#include "instancia_PFSP.h"
#include "solucion.h"

class AlgoritmoGenetico {
 public:
  AlgoritmoGenetico(const InstanciaPFSP& instancia, std::mt19937& gen, int tam_poblacion,
                    double prob_cruce, double prob_mutacion, 
                    int tipo_seleccion, double presion_selectiva, // <-- NUEVOS PARÁMETROS M3
                    int tam_torneo, int presupuesto_max) :
                    instancia_(instancia), gen_(gen), tam_poblacion_(tam_poblacion), prob_cruce_(prob_cruce), 
                    prob_mutacion_(prob_mutacion), tipo_seleccion_(tipo_seleccion), 
                    presion_selectiva_(presion_selectiva), // <-- NUEVOS PARÁMETROS M3
                    tam_torneo_(tam_torneo), presupuesto_max_(presupuesto_max), 
                    n_evaluaciones_makespan_(0), solucion_() {}
  Solucion GetSolucion() const { return solucion_; }
  void InicializarPoblacion();
  
  Solucion SeleccionTorneo();
  
  // <-- NUEVAS FUNCIONES M3 -->
  void PrepararRanking();
  Solucion SeleccionRanking();
  static void EjecutarEjemploComprobacionRanking(); 
  // <------------------------->

  std::pair<Solucion, Solucion> CruceOX(const Solucion& solucion1, const Solucion& solucion2);
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
  
  int tipo_seleccion_; // <-- 0: Torneo, 1: Ranking
  double presion_selectiva_; // <-- Parámetro s
  std::vector<int> indices_ranking_; // <-- Para no ordenar in-place
  std::vector<double> prob_acumulada_; // <-- Caché por generación
  
  int tam_torneo_;
  int presupuesto_max_;
  int n_evaluaciones_makespan_;
  Solucion solucion_;
};

#endif