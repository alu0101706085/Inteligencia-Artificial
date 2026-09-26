#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <queue>
#include <deque>
#include <list>

#include "robot.h"

/**
 * @brief Ejecuta el algoritmo de búsqueda A* (A-Estrella) para encontrar el camino óptimo.
 * 
 * Explora el mapa utilizando listas de nodos abiertos y cerrados, evaluando el coste 
 * real (G) y la estimación heurística (H) para llegar al destino.
 * 
 * @param mensaje Referencia a una cadena de texto donde se concatenará el registro de iteraciones, el camino final y el coste total.
 * @return std::vector<Estado> Vector que contiene la secuencia de estados del camino óptimo desde el origen hasta el destino. Retorna un vector vacío si no se encuentra solución.
 */
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

  if (!camino_final.empty()) {
    mensaje += "Camino: ";
    for (size_t i = 0; i < camino_final.size(); ++i) {
      int x_camino = camino_final[i].GetColumna() + 1;
      int y_camino = mapa_.GetNFilas() - camino_final[i].GetFila();
      mensaje += "(" + std::to_string(x_camino) + "," + std::to_string(y_camino) + ")";
      
      if (i < camino_final.size() - 1) {
        mensaje += " -> ";
      }
    }
    // El coste final es el coste G del último nodo del camino
    mensaje += "\nCoste: " + std::to_string(camino_final.back().GetCosteG()) + "\n";
  }

  // Liberar memoria dinámica reservada
  for (Estado* n : historial) {
    delete n;
  }

  return camino_final;
}

/**
 * @brief Obtiene los estados vecinos válidos a los que el robot se puede mover.
 * 
 * Comprueba los movimientos posibles (arriba, abajo, derecha, izquierda). Para cada 
 * movimiento válido, genera un nuevo estado calculando su coste acumulado (G) y su heurística (H).
 * 
 * @return std::vector<Estado> Vector con los estados vecinos válidos y transitables.
 */
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

/**
 * @brief Calcula el valor heurístico desde una coordenada dada hasta el destino.
 * 
 * Emplea el cálculo de la distancia rectilínea entre la posición actual y la
 * posición final, multiplicada por un factor de 2.
 * 
 * @return int Valor estimado de la heurística.
 */
int Robot::CalcularHeuristica(int fila, int columna) {
  std::pair<int, int> destino = mapa_.GetDestino();
  int distancia_rectilinea = std::abs(destino.first - fila) + std::abs(destino.second - columna);
  return 2 * distancia_rectilinea;
} 

/**
 * @brief Reconstruye la ruta trazada desde el destino hasta el origen.
 * 
 * Retrocede secuencialmente a través de los punteros del estado anterior guardados en cada 
 * nodo hasta llegar al estado inicial, y luego invierte el orden para formar el camino correcto.
 * 
 * @param estado Puntero al estado final (destino) desde el cual se comienza a retroceder.
 * @return std::vector<Estado> Vector ordenado cronológicamente desde el inicio hasta el fin del camino.
 */
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

/**
 * @brief Genera una representación en texto del estado actual de las listas abierta y cerrada.
 * 
 * @param iteracion Número de la iteración actual del algoritmo.
 * @param copia_abiertos Copia de la cola de prioridad (lista abierta) para extraer y formatear sus elementos sin vaciar la estructura original.
 * @param cerrados Vector constante que contiene las posiciones (fila, columna) de los nodos ya evaluados.
 * @return std::string Cadena de texto formateada que muestra la lista de abiertos y cerrados en coordenadas cartesianas.
 */
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
    int x_abiertos = e.GetColumna() + 1;
    int y_abiertos = mapa_.GetNFilas() - e.GetFila();
    resultado += "(" + std::to_string(x_abiertos) + "," + std::to_string(y_abiertos) + ")";
    primero = false;
  }
  resultado += "\n";
  // Imprimir Lista Cerrada
  resultado += "Cerrados = ";
  for (size_t i = 0; i < cerrados.size(); ++i) {
    if (i > 0) resultado += ", ";
    int x_cerrados = cerrados[i].second + 1;
    int y_cerrados = mapa_.GetNFilas() - cerrados[i].first;
    resultado += "(" + std::to_string(x_cerrados) + "," + std::to_string(y_cerrados) + ")";
  }
  resultado += "\n------------------------\n";
  return resultado;
}

/**
 * @brief Guarda los resultados del algoritmo, incluyendo el mapa trazado y las iteraciones, en ficheros.
 * 
 * Genera un mapa visual reemplazando las casillas del camino con el carácter '*' en un archivo, 
 * y vuelca toda la cadena de texto con el log de las iteraciones en un segundo archivo.
 * 
 * @param ruta_salida1 Ruta del archivo de texto donde se dibujará el estado final del mapa con el camino.
 * @param ruta_salida2 Ruta del archivo de texto donde se volcará el registro de las iteraciones.
 * @param iteraciones Cadena de texto con el registro completo de la ejecución del algoritmo.
 * @param camino Vector que contiene los estados pertenecientes al camino óptimo calculado.
 */
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