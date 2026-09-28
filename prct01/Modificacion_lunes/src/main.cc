#include <iostream>

#include "estado.h"
#include "mapa.h"
#include "robot.h"

/**
 * @brief Función principal que coordina la ejecución del programa.
 * 
 * Inicializa el mapa leyendo los datos desde el archivo proporcionado, configura 
 * el robot en la posición de origen e invoca el algoritmo de búsqueda A*. 
 * Finalmente, guarda los resultados generados (el mapa visual con la ruta y el log de 
 * iteraciones) en los ficheros de salida especificados por línea de comandos, 
 * y muestra el registro de iteraciones por la salida estándar.
 * 
 * @param argc Número de argumentos pasados por la línea de comandos.
 * @param argv Array de cadenas de texto con los argumentos. Se espera:
 *             - argv[1]: Ruta al archivo de texto con la matriz del mapa de entrada.
 *             - argv[2]: Ruta al archivo de salida para guardar el mapa con el camino (*).
 *             - argv[3]: Ruta al archivo de salida para el registro completo de iteraciones.
 * @return int Devuelve 0 al finalizar la ejecución correctamente.
 */
int main(int argc, char* argv[]) {
  Mapa mapa;
  mapa.CargarFichero(argv[1]);
  Robot robot(mapa.GetOrigen().first, mapa.GetOrigen().second, mapa);
  std::string iteraciones = "";
  std::vector<Estado> camino = robot.AlgoritmoEstrella(iteraciones);
  robot.GuardarResultadosEnFicheros(argv[2], argv[3], iteraciones, camino);
  std::cout << iteraciones;
  return 0;
}