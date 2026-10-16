#include "algoritmoGenetico.h"

#include <algorithm>
#include <numeric>
#include <utility>

#include "AlgoritmosReferencia.h"

void AlgoritmoGenetico::InicializarPoblacion() {
  poblacion_.reserve(tam_poblacion_);
  poblacion_.push_back(AlgoritmosReferencias::Alg_NEH(instancia_));
  solucion_ = poblacion_.front();
  for (int i{1}; i < tam_poblacion_; i++) {
    std::vector<int> tareas(instancia_.Get_NTareas());
    std::iota(tareas.begin(), tareas.end(), 1);
    std::shuffle(tareas.begin(), tareas.end(), gen_);
    Solucion nueva_solucion(tareas);
    nueva_solucion.Evaluar(instancia_);
    n_evaluaciones_makespan_++;
    if (nueva_solucion < solucion_) solucion_ = nueva_solucion;
    poblacion_.push_back(nueva_solucion);
  }
}

Solucion AlgoritmoGenetico::SeleccionTorneo() {
  std::uniform_int_distribution<int> azar_poblacion(0, tam_poblacion_ - 1);
  Solucion ganador_temporal;
  ganador_temporal.Set_Makespan(999999);
  for (int i{0}; i < tam_torneo_; i++) {
    int n_aleatorio = azar_poblacion(gen_);
    if (poblacion_[n_aleatorio] < ganador_temporal) ganador_temporal = poblacion_[n_aleatorio];
  }
  return ganador_temporal;
}

std::vector<int> ObtenerPermutacionHijo(const Solucion& solucion1, const Solucion& solucion2, int num1, int num2) {
  std::vector<int> hijo(solucion1.Get_Permutacion().size(), -1);
  std::vector<bool> tareas_usadas((hijo.size() + 1), false);
  if (num2 < num1) std::swap(num1, num2);
  for (int i{num1}; i <= num2; i++) {
    hijo[i] = solucion1.Get_Permutacion()[i];
    tareas_usadas[hijo[i]] = true;
  }
  int indice_insercion{0};
  if (indice_insercion == num1) indice_insercion = num2 + 1;
  for (const auto& tareas : solucion2.Get_Permutacion()) {
    if (!tareas_usadas[tareas]) {
      hijo[indice_insercion] = tareas;
      indice_insercion++;
      if (indice_insercion == num1) indice_insercion = num2 + 1;
    }
  }
  return hijo;
}

std::pair<Solucion, Solucion> AlgoritmoGenetico::CruceOX(const Solucion& solucion1, const Solucion& solucion2) {
  std::uniform_int_distribution<int> lista_num(0, solucion1.Get_Permutacion().size() - 1);
  int num1 = lista_num(gen_);
  int num2 = lista_num(gen_);
  std::vector<int> hijo1 = ObtenerPermutacionHijo(solucion1, solucion2, num1, num2);
  std::vector<int> hijo2 = ObtenerPermutacionHijo(solucion2, solucion1, num1, num2);
  Solucion opcion_1(hijo1);
  Solucion opcion_2(hijo2);
  std::pair<Solucion, Solucion> solucion = {opcion_1, opcion_2};
  return solucion;
}

void AlgoritmoGenetico::Mutacion(Solucion& solucion) {
  std::uniform_real_distribution<double> probabilidad(0.0, 1.0);
  double valor_aleatorio = probabilidad(gen_);
  if (valor_aleatorio > prob_mutacion_) return;
  else {
    std::uniform_int_distribution<int> distribucion(0, solucion.Get_Permutacion().size() - 1);
    int origen = distribucion(gen_);
    int destino = distribucion(gen_);
    while (origen == destino) {
      origen = distribucion(gen_);
      destino = distribucion(gen_);
    }
    std::vector<int> permutacion_mutada = solucion.Get_Permutacion();
    int tarea_origen = permutacion_mutada[origen];
    permutacion_mutada.erase(permutacion_mutada.begin() + origen);
    permutacion_mutada.insert(permutacion_mutada.begin() + destino, tarea_origen);
    solucion.Set_Permutacion(permutacion_mutada);
  }
}

void AlgoritmoGenetico::Reemplazo(std::vector<Solucion> lista) {
  for (auto& elemento : lista) {
    elemento.Evaluar(instancia_);
    n_evaluaciones_makespan_++;
    if (elemento < solucion_) solucion_ = elemento;
  }
  poblacion_ = lista;
  int indice_peor_individuo{0};
  for (int i{0}; i < poblacion_.size(); i++) {
    if (poblacion_[indice_peor_individuo] < poblacion_[i]) indice_peor_individuo = i;
  }
  poblacion_[indice_peor_individuo] = solucion_;
}

void AlgoritmoGenetico::Ejecutar() {
  InicializarPoblacion();
  while( n_evaluaciones_makespan_ < presupuesto_max_) {
    std::vector<Solucion> nueva_generacion;
    nueva_generacion.reserve(tam_poblacion_);
    // Hacemos la mitad de iteraciones pq en cada cruce nacen 2 hijos
    for (int i{0}; i < tam_poblacion_ / 2; i++) { 
      Solucion padre1 = SeleccionTorneo();
      Solucion padre2 = SeleccionTorneo();
      Solucion hijo1;
      Solucion hijo2;
      std::uniform_real_distribution<double> probabilidad(0.0, 1.0);
      double num_aleatorio = probabilidad(gen_);
      if (num_aleatorio <= prob_cruce_) {
        std::pair<Solucion, Solucion> hijos = CruceOX(padre1, padre2);
        hijo1 = hijos.first;
        hijo2 = hijos.second;
      } else {
        hijo1 = padre1;
        hijo2 = padre2;
      }
      Mutacion(hijo1);
      Mutacion(hijo2);
      nueva_generacion.emplace_back(hijo1);
      nueva_generacion.emplace_back(hijo2);
    }
    Reemplazo(nueva_generacion);
  }
}