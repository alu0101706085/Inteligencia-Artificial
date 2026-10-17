#include "instancia_PFSP.h"

#include <fstream>
#include <algorithm>
#include <stdexcept>

/**
 * @brief Calcula el makespan de una permutación de tareas.
 *
 * Simula el paso de las tareas, en el orden indicado, por todas las máquinas.
 * Se mantiene para cada máquina el instante en que queda libre: una tarea
 * empieza en una máquina cuando ésta está libre y la tarea ha terminado en la
 * máquina anterior, es decir, en el máximo de ambos instantes. El makespan es
 * el instante en que la última tarea termina en la última máquina.
 *
 * @param permutacion Orden de procesamiento de las tareas (ids de 1 a n).
 * @return Makespan de la permutación (tiempo total de finalización).
 *
 * @note Complejidad temporal O(n · m), con n tareas y m máquinas.
 */
int InstanciaPFSP::CalcularMakespan(const std::vector<int>& permutacion) const {
  std::vector<int> TiemposLibres(n_maquinas_, 0);
  for (size_t k{0}; k < permutacion.size(); k++) {
    int indice = permutacion[k] - 1;
    TiemposLibres[0] = TiemposLibres[0] + matriz_tiempos_[0][indice]; // Primera máquina i = 0;
    for (int i{1}; i < n_maquinas_; i++) { // Resto de máquinas (hasta m - 1)
      int tiempo_anterior_maquina = TiemposLibres[i - 1];
      int tiempo_actual_maquina = TiemposLibres[i];
      TiemposLibres[i] = std::max(tiempo_actual_maquina, tiempo_anterior_maquina) + matriz_tiempos_[i][indice];
    }
  }
  return TiemposLibres[n_maquinas_ - 1];
}