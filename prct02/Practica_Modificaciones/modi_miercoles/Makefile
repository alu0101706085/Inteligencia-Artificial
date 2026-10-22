# Variables
CC = g++
CFLAGS = -std=c++17 -O2 -DNDEBUG -Wall -I./include -I./src
TARGET = aa.exe

# Directorios
SRC_DIR = src
OBJ_DIR = obj

# Archivos fuente (Extraídos de tu directorio src)
SRCS = $(SRC_DIR)/main.cc \
       $(SRC_DIR)/algoritmoGenetico.cc \
       $(SRC_DIR)/AlgoritmosReferencia.cc \
       $(SRC_DIR)/instancia_PFSP.cc \
       $(SRC_DIR)/solucion.cc

# Archivos objeto
OBJS = $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean run help

# Regla principal
all: $(TARGET)

# Regla para el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Regla para archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f $(TARGET)
	rm -rf $(OBJ_DIR)
	rm -f tabla_resultados.csv convergencia.csv

# Ejecutar (Ejemplo con parámetros por defecto)
run: all
	./$(TARGET) ./instancias/tai20_5_0.fsp 100 50000 2 0.8 0.1

# Ayuda
help:
	@echo "Opciones:"
	@echo "  make         Compila el programa principal (aa.exe)."
	@echo "  make clean   Limpia los archivos generados (.o, .exe, .csv)."
	@echo "  make run     Ejecuta el programa con parámetros de prueba."
	@echo "  make help    Muestra esta ayuda."