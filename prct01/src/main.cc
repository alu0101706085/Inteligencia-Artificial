#include <iostream>

#include "estado.h"
#include "mapa.h"
#include "robot.h"

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