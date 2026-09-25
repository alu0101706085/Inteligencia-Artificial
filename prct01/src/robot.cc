#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <queue>
#include <deque>
#include <list>
#include "robot.h"

std::vector<Estado> Robot::AlgoritmoEstrella(std::string& mensaje) {
  std::priority_queue<Estado, std::vector<Estado>, std::greater<Estado>> lista_abierta;
  std::vector<std::vector<bool>> lista_cerrada(mapa_.GetNFilas(), std::vector<bool>(mapa_.GetNColumnas(), false));
  std::vector<std::pair<int, int>> cerrados_ordenados;
  std::vector<Estado*> historial; // Mantiene vivos los punteros de los nodos

  int iteracion = 0;

  Estado* origen = new Estado(pos_actual_.first, pos_actual_.second, 0, CalcularHeuristica(pos_actual_.first, pos_actual_.second));
  historial.push_back(origen);
  lista_abierta.push(*origen);

  mensaje += MostrarResultados(iteracion++, lista_abierta, cerrados_ordenados);
  SetNodosGenerados(0);
  SetNodosInspeccionados(0);

  std::vector<Estado> camino_final;

  while (!lista_abierta.empty()) {
    Estado estado_actual = lista_abierta.top();
    lista_abierta.pop();

    if (lista_cerrada[estado_actual.GetFila()][estado_actual.GetColumna()]) {
      continue;
    }

    lista_cerrada[estado_actual.GetFila()][estado_actual.GetColumna()] = true;
    cerrados_ordenados.push_back({estado_actual.GetFila(), estado_actual.GetColumna()});
    nodos_inspeccionados_++;

    // Obtener el puntero persistente correspondiente al estado actual
    Estado* estado_estable = nullptr;
    for (Estado* n : historial) {
      if (n->GetFila() == estado_actual.GetFila() && n->GetColumna() == estado_actual.GetColumna()) {
        estado_estable = n;
        break;
      }
    }

    std::pair<int, int> pos(estado_actual.GetFila(), estado_actual.GetColumna());
    if (pos == mapa_.GetDestino()) {
      camino_final = ReconstruirCamino(estado_estable);
      break;
    }

    std::vector<Estado> vecinos = ObtenerVecinosValidos(estado_estable);
    for (const Estado& vecino : vecinos) {
      if (!lista_cerrada[vecino.GetFila()][vecino.GetColumna()]) {
        Estado* nuevo_nodo = new Estado(vecino);
        historial.push_back(nuevo_nodo);
        lista_abierta.push(*nuevo_nodo);
        nodos_generados_++;
      }
    }
    mensaje += MostrarResultados(iteracion++, lista_abierta, cerrados_ordenados);
  }

  // Liberar memoria dinámica reservada
  for (Estado* n : historial) {
    delete n;
  }

  return camino_final;
}

std::vector<Estado> Robot::ObtenerVecinosValidos(Estado* estado_actual) {
  std::vector<Estado> vecinos_validos;
  int fila = estado_actual->GetFila();
  int columna = estado_actual->GetColumna();
  if (mapa_.EsMovimientoValido(fila + 1, columna)) {
    int coste_g = estado_actual->GetCosteG() + mapa_.ObtenerCoste(fila + 1, columna);
    int coste_h = CalcularHeuristica(fila + 1, columna);
    vecinos_validos.emplace_back(Estado(fila + 1, columna, coste_g, coste_h, estado_actual));
  } 
  if (mapa_.EsMovimientoValido(fila - 1, columna)) {
    int coste_g = estado_actual->GetCosteG() + mapa_.ObtenerCoste(fila - 1, columna);
    int coste_h = CalcularHeuristica(fila - 1, columna);
    vecinos_validos.emplace_back(Estado(fila - 1, columna, coste_g, coste_h, estado_actual));
  } 
  if (mapa_.EsMovimientoValido(fila, columna + 1)) {
    int coste_g = estado_actual->GetCosteG() + mapa_.ObtenerCoste(fila, columna + 1);
    int coste_h = CalcularHeuristica(fila, columna + 1);
    vecinos_validos.emplace_back(Estado(fila, columna + 1, coste_g, coste_h, estado_actual));
  } 
  if (mapa_.EsMovimientoValido(fila, columna - 1)) {
    int coste_g = estado_actual->GetCosteG() + mapa_.ObtenerCoste(fila, columna - 1);
    int coste_h = CalcularHeuristica(fila, columna - 1);
    vecinos_validos.emplace_back(Estado(fila, columna - 1, coste_g, coste_h, estado_actual));
  }
  return vecinos_validos;
}

