#include <fstream>
#include <sstream>

#include "mapa.h"

void Mapa::SetOrigen(const int fila, const int columna) {
  origen_.first = fila;
  origen_.second = columna;
}

void Mapa::SetDestino(const int fila, const int columna) {
  destino_.first = fila;
  destino_.second = columna;
}

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