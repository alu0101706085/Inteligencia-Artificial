#include "../include/algoritmoGenetico.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <utility>
#include <fstream>
#include <cassert>

#include "AlgoritmosReferencia.h"

/**
 * @brief Genera la población inicial del algoritmo genético.
 *
 * El primer individuo se construye con la heurística constructiva NEH
 * (Alg_NEH). El resto de individuos son permutaciones aleatorias de las
 * tareas, que se evalúan y se contabilizan en el número de evaluaciones del
 * makespan. Durante el proceso se actualiza la mejor solución encontrada
 * hasta el momento (@c solucion_).
 *
 * @note Se asume que la solución NEH ya viene evaluada.
 */
void AlgoritmoGenetico::InicializarPoblacion() {
  poblacion_.reserve(tam_poblacion_);
  poblacion_.push_back(AlgoritmosReferencias::Alg_NEH(instancia_));
  solucion_ = poblacion_.front();
  n_evaluaciones_makespan_++;
  for (int i{1}; i < tam_poblacion_ && n_evaluaciones_makespan_ < presupuesto_max_; i++) {
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

/**
 * @brief Selecciona un individuo de la población mediante torneo.
 *
 * Se eligen @c tam_torneo_ individuos al azar (con reemplazo) de la población
 * y se devuelve el mejor de ellos, es decir, el de menor makespan.
 *
 * @return Copia del individuo ganador del torneo.
 */
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

/**
 * @brief Construye la permutación de un hijo mediante el cruce OX
 *        (Order Crossover).
 *
 * Copia en el hijo el segmento de @p solucion1 comprendido entre las
 * posiciones @p num1 y @p num2 (ambas incluidas). Las posiciones restantes se
 * rellenan, de izquierda a derecha, con las tareas de @p solucion2 en el orden
 * en que aparecen, omitiendo las que ya han sido copiadas del segmento.
 *
 * @param solucion1 Padre que aporta el segmento central.
 * @param solucion2 Padre que aporta el orden del resto de tareas.
 * @param num1 Primera posición de corte.
 * @param num2 Segunda posición de corte (si es menor que @p num1 se
 *             intercambian).
 * @return Permutación resultante del hijo.
 */
static std::vector<int> ObtenerPermutacionHijo(const Solucion& solucion1, const Solucion& solucion2, int num1, int num2) {
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

/**
 * @brief Realiza el cruce OX entre dos soluciones padre.
 *
 * Se eligen dos puntos de corte aleatorios y se generan dos hijos: el primero
 * toma el segmento de @p solucion1 y completa con @p solucion2, y el segundo
 * hace lo contrario.
 *
 * @param solucion1 Primer padre.
 * @param solucion2 Segundo padre.
 * @return Pareja de hijos generados.
 *
 * @note Los hijos devueltos no están evaluados; la evaluación se realiza en
 *       Reemplazo().
 */
std::pair<Solucion, Solucion> AlgoritmoGenetico::CruceOX(const Solucion& solucion1, const Solucion& solucion2) {
  std::uniform_int_distribution<int> lista_num(0, solucion1.Get_Permutacion().size() - 1);
  int num1 = lista_num(gen_);
  int num2 = lista_num(gen_);
  std::vector<int> hijo1 = ObtenerPermutacionHijo(solucion1, solucion2, num1, num2);
  std::vector<int> hijo2 = ObtenerPermutacionHijo(solucion2, solucion1, num1, num2);
  Solucion opcion_1(hijo1);
  Solucion opcion_2(hijo2);
  std::pair<Solucion, Solucion> solucion = {opcion_1, opcion_2};
  assert(opcion_1.EsFactible(instancia_.Get_NTareas()) && opcion_2.EsFactible(instancia_.Get_NTareas()));
  return solucion;
}

/**
 * @brief Aplica el operador de mutación por inserción a una solución.
 *
 * Con probabilidad @c prob_mutacion_ se elige una tarea en una posición
 * aleatoria (origen), se extrae de la permutación y se inserta en otra
 * posición distinta (destino). En caso contrario la solución no se modifica.
 *
 * @param[in,out] solucion Solución a mutar.
 *
 * @note La solución mutada no se reevalúa aquí; se hace en Reemplazo().
 */
void AlgoritmoGenetico::Mutacion(Solucion& solucion) {
  std::uniform_real_distribution<double> probabilidad(0.0, 1.0);
  double valor_aleatorio = probabilidad(gen_);
  if (valor_aleatorio > prob_mutacion_) return;
  else {
    std::uniform_int_distribution<int> distribucion(0, solucion.Get_Permutacion().size() - 1);
    int origen = distribucion(gen_);
    int destino = distribucion(gen_);
    while (origen == destino) {
      destino = distribucion(gen_);
    }
    std::vector<int> permutacion_mutada = solucion.Get_Permutacion();
    int tarea_origen = permutacion_mutada[origen];
    permutacion_mutada.erase(permutacion_mutada.begin() + origen);
    permutacion_mutada.insert(permutacion_mutada.begin() + destino, tarea_origen);
    solucion.Set_Permutacion(permutacion_mutada);
    assert(solucion.EsFactible(instancia_.Get_NTareas()));
  }
}

/**
 * @brief Reemplaza la población actual por la nueva generación (con elitismo).
 *
 * Evalúa cada individuo de la nueva generación, actualiza la mejor solución
 * global y sustituye la población. Después, para conservar el mejor individuo
 * encontrado (elitismo), el peor individuo de la nueva población se sustituye
 * por la mejor solución global.
 *
 * @param lista Nueva generación de individuos (se recibe por copia).
 */
void AlgoritmoGenetico::Reemplazo(std::vector<Solucion> lista) {
  for (auto& elemento : lista) {
    if (elemento.Get_Makespan() == -1) { 
      elemento.Evaluar(instancia_);
      n_evaluaciones_makespan_++;
    }
    if (elemento < solucion_) solucion_ = elemento;
  }
  poblacion_ = std::move(lista);
  int indice_peor_individuo{0};
  for (size_t i{0}; i < poblacion_.size(); i++) {
    if (poblacion_[indice_peor_individuo] < poblacion_[i]) indice_peor_individuo = i;
  }
  poblacion_[indice_peor_individuo] = solucion_;
}

/**
 * @brief Ejecuta el algoritmo genético completo.
 *
 * Inicializa la población y, mientras no se agote el presupuesto de
 * evaluaciones (@c presupuesto_max_), genera nuevas generaciones mediante:
 *  -# Selección por torneo de dos padres.
 *  -# Cruce OX con probabilidad @c prob_cruce_ (si no hay cruce, los hijos
 *     son copias de los padres).
 *  -# Mutación de ambos hijos.
 *  -# Reemplazo generacional con elitismo.
 *
 * @param guardar_convergencia Si es @c true, se escribe en el fichero
 *        @c convergencia.csv una línea por generación con el formato
 *        <tt>n_evaluaciones,mejor_makespan</tt>.
 */
void AlgoritmoGenetico::Ejecutar(bool guardar_convergencia) {
  InicializarPoblacion();
  std::ofstream fichero_grafica;
  if (guardar_convergencia) fichero_grafica.open("convergencia.csv");

  while( n_evaluaciones_makespan_ < presupuesto_max_) {
    std::vector<Solucion> nueva_generacion;
    nueva_generacion.reserve(tam_poblacion_);

    while (nueva_generacion.size() < tam_poblacion_ && n_evaluaciones_makespan_ < presupuesto_max_) {
      Solucion padre1 = SeleccionTorneo();
      Solucion padre2 = SeleccionTorneo();
      Solucion hijo1, hijo2;
      std::uniform_real_distribution<double> probabilidad(0.0, 1.0);
      
      if (probabilidad(gen_) <= prob_cruce_) {
        std::pair<Solucion, Solucion> hijos = CruceOX(padre1, padre2);
        hijo1 = hijos.first; hijo2 = hijos.second;
      } else {
        hijo1 = padre1; hijo2 = padre2;
      }

      Mutacion(hijo1);
      // Obligatorio evaluar el hijo para poder aplicar Búsqueda Local (M4)
      if (hijo1.Get_Makespan() == -1) {
          hijo1.Evaluar(instancia_);
          n_evaluaciones_makespan_++;
      }
      if (n_evaluaciones_makespan_ < presupuesto_max_) BusquedaLocalIntercambio(hijo1);
      nueva_generacion.emplace_back(hijo1);

      if (nueva_generacion.size() < tam_poblacion_ && n_evaluaciones_makespan_ < presupuesto_max_) {
        Mutacion(hijo2);
        if (hijo2.Get_Makespan() == -1) {
            hijo2.Evaluar(instancia_);
            n_evaluaciones_makespan_++;
        }
        if (n_evaluaciones_makespan_ < presupuesto_max_) BusquedaLocalIntercambio(hijo2);
        nueva_generacion.emplace_back(hijo2);
      }
    }
    Reemplazo(std::move(nueva_generacion));
    if (guardar_convergencia) {
      fichero_grafica << n_evaluaciones_makespan_ << "," << GetSolucion().Get_Makespan() << "\n";
    }
  }
  if (guardar_convergencia) fichero_grafica.close();
}

/**
 * @brief Aplica la búsqueda local por intercambio a una solución (M4).
 */
void AlgoritmoGenetico::BusquedaLocalIntercambio(Solucion& solucion) {
  // Regla estricta: si Pls es 0, abortar sin gastar un número aleatorio
  if (prob_busqueda_local_ == 0.0) return; 

  std::uniform_real_distribution<double> probabilidad(0.0, 1.0);
  if (probabilidad(gen_) > prob_busqueda_local_) return;

  int n = instancia_.Get_NTareas();
  std::uniform_int_distribution<int> distribucion(0, n - 1);
  int i = distribucion(gen_); // 1. Elegir posición al azar

  std::vector<int> perm_original = solucion.Get_Permutacion();
  int mejor_makespan = solucion.Get_Makespan();
  std::vector<int> mejor_perm = perm_original;

  // 2. Intercambiar con cada una de las otras n-1 posiciones
  for (int k = 0; k < n; k++) {
    if (n_evaluaciones_makespan_ >= presupuesto_max_) break; // Control estricto de presupuesto
    if (k == i) continue; // La original no se re-evalúa

    std::vector<int> nueva_perm = perm_original;
    std::swap(nueva_perm[i], nueva_perm[k]); // Intercambio (swap)

    Solucion temp(nueva_perm);
    temp.Evaluar(instancia_);
    n_evaluaciones_makespan_++;

    // 3. Sustituir por el menor. El operador '<' estricto garantiza que:
    // - En empate con h, no se actualiza (conserva h).
    // - En empate entre nuevas, se conserva la primera encontrada (menor índice k).
    if (temp.Get_Makespan() < mejor_makespan) {
      mejor_makespan = temp.Get_Makespan();
      mejor_perm = nueva_perm;
    }
  }
  
  solucion.Set_Permutacion(mejor_perm);
  solucion.Set_Makespan(mejor_makespan);
  assert(solucion.EsFactible(instancia_.Get_NTareas()));
}

/**
 * @brief Ejecuta el ejemplo del PDF (Punto 4) para verificar la búsqueda local por intercambio.
 */
void AlgoritmoGenetico::EjecutarEjemploComprobacionM4() {
  std::cout << "\n=== EJEMPLO DE COMPROBACION M4 (Búsqueda Local por Intercambio) ===\n";
  std::vector<std::vector<int>> matriz = {
      {6, 4, 2, 7},
      {1, 3, 7, 7},
      {1, 1, 7, 1}
  };
  InstanciaPFSP instancia_prueba(4, 3, matriz);

  Solucion h({1, 2, 3, 4});
  h.Evaluar(instancia_prueba);
  std::cout << "Hijo original (h): (1,2,3,4) con Cmax = " << h.Get_Makespan() << "\n";
  std::cout << "Se elige forzadamente la posición i=1 (trabajo J1)\n\n";

  int n = 4;
  int i = 0; // Índice 0 (Posición 1 en base 1)
  std::vector<int> perm_original = h.Get_Permutacion();
  int mejor_makespan = h.Get_Makespan();
  std::vector<int> mejor_perm = perm_original;

  std::cout << "Posición k\tSecuencia\tCmax\t¿Se evalúa?\n";
  for (int k = 0; k < n; k++) {
    if (k == i) {
        std::cout << k + 1 << "\t\t(";
        for(int m=0; m<n; m++) std::cout << perm_original[m] << (m==n-1?"":",");
        std::cout << ")\t" << mejor_makespan << "\tNo (es h)\n";
        continue;
    }
    std::vector<int> nueva_perm = perm_original;
    std::swap(nueva_perm[i], nueva_perm[k]);

    Solucion temp(nueva_perm);
    temp.Evaluar(instancia_prueba);

    std::cout << k + 1 << "\t\t(";
    for(int m=0; m<n; m++) std::cout << nueva_perm[m] << (m==n-1?"":",");
    std::cout << ")\t" << temp.Get_Makespan() << "\tSí\n";

    if (temp.Get_Makespan() < mejor_makespan) {
      mejor_makespan = temp.Get_Makespan();
      mejor_perm = nueva_perm;
    }
  }
  h.Set_Permutacion(mejor_perm);
  h.Set_Makespan(mejor_makespan);

  std::cout << "\nEl operador devuelve h*: (";
  for (int idx = 0; idx < 4; idx++) std::cout << h.Get_Permutacion()[idx] << (idx == 3 ? "" : ",");
  std::cout << ") Cmax = " << h.Get_Makespan() << "\n";
  std::cout << "Es factible: " << (h.EsFactible(4) ? "Si" : "No") << "\n";
  std::cout << "===================================================================\n\n";
}