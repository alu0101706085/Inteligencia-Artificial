#include <fstream>
#include <sstream>

#include "mapa.h"

/**
 * @brief Establece las coordenadas del punto de origen en el mapa.
 */
void Mapa::SetOrigen(const int fila, const int columna) {
  origen_.first = fila;
  origen_.second = columna;
}

/**
 * @brief Establece las coordenadas del punto de destino en el mapa.
 */
void Mapa::SetDestino(const int fila, const int columna) {
  destino_.first = fila;
  destino_.second = columna;
}

/**
 * @brief Carga la configuración del mapa a partir de un fichero de texto.
 * 
 * Lee el archivo línea por línea e inicializa la matriz del entorno. Además, detecta 
 * de manera automática el punto de origen (identificado por el valor 0) y el punto 
 * de destino (identificado por el valor 10), configurando sus coordenadas respectivas.
 * 
 * @param fichero Ruta al archivo de texto que contiene los datos del mapa.
 * @return true Si el archivo se abrió y cargó con éxito.
 * @return false Si hubo un error al intentar abrir o leer el archivo.
 */
bool Mapa::CargarFichero(const std::string& fichero) {
  std::ifstream fichero_lectura(fichero);
  if (!fichero_lectura) return false;
  std::string linea;
  int num_linea{0};
  while(std::getline(fichero_lectura, linea)) {
    if (linea.empty()) continue; // Evita procesar líneas vacías accidentales
    std::stringstream flujo(linea);
    int num_columna{0};
    std::vector<int> linea_matriz;
    int valor{0};
    while (flujo >> valor) {
      linea_matriz.emplace_back(valor);
      if (valor == 10) {
        SetDestino(num_linea, num_columna);
      } else if (valor == 0) {
        SetOrigen(num_linea, num_columna);
      }
      num_columna++;
    }

    // Solo guardamos si la fila contenía números
    if (!linea_matriz.empty()) {
      GuardarEnMatriz(linea_matriz);
      SetNColumnas(num_columna);
      num_linea++;
    }
  }
  SetNFilas(num_linea);
  return true;
}

/**
 * @brief Comprueba si unas coordenadas específicas se encuentran dentro de los límites del mapa.
 * 
 * @return true Si la posición es válida y está dentro de las dimensiones de la matriz.
 * @return false Si la posición está fuera de los límites del mapa (ej. coordenadas negativas o mayores al tamaño).
 */
bool Mapa::EsMovimientoValido(int fila, int columna) {
  if (fila < 0 || fila >= GetNFilas() || columna < 0 || columna >= GetNColumnas()) {
    return false;
  }
  return true;
}

int Mapa::ObtenerCoste(int fila, int columna) {
  int coste = matriz_[fila][columna];
  // Manejo especial de la celda de destino
  if (coste == 10) {
    return 2; 
  }
  return coste;
}

/**
 * @brief Devuelve el coste de transitar hacia una celda en concreto.
 * 
 * Extrae el valor numérico de la matriz en las coordenadas indicadas. Si la celda 
 * es la de destino (valor original 10), realiza un manejo especial devolviendo un coste efectivo de 2.
 * 
 * @return int El coste de movimiento asociado a la celda.
 */
void Mapa::GuardarFichero(const std::string& fichero, const std::vector<Estado>& camino) {
  std::ofstream fichero_salida(fichero);
  if (!fichero_salida) return;
  int num_filas{0};
  for (const auto& filas : matriz_) {
    int num_columnas{0};
    for (int elemento : filas) {
      bool es_camino = false;
      // Comprobamos si las coordenadas actuales coinciden con algún paso del camino
      for (const auto& estado : camino) {
        if (num_filas == estado.GetFila() && num_columnas == estado.GetColumna()) {
          es_camino = true;
          break; // Si ya lo encontramos, no hace falta seguir buscando
        }
      }
      // Imprimimos un solo carácter por celda
      if (es_camino) {
        fichero_salida << "* ";
      } else {
        fichero_salida << elemento << " ";
      }
      num_columnas++;
    }
    fichero_salida << "\n";
    num_filas++;
  }
}