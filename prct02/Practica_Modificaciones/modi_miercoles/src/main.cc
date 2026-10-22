#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <chrono>

#include "../include/algoritmoGenetico.h"
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
  int tipo_seleccion; // <-- NUEVO M3
  double presion_selectiva; // <-- NUEVO M3
};

Configuracion CargarDatos(int argc, char* argv[]) {
  // Ahora esperamos 9 o 10 argumentos
  if (argc != 9 && argc != 10) {
    std::cout << "Error: Numero incorrecto de parametros\n";
    std::cout << "Uso: programa.exe archivo N E K Pc Pm TipoSel(0=Torneo,1=Ranking) Presion_S [semilla]\n";
    exit(1);
  }
  int N = std::stoi(argv[2]);
  int E = std::stoi(argv[3]);
  int K = std::stoi(argv[4]);
  double Pc = std::stod(argv[5]);
  double Pm = std::stod(argv[6]);
  int TipoSel = std::stoi(argv[7]); // <-- NUEVO M3
  double PresionS = std::stod(argv[8]); // <-- NUEVO M3

  // USO LA OPCION 1 DE LECTURA (Carga cruda) PARA QUE FUNCIONE CON TU INSTANCIA
  std::ifstream Fichero(argv[1]);
  if (!Fichero){
    std::cout << "No se pudo abrir el fichero de lectura\n";
    exit(1);
  }
  
  int n_trabajos, n_maquinas;
  Fichero >> n_trabajos >> n_maquinas;
  
  std::vector<std::vector<int>> tiempo_procesado(n_maquinas, std::vector<int>(n_trabajos));
  for (int i{0}; i < n_maquinas; i++) {
    for (int j{0}; j < n_trabajos; j++) {
      Fichero >> tiempo_procesado[i][j];
    }
  }
  
  int upper_bound = 1200; // Fake upper bound
  int semilla = (argc == 10) ? std::stoi(argv[9]) : 42; 
  
  InstanciaPFSP instancia(n_trabajos, n_maquinas, tiempo_procesado);
  Configuracion config{instancia, upper_bound, semilla, N, E, K, Pc, Pm, TipoSel, PresionS};
  Fichero.close();
  return config;
}

int main(int argc, char* argv[]) {
  // <-- Ejecución de prueba obligatoria del pdf
  AlgoritmoGenetico::EjecutarEjemploComprobacionRanking();

  Configuracion datos = CargarDatos(argc, argv);
  AlgoritmosReferencias algoritmo_neh;
  Solucion CmaxNEH = algoritmo_neh.Alg_NEH(datos.instancia);
  std::vector<int> resultados_cmax;
  double tiempo_total{0.0};
  
  for (int i{0}; i < 10; i++) {
    std::mt19937 gen(datos.semilla_base + i);
    bool hacer_grafica = (i == 0);
    
    // Pasamos los nuevos parámetros
    AlgoritmoGenetico genetico(datos.instancia, gen, datos.N_tam_poblacion, 
                               datos.Pc_prob_cruce, datos.Pm_prob_mutacion, 
                               datos.tipo_seleccion, datos.presion_selectiva, // <-- NUEVO M3
                               datos.K_tam_torneo, datos.E_evaluaciones_max);
    auto inicio = std::chrono::steady_clock::now();
    genetico.Ejecutar(hacer_grafica);
    tiempo_total += std::chrono::duration<double>(std::chrono::steady_clock::now() - inicio).count();
    
    if (hacer_grafica) {
      std::string comando = std::string("python3 graficar_convergencia.py \"") + argv[1] + "\"";
      int r1 = std::system(comando.c_str()); (void)r1;
    }
    resultados_cmax.emplace_back(genetico.GetSolucion().Get_Makespan());
  }
  
  // (El resto del main para estadísticas y script python sigue intacto)
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
  double rpd_ub = (((double)BestCmax - datos.upper_bound) / (double)datos.upper_bound) * 100.0;
  double rpd_neh = ((BestCmax - (double)CmaxNEH.Get_Makespan()) / (double)CmaxNEH.Get_Makespan()) * 100;
  
  std::ofstream FicheroSalida("tabla_resultados.csv", std::ios::app);
  FicheroSalida << argv[1] << "," << datos.instancia.Get_NTareas() << "," << datos.instancia.Get_NMaquinas() << "," << BestCmax 
                << "," << promedio_cmax << "," << desviacion_estandar << "," << CmaxNEH.Get_Makespan() << "," << rpd_ub 
                << "," << rpd_neh << "," << tiempo_total << "\n";
  FicheroSalida.close();
  
  int r2 = std::system("python3 mostrar_tabla.py"); (void)r2;
  return 0;
}