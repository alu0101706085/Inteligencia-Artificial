import pandas as pd

ARCHIVO_SALIDA = "tabla_resultados.csv"

# 1. Definimos las columnas exactamente en el mismo orden que las escupe tu C++
columnas = [
    "Instancia", "N", "M", "Best_Cmax", "Average_Cmax", 
    "Desviacion", "Cmax_NEH", "RPD_UB", "RPD_NEH", "CPU_Tiempo"
]

try:
    # 2. names=columnas fuerza a Pandas a usar nuestra lista como cabecera
    df = pd.read_csv(ARCHIVO_SALIDA, names=columnas)
    
    print("\n=== TABLA DE RESULTADOS ===")
    # 3. to_string(index=False) lo imprime limpio, sin los números de fila a la izquierda
    print(df.to_string(index=False))
    
except FileNotFoundError:
    print(f"Error: El archivo {ARCHIVO_SALIDA} no se encuentra.")