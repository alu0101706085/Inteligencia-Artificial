import pandas as pd
import matplotlib.pyplot as plt

ARCHIVO_DATOS = "convergencia.csv"

try:
    df = pd.read_csv(ARCHIVO_DATOS, names=["Evaluaciones", "Cmax"])
    
    plt.figure(figsize=(10, 6))
    plt.plot(df["Evaluaciones"], df["Cmax"], color='#2c3e50', linewidth=2)
    
    plt.title('Gráfica de Convergencia (Cmax vs Evaluaciones)', fontsize=14, fontweight='bold')
    plt.xlabel('Número de Evaluaciones', fontsize=12)
    plt.ylabel('Mejor Cmax Encontrado', fontsize=12)
    
    plt.xlim(0, max(df["Evaluaciones"]))
    plt.grid(True, linestyle='--', alpha=0.6)
    
    plt.savefig('grafica_convergencia.png', dpi=300, bbox_inches='tight')
    print("Gráfica guardada con éxito como 'grafica_convergencia.png'")
    plt.show()
    
except FileNotFoundError:
    print(f"Error: No se encuentra '{ARCHIVO_DATOS}'.")