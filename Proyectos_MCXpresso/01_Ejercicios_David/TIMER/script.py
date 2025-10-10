# Script para calculo del valor de MATCH REGISTER

Fclock = 25e6 # Frecuencia de los perifericos 25[MHz]

while(1):
    Tint = float(input("Introducir el tiempo de Interrupcion: "))
    Pr = int(input("Ingrese PR en TICKS:"))
    Mr = ((Tint*Fclock)/(Pr+1))-1
    print(f"Match Register: {Mr}")
