/* 5. Escriba un programa que identifique la presión de un pulsador conectado a un pin GPIO configurado
 * como entrada. Utiliza otro pin de salida para encender un LED
 * solo mientras el botón está presionado.
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Funciones
void configGPIO();
uint8_t contarBits();

int main(void)
{

    configGPIO();
    int numero;
    while (1)
    {
        // ejercicio 5
        uint8_t estado = (LPC_GPIO0->FIOPIN) & 0b1; // TOMO EL PRIMER BIT
        if (estado == 0)
        {
            LPC_GPIO0->FIOSET = (1 << 1); // PRENDE POR ALTO
        }
        else
        {
            LPC_GPIO0->FIOCLR = (1 << 1); // SE APAGA POR BAJO
        }
    }
    return 0;
}

void configGPIO()
{
    // configurar P0.0 (ENTRADA) y P0.1 (SALIDA)
    LPC_PINCON->PINSEL0 &= ~(0xF);   // PRIMEROS 4 EN 0
    LPC_PINCON->PINMODE0 &= ~(0b11); // PULL UP -> SIEMPRE EN 1
    LPC_GPIO0->FIODIR &= ~(0b1);     // p0.0 COMO ENTRADA
    LPC_GPIO0->FIODIR |= 1 << 1;     // P0.1 COMO SALIDA
}
