import os
import subprocess

# Configuración
EJECUTABLE = "./aa.exe"
CARPETA_INSTANCIAS = "./instancias"
POBLACION = "100"
EVALUACIONES = "50000"
TORNEO = "2"
CRUCE = "0.8"
MUTACION = "0.1"

print("=== INICIANDO EJECUCIÓN POR LOTES (MODO SILENCIOSO) ===")

try:
    archivos = os.listdir(CARPETA_INSTANCIAS)
    # Filtramos solo los ficheros 20_5 y 20_10
    instancias_objetivo = [
        f for f in archivos 
        if (f.startswith("tai20_5_") or f.startswith("tai20_10_")) and f.endswith(".fsp")
    ]
    instancias_objetivo.sort()

    if not instancias_objetivo:
        print("No se encontraron instancias 20_5 o 20_10 en la carpeta.")
        exit()

    for archivo in instancias_objetivo:
        ruta_completa = os.path.join(CARPETA_INSTANCIAS, archivo)
        comando = [
            EJECUTABLE, ruta_completa, POBLACION, EVALUACIONES, 
            TORNEO, CRUCE, MUTACION
        ]
        
        print(f"Procesando: {archivo}...", end="", flush=True)
        
        # Al poner stdout y stderr en DEVNULL, silenciamos tu C++ y las llamadas a los otros scripts
        subprocess.run(comando, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)
        
        print(" ¡Completado!")

    print("\nLote terminado. Ejecuta 'python3 mostrar_tabla.py' para ver el resultado final.")

except FileNotFoundError:
    print(f"Error: Asegúrate de que la carpeta '{CARPETA_INSTANCIAS}' existe.")
except subprocess.CalledProcessError:
    print(" Error durante la ejecución del programa C++.")