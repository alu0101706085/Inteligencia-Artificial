#include "instancia_PFSP.h"

#include <fstream>
#include <algorithm>
#include <stdexcept>

InstanciaPFSP::InstanciaPFSP(const std::string& archivo) {
  std::ifstream FicheroLectura(archivo);
  if (!FicheroLectura.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo");
  }
  FicheroLectura >> n_tareas_ >> n_maquinas_;
  matriz_tiempos_.assign(n_maquinas_, std::vector<int>(n_tareas_));
  for (int i{0}; i < n_maquinas_; i++) {
    for (int j{0}; j < n_tareas_; j++) {
      FicheroLectura >> matriz_tiempos_[i][j];
    }
  }
}

int InstanciaPFSP::CalcularMakespan(const std::vector<int>& permutacion) const {
  std::vector<int> TiemposLibres(n_maquinas_, 0);
  for (int k{0}; k < n_tareas_; k++) {
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