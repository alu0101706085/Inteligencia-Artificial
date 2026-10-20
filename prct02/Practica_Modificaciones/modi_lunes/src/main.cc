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
  double Pls_prob_busqueda_local; // <-- NUEVO
};

Configuracion CargarDatos(int argc, char* argv[]) {
  if (argc != 8 && argc != 9) {
    std::cout << "Error: Numero incorrecto de parámetros\n";
    exit(1);
  }
  int N = std::stoi(argv[2]);
  int E = std::stoi(argv[3]);
  int K = std::stoi(argv[4]);
  double Pc = std::stod(argv[5]);
  double Pm = std::stod(argv[6]);
  double Pls = std::stod(argv[7]);

  std::ifstream Fichero(argv[1]);
  if (!Fichero){
    std::cout << "No se pudo abrir el fichero de lectura\n";
    exit(1);
  }
  
  int n_trabajos, n_maquinas;
  Fichero >> n_trabajos >> n_maquinas; // Lee el "20 10" directamente
  
  std::vector<std::vector<int>> tiempo_procesado(n_maquinas, std::vector<int>(n_trabajos));
  for (int i{0}; i < n_maquinas; i++) {
    for (int j{0}; j < n_trabajos; j++) {
      Fichero >> tiempo_procesado[i][j]; // Rellena la matriz directamente
    }
  }
  
  // Ponemos valores por defecto porque ya no vienen en el archivo
  int upper_bound = 1200; // Cota inventada para evitar division por cero en rpd_ub
  int semilla = (argc == 9) ? std::stoi(argv[8]) : 42; 
  
  InstanciaPFSP instancia(n_trabajos, n_maquinas, tiempo_procesado);
  Configuracion config{instancia, upper_bound, semilla, N, E, K, Pc, Pm, Pls};
  Fichero.close();
  return config;
}

/**
 * @brief Ejecuta el ejemplo del PDF (Punto 4) para verificar la búsqueda local.
 */
void EjecutarEjemploComprobacion() {
  std::cout << "\n=== EJEMPLO DE COMPROBACION (Sección 4) ===\n";
  std::vector<std::vector<int>> matriz = {
      {6, 4, 2, 7},
      {1, 3, 7, 7},
      {1, 1, 7, 1}
  };
  InstanciaPFSP instancia_prueba(4, 3, matriz);
  std::mt19937 gen_prueba(42);
  AlgoritmoGenetico ag_prueba(instancia_prueba, gen_prueba, 10, 0.0, 0.0, 0.0, 2, 50000);

  Solucion h({1, 2, 3, 4});
  h.Evaluar(instancia_prueba);
  std::cout << "Hijo original (h): (1,2,3,4) con Cmax = " << h.Get_Makespan() << "\n\n";

  ag_prueba.BusquedaLocalForzada(h, 0); // Forzamos i=0 (J1)

  std::cout << "\nOperador devuelve h*: (";
  for (int i = 0; i < 4; i++) std::cout << h.Get_Permutacion()[i] << (i == 3 ? "" : ",");
  std::cout << ") Cmax = " << h.Get_Makespan() << "\n";
  std::cout << "Es factible: " << (h.EsFactible(4) ? "Si" : "No") << "\n";
  std::cout << "===========================================\n\n";
}

int main(int argc, char* argv[]) {
  // Ejecutamos y mostramos la traza obligatoria
  EjecutarEjemploComprobacion();

  Configuracion datos = CargarDatos(argc, argv);
  AlgoritmosReferencias algoritmo_neh;
  Solucion CmaxNEH = algoritmo_neh.Alg_NEH(datos.instancia);
  std::vector<int> resultados_cmax;
  double tiempo_total{0.0};
  
  for (int i{0}; i < 10; i++) {
    std::mt19937 gen(datos.semilla_base + i);
    bool hacer_grafica = (i == 0);
    
    // <-- AÑADIDO: datos.Pls_prob_busqueda_local
    AlgoritmoGenetico genetico(datos.instancia, gen, datos.N_tam_poblacion, 
                               datos.Pc_prob_cruce, datos.Pm_prob_mutacion, 
                               datos.Pls_prob_busqueda_local, 
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