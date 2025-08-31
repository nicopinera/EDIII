/*
 * 6. Escriba un programa que lea el estado de todos los pines disponibles del Puerto 0 y cuente cuántos de
 * ellos están en un nivel alto (1 lógico). El resultado debe mostrarse en binario utilizando 5 LEDs
 * conectados a los pines menos significativos del Puerto 2.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define
#define DELAY_TIME 1000000

// Funciones
void configGPIO();
uint8_t contarBits();

int main(void)
{

    configGPIO();
    int numero;
    while (1)
    {
        // EJERCICIO 6
        numero = contarBits();
        LPC_GPIO2->FIOPIN = numero;
    }
    return 0;
}

void configGPIO()
{
    LPC_PINCON->PINSEL0 = 0; // TODOS 0
    LPC_PINCON->PINSEL1 = 0; // TODOS 0
    LPC_GPIO0->FIODIR = 0;   // TODOS COMO ENTRADA

    LPC_PINCON->PINSEL4 &= ~(0x3FF); // GPIO
    LPC_GPIO2->FIODIR |= (0x1F);
}
uint8_t contarBits()
{
    uint8_t contador = 0;
    uint32_t estado = LPC_GPIO0->FIOPIN;
    for (int i = 0; i < 32; i++)
    {
        if (estado & 1 << i)
        {
            contador++;
        }
    }

    return contador;
}
