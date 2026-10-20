#include "solucion.h"

#include <utility>

/**
 * @brief Evalúa la solución calculando su makespan.
 *
 * El cálculo solo se realiza si la solución no ha sido evaluada antes (es
 * decir, si el makespan vale -1). En caso contrario se devuelve el valor ya
 * almacenado, evitando recalcularlo.
 *
 * @param instancia Instancia del PFSP sobre la que se evalúa la permutación.
 * @return Makespan de la solución.
 *
 * @note Se usa -1 como valor centinela de "solución sin evaluar". Si se
 *       modifica la permutación con Set_Permutacion(), hay que reiniciar el
 *       makespan a -1 para que se vuelva a calcular.
 */
int Solucion::Evaluar(const InstanciaPFSP& instancia) {
  if (Get_Makespan() == -1) {
    makespan_ = instancia.CalcularMakespan(Get_Permutacion());
    return Get_Makespan();
  }
  return Get_Makespan();
}

/**
 * @brief Compara dos soluciones según su makespan.
 *
 * Una solución es "menor" que otra si su makespan es menor, es decir, si es
 * mejor para el problema (minimización).
 *
 * @param otro Solución con la que se compara.
 * @return @c true si el makespan de esta solución es estrictamente menor que
 *         el de @p otro; @c false en caso contrario.
 *
 * @note Las dos soluciones deben estar evaluadas para que la comparación
 *       tenga sentido.
 */
bool Solucion::operator<(const Solucion& otro) const {
  return makespan_ < otro.makespan_;
}

/**
 * @brief Comprueba que la permutación es factible.
 *
 * Una permutación es factible si contiene exactamente los trabajos 1..n,
 * sin repetidos ni ausentes.
 *
 * @param n_tareas Número de trabajos de la instancia.
 * @return @c true si la permutación es válida; @c false en caso contrario.
 */
bool Solucion::EsFactible(int n_tareas) const {
  if (static_cast<int>(permutacion_.size()) != n_tareas) return false;
  std::vector<bool> vista(n_tareas + 1, false);
  for (int tarea : permutacion_) {
    if (tarea < 1 || tarea > n_tareas || vista[tarea]) return false;
    vista[tarea] = true;
  }
  return true;
}