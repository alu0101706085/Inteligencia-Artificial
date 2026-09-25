# Variables
CC = g++
CFLAGS = -std=c++17 -g -Wall -I./include
TARGET = $(SRC_DIR)/main

# Directorios
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Archivos fuente
SRCS = $(SRC_DIR)/main.cc \
       $(SRC_DIR)/estado.cc \
       $(SRC_DIR)/mapa.cc \
	   $(SRC_DIR)/robot.cc \

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

# Ejecutar
run: all
	$(TARGET) $(SRC_DIR)/input.txt $(SRC_DIR)/mapa.txt $(SRC_DIR)/iteraciones.txt

# Ayuda
help:
	@echo "Opciones:"
	@echo "  make         Compila el programa principal."
	@echo "  make clean   Limpia los archivos generados."
	@echo "  make run     Ejecuta el programa principal."
	@echo "  make help    Muestra esta ayuda."