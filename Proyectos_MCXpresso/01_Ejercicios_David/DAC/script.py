import numpy as np

# Número de muestras
N = 32

# Genero 32 valores uniformemente distribuidos entre 0 y 2*pi (un ciclo completo)
x = np.linspace(0, 2*np.pi, N, endpoint=False)

# Calculo la onda sinusoidal
onda_seno = np.sin(x)

# Normalizamos a valores positivos entre 0 y 1
onda_normalizada = (onda_seno + 1) / 2

# Escalamos a rango de 10 bits
onda_DAC = np.round(onda_normalizada * 1023).astype(int)

print(onda_DAC)