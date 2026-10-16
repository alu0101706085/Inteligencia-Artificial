#ifndef INSTANCIA_PFSP_H
#define INSTANCIA_PFSP_H

#include <vector>
#include <string>

class InstanciaPFSP {
 public:
  InstanciaPFSP(const std::string& archivo);
  InstanciaPFSP(int n_tareas, int n_maquinas, std::vector<std::vector<int>> matriz) :
                n_tareas_(n_tareas), n_maquinas_(n_maquinas), matriz_tiempos_(matriz) {}
  int Get_NTareas() const { return n_tareas_; }
  int Get_NMaquinas() const { return n_maquinas_; }
  std::vector<std::vector<int>> Get_MatrizTiempos() const { return matriz_tiempos_; }
  int CalcularMakespan(const std::vector<int>& permutacion) const;
 private:
  int n_tareas_;
  int n_maquinas_;
  std::vector<std::vector<int>> matriz_tiempos_;
};

#endif