int Robot::CalcularHeuristica(int fila, int columna) {
  std::pair<int, int> destino = mapa_.GetDestino();
  int distancia_rectilinea = std::abs(destino.first - fila) + std::abs(destino.second - columna);
  return 2 * distancia_rectilinea;
} 

std::vector<Estado> Robot::ReconstruirCamino(Estado* estado) {
  std::vector<Estado> camino_optimo;
  while(estado->GetEstadoAnterior() != nullptr) {
    // Nota: Incluir "*estado" obliga a ir a dicha direccion de memoria y devolver el valor alli guardado
    camino_optimo.emplace_back(*estado); 
    estado = estado->GetEstadoAnterior();
  }
  camino_optimo.emplace_back(*estado);
  std::reverse(camino_optimo.begin(), camino_optimo.end()); // Ordenar el camino final
  return camino_optimo;
}


std::string Robot::MostrarResultados(int iteracion, 
                                     std::priority_queue<Estado, std::vector<Estado>, std::greater<Estado>> copia_abiertos, 
                                     const std::vector<std::pair<int, int>>& cerrados) {
  std::string resultado = "Iteración " + std::to_string(iteracion);
  resultado += "\n";
  resultado += "-----------\n";
  // Imprimir Lista Abierta
  resultado += "Abiertos = ";
  bool primero = true;
  while (!copia_abiertos.empty()) {
    Estado e = copia_abiertos.top();
    copia_abiertos.pop();
    if (!primero) resultado += ", ";
    resultado += "(" + std::to_string(e.GetFila()) + "," + std::to_string(e.GetColumna()) + ")";
    primero = false;
  }
  resultado += "\n";
  // Imprimir Lista Cerrada
  resultado += "Cerrados = ";
  for (size_t i = 0; i < cerrados.size(); ++i) {
    if (i > 0) resultado += ", ";
    resultado += "(" + std::to_string(cerrados[i].first) + "," + std::to_string(cerrados[i].second) + ")";
  }
  resultado += "\n------------------------\n";
  return resultado;
}

void Robot::GuardarResultadosEnFicheros(std::string ruta_salida1, std::string ruta_salida2, std::string iteraciones, std::vector<Estado> camino) {
  std::ofstream FicheroSalida1(ruta_salida1);
  if (!FicheroSalida1) return;
  std::ofstream FicheroSalida2(ruta_salida2);
  if (!FicheroSalida2) return;
  std::vector<std::vector<bool>> es_camino(mapa_.GetNFilas(), std::vector<bool>(mapa_.GetNColumnas(), false));
  for (const auto& estado : camino) {
    es_camino[estado.GetFila()][estado.GetColumna()] = true;
  }
  for (int i{0}; i < mapa_.GetNFilas(); i++) {
    for (int j{0}; j < mapa_.GetNColumnas(); j++) {
      if (es_camino[i][j]) {
        FicheroSalida1 << "* ";
      } else {
        FicheroSalida1 << mapa_.GetMatriz()[i][j] << " ";
      }
    }
    FicheroSalida1 << "\n";
  }
  FicheroSalida2 << iteraciones;
  FicheroSalida1.close();
  FicheroSalida2.close();
}