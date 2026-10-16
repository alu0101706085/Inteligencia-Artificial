import pandas as pd

ARCHIVO_SALIDA = "tabla_resultados.csv"
columnas = ["Instancia", "N", "M", "Best_Cmax", "Average_Cmax", "Desviacion", "Cmax_NEH", "RPD_UB", "RPD_NEH"]

try:
    df = pd.read_csv(ARCHIVO_SALIDA, names=columnas)
    print("\n=== TABLA DE RESULTADOS ===")
    print(df.to_string(index=False))
except FileNotFoundError:
    print(f"Error: El archivo {ARCHIVO_SALIDA} aún no ha sido generado por C++.")