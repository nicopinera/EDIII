/*
 * 2. Escriba un programa que haga parpadear un color en el LED RGB integrado en
 * la placa.
 *
 * Pines: Rojo (P0.22), Verde (P3.25), Azul (P3.26). Los LEDs son activos por
 * bajo (se encienden con un 0 lógico).
 *
 * Modularice el código utilizando funciones como configGPIO() y longDelay().
 * Implemente un retardo por software (con un bucle for).
 * Defina el tiempo del retardo con una macro (#define).
 *
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#define DELAY_TIME 10000000

// Funciones
void configGPIO();
void longDelay();

int main(void) {

  configGPIO();
  while (1) {
    // Ejercicio 2
    LPC_GPIO3->FIOCLR = (1 << 26); // Prendido
    longDelay();
    LPC_GPIO3->FIOSET = (1 << 26); // Apagado
    longDelay();
  }
  return 0;
}

void configGPIO() {
  // se configurara para que parpadee el azul - P3.26
  LPC_PINCON->PINSEL7 &=
      ~(3 << 20);                 // UN 00 EN LOS BIT 21 Y 20 -> SELECCIONO GPIO
  LPC_GPIO3->FIODIR |= (1 << 26); // SE COLOCA COMO SALIDA
}
