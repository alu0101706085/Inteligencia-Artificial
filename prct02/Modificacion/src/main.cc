#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <chrono>

#include "algoritmoGenetico.h"
#include "AlgoritmosReferencia.h"
#include "instancia_PFSP.h"
#include "solucion.h"

/**
 * @struct Configuracion
 * @brief Agrupa la instancia del problema y los parámetros de ejecución del
 *        algoritmo genético.
 */
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

/**
 * @brief Lee los parámetros de la línea de comandos y la instancia del fichero.
 *
 * Los argumentos esperados son: @c archivo N E K Pc Pm [semilla]. El fichero de la
 * instancia tiene una primera línea de cabecera que se ignora, una segunda con
 * número de trabajos, número de máquinas, semilla, cota superior y cota
 * inferior, una tercera línea que se ignora y, por último, la matriz de
 * tiempos (una fila por máquina y una columna por trabajo).
 *
 * @param argc Número de argumentos de la línea de comandos.
 * @param argv Argumentos de la línea de comandos.
 * @return Configuración con la instancia y los parámetros del algoritmo.
 *
 * @note Si el número de argumentos es incorrecto o el fichero no se puede
 *       abrir, se muestra un mensaje de error y el programa termina con
 *       @c exit(1).
 */
Configuracion CargarDatos(int argc, char* argv[]) {
  if (argc != 7 && argc != 8) {
    std::cout << "Error: Numero incorrecto de parámetros" << std::endl;
    std::cout << "Uso: programa.exe archivo N E K Pc Pm [semilla]" << std::endl;
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
  Configuracion config{instancia, upper_bound, (argc == 8 ? std::stoi(argv[7]) : semilla), N, E, K, Pc, Pm};
  Fichero.close();
  return config;
}

/**
 * @brief Punto de entrada del programa.
 *
 * Realiza los siguientes pasos:
 *  -# Carga la instancia y los parámetros (CargarDatos()).
 *  -# Calcula la solución de referencia con la heurística NEH.
 *  -# Ejecuta 10 veces el algoritmo genético con semillas consecutivas. En la
 *     primera ejecución se guarda la convergencia y se genera la gráfica con
 *     el script @c graficar_convergencia.py.
 *  -# Calcula las estadísticas de los 10 resultados: mejor makespan, media y
 *     desviación estándar, así como el RPD respecto a la cota superior y
 *     respecto a NEH.
 *  -# Añade una línea con los resultados a @c tabla_resultados.csv y muestra
 *     la tabla con el script @c mostrar_tabla.py.
 *
 * Las desviaciones relativas se calculan como
 * RPD = (BestCmax - referencia) / referencia * 100.
 *
 * @param argc Número de argumentos de la línea de comandos.
 * @param argv Argumentos: <tt>archivo N E K Pc Pm</tt>.
 * @return 0 si la ejecución termina correctamente.
 */
int main(int argc, char* argv[]) {
  Configuracion datos = CargarDatos(argc, argv);
  AlgoritmosReferencias algoritmo_neh;
  Solucion CmaxNEH = algoritmo_neh.Alg_NEH(datos.instancia);
  std::vector<int> resultados_cmax;
  double tiempo_total{0.0};
  for (int i{0}; i < 10; i++) {
    std::mt19937 gen(datos.semilla_base + i);
    bool hacer_grafica = (i == 0);
    AlgoritmoGenetico genetico(datos.instancia, gen, datos.N_tam_poblacion, datos.Pc_prob_cruce, datos.Pm_prob_mutacion, 
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