# Script para calculo del valor de MATCH REGISTER

Fclock = 25e6  # Frecuencia de los perifericos 25[MHz]
f = 1025
t = 1 / f
print(t)
print(2**32)

while 1:
    Tint = float(input("Introducir el tiempo de Interrupcion: "))
    Pr = int(input("Ingrese PR en TICKS:"))
    Mr = ((Tint * Fclock) / (Pr + 1)) - 1
    print(f"Match Register: {Mr}")
    # print(Mr/2**32)
