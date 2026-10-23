# Algoritmo Genético para el PFSP (Permutation Flow Shop Scheduling)

Implementación en C++17 de un algoritmo genético (AG) para minimizar el
makespan (Cmax) en el problema de flujo permutacional. La población inicial
incluye la solución de la heurística constructiva **NEH**, que además se usa
como referencia para calcular la mejora del AG.

Probado sobre las instancias de Taillard `tai20_5_*` y `tai20_10_*`.

## Estructura del proyecto

```
.
├── Makefile
├── README.md
├── src/                      # Código fuente (.cc)
│   ├── main.cc
│   ├── algoritmoGenetico.cc
│   ├── AlgoritmosReferencia.cc
│   ├── instancia_PFSP.cc
│   └── solucion.cc
├── include/                  # Cabeceras (.h)
├── instancias/               # Instancias de Taillard (.fsp)
├── ejecutar_instancias.py    # Ejecuta todas las instancias por lotes
├── mostrar_tabla.py          # Muestra la tabla de resultados (lee tabla_resultados.csv)
└── graficar_convergencia.py  # Genera la gráfica de convergencia (lee convergencia.csv)
```

## Requisitos

- `g++` con soporte de C++17 y `make`
- `python3` con las librerías que usan los scripts auxiliares
  (`pandas` y `matplotlib`; instalar con `pip install pandas matplotlib`)

## Compilación

```bash
make
```

Genera el ejecutable `aa.exe` (los objetos intermedios quedan en `obj/`).

Para borrar el ejecutable, los objetos y los ficheros de resultados
(`tabla_resultados.csv` y `convergencia.csv`):

```bash
make clean
```

## Ejecución

### Una instancia

```bash
./aa.exe <instancia> <N> <E> <K> <Pc> <Pm> <tipo_reemplazo> [semilla]
```

| Parámetro   | Descripción                                                        |
|------------------|--------------------------------------------------------------------|
| `instancia`      | Ruta al fichero `.fsp` de la instancia                             |
| `N`              | Tamaño de la población                                             |
| `E`              | Número máximo de evaluaciones (criterio de parada)                 |
| `K`              | Tamaño del torneo (selección por torneo)                           |
| `Pc`             | Probabilidad de cruce (0–1)                                        |
| `Pm`             | Probabilidad de mutación (0–1)                                     |
| `tipo_reemplazo` | Probabilidad de mutación (0–1)                                     |
| `semilla`        | (Opcional) Semilla base. Si se omite, se usa la que trae la instancia |

Ejemplo (los parámetros usados en los experimentos):

```bash
./aa.exe ./instancias/tai20_5_1.fsp 100 50000 2 0.8 0.1 1
```

También se puede lanzar un ejemplo directamente con:

```bash
make run
```

que equivale a ejecutar `tai20_5_0.fsp` con `N=100, E=50000, K=2, Pc=0.8, Pm=0.1, tipo_reemplazo=1`.

### Todas las instancias (por lotes)

```bash
python3 ejecutar_instancias.py
```

Recorre todas las instancias de `instancias/` y ejecuta el AG con los
parámetros definidos en el script. Al terminar, muestra los resultados con:

```bash
python3 mostrar_tabla.py
```

> **Importante:** ejecutar siempre desde el directorio raíz del proyecto, ya que
> el programa llama a los scripts de Python con rutas relativas. Además
> `tabla_resultados.csv` se abre en modo *append*: si se repite una instancia,
> se añade una fila nueva. Para empezar una tabla limpia, ejecutar `make clean`
> (o borrar el CSV) antes de lanzar el lote.