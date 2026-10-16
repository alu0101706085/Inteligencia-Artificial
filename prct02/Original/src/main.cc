#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cmath>

#include "algoritmoGenetico.h"
#include "AlgoritmosReferencia.h"
#include "instancia_PFSP.h"
#include "solucion.h"

struct Configuracion {
  InstanciaPFSP instancia;
  int upper_bound;
  int semilla_base;
  int N_tam_poblacion;
  int E_evaluaciones_max;
  int K_tam_torneo;
  double Pc_prob_cruce;
  double Pm_prob_mutacion;
};

Configuracion CargarDatos(int argc, char* argv[]) {
  if (argc != 7) {
    std::cout << "Error: Numero incorrecto de parámetros" << std::endl;
    std::cout << "Uso: programa.exe archivo N E K Pc Pm" << std::endl;
    exit(1);
  }
  int N = std::stoi(argv[2]);
  int E = std::stoi(argv[3]);
  int K = std::stoi(argv[4]);
  double Pc = std::stod(argv[5]);
  double Pm = std::stod(argv[6]);
  std::ifstream Fichero(argv[1]);
  if (!Fichero){
    std::cout << "No se pudo abrir el fichero de lectura" << std::endl;
    exit(1);
  }
  std::string linea_basura;
  std::getline(Fichero, linea_basura); 
  int n_trabajos, n_maquinas, semilla, upper_bound, lower_bound;
  Fichero >> n_trabajos >> n_maquinas >> semilla >> upper_bound >> lower_bound;
  std::getline(Fichero, linea_basura); // Este solo lee el final de la linea ('\n')
  std::getline(Fichero, linea_basura); 
  std::vector<std::vector<int>> tiempo_procesado(n_maquinas, std::vector<int>(n_trabajos));
  for (int i{0}; i < n_maquinas; i++) {
    for (int j{0}; j < n_trabajos; j++) {
      Fichero >> tiempo_procesado[i][j];
    }
  }
  InstanciaPFSP instancia(n_trabajos, n_maquinas, tiempo_procesado);
  Configuracion config{instancia, upper_bound, semilla, N, E, K, Pc, Pm};
  Fichero.close();
  return config;
}

int main(int argc, char* argv[]) {
  Configuracion datos = CargarDatos(argc, argv);
  AlgoritmosReferencias algoritmo_neh;
  Solucion CmaxNEH = algoritmo_neh.Alg_NEH(datos.instancia);
  std::vector<int> resultados_cmax;
  for (int i{0}; i < 10; i++) {
    std::mt19937 gen(datos.semilla_base + i);
    bool hacer_grafica = (i == 0);
    AlgoritmoGenetico genetico(datos.instancia, gen, datos.N_tam_poblacion, datos.Pc_prob_cruce, datos.Pm_prob_mutacion, 
                      datos.K_tam_torneo, datos.E_evaluaciones_max);
    if (hacer_grafica) {
      genetico.Ejecutar(hacer_grafica);
      std::system("python3 graficar_convergencia.py");
    } else {
      genetico.Ejecutar(hacer_grafica);
    }
    resultados_cmax.emplace_back(genetico.GetSolucion().Get_Makespan());
  }
  int BestCmax = 999999;
  int suma_elementos{0};
  for (const auto& elemento : resultados_cmax) {
    if (elemento < BestCmax) BestCmax = elemento;
    suma_elementos += elemento;
  }
  double promedio_cmax = suma_elementos / (double)resultados_cmax.size();
  double sumatoria{0.0};
  for (size_t i{0}; i < resultados_cmax.size(); i++) {
    sumatoria += pow(resultados_cmax[i] - promedio_cmax, 2);
  }
  double desviacion_estandar = sqrt(sumatoria / resultados_cmax.size());
  double rpd_ub = ((BestCmax - datos.upper_bound) / (double)datos.upper_bound) * 100.0;
  double rpd_neh = ((BestCmax - (double)CmaxNEH.Get_Makespan()) / (double)CmaxNEH.Get_Makespan()) * 100;
  std::ofstream FicheroSalida("tabla_resultados.csv", std::ios::app);
  FicheroSalida << argv[1] << "," << datos.instancia.Get_NTareas() << "," << datos.instancia.Get_NMaquinas() << "," << BestCmax << "," << promedio_cmax << "," << desviacion_estandar << "," << CmaxNEH.Get_Makespan() << "," << rpd_ub << "," << rpd_neh << "\n";
  FicheroSalida.close();
  std::system("python3 mostrar_tabla.py");
  return 0;
}