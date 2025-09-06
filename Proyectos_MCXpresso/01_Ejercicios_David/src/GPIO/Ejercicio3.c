/* 3. Escriba un programa que alterne entre dos o más secuencias de colores en
 * el LED RGB. El tiempo de duración de cada secuencia debe ser
 * considerablemente mayor que el retardo entre los colores dentro de la misma
 * secuencia. Ejemplo: secuencia_A: rojo -> verde -> amarillo (con un retardo
 * corto entre colores). secuencia_B: azul -> magenta -> blanco (con un retardo
 * corto entre colores). La transición de secuencia_A a secuencia_B debe ocurrir
 * después de un tiempo largo (ej. varios segundos).
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define
#define DELAY_TIME 10000000

// Funciones
void configGPIO();
void longDelay();
void shortDelay();
void secuenciaA();
void secuenciaB();

int main(void) {

  SystemInit();
  configGPIO();

  while (1) {

    // Ejercicio 3
    secuenciaA();
    longDelay();
    secuenciaB();
    longDelay();
  }
  return 0;
}

void configGPIO() {
  // se configurara para que parpadee el azul - P3.26
  LPC_PINCON->PINSEL7 &=
      ~(3 << 20);                 // UN 00 EN LOS BIT 21 Y 20 -> SELECCIONO GPIO
  LPC_GPIO3->FIODIR |= (1 << 26); // SE COLOCA COMO SALIDA

  // Configuracion del P3.25 - led verde
  LPC_PINCON->PINSEL7 &= ~(3 << 18);
  LPC_GPIO3->FIODIR |= (1 << 25);

  // Configuracion del P0.22 - Led Rojo
  LPC_PINCON->PINSEL1 &= ~(3 << 12);
  LPC_GPIO0->FIODIR |= (1 << 22);
}
void shortDelay() {
  for (uint32_t i = 0; i < DELAY_TIME; i++) {
  }
}

void longDelay() {
  for (uint32_t i = 0; i < DELAY_TIME; i++) {
    for (uint32_t i = 0; i < DELAY_TIME; i++) {
      for (uint32_t i = 0; i < DELAY_TIME; i++) {
      }
    }
  }
}

void secuenciaA() {
  // rojo -> verde -> amarillo
  uint8_t i = 0;
  while (i < 10) {
    // rojo
    LPC_GPIO0->FIOCLR = 1 << 22; // PRENDO ROJO
    LPC_GPIO3->FIOSET = 3 << 25; // APAGO VERDE Y AZUL
    shortDelay();

    // verde
    LPC_GPIO0->FIOSET = 1 << 22; // APAGO ROJO
    LPC_GPIO3->FIOSET = 1 << 26; // APAGO AZUL
    LPC_GPIO3->FIOCLR = 1 << 25; // PRENDO VERDE
    shortDelay();

    // amarillo
    LPC_GPIO0->FIOCLR = 1 << 22; // PRENDO ROJO
    LPC_GPIO3->FIOCLR = 1 << 25; // PRENDO VERDE
    LPC_GPIO3->FIOSET = 1 << 26; // APAGO AZUL
    shortDelay();

    i++;
  }
}

void secuenciaB() {
  // rojo -> verde -> amarillo
  uint8_t i = 0;
  while (i < 10) {
    // AZUL
    LPC_GPIO0->FIOSET = 1 << 22; // APAGO ROJO
    LPC_GPIO3->FIOSET = 1 << 25; // APAGO VERDE
    LPC_GPIO3->FIOCLR = 1 << 26; // PRENDO AZUL
    shortDelay();

    // MAGENTA
    LPC_GPIO0->FIOCLR = 1 << 22; // PRENDO ROJO
    LPC_GPIO3->FIOCLR = 1 << 26; // PRENDO AZUL
    LPC_GPIO3->FIOSET = 1 << 25; // APAGO VERDE
    shortDelay();

    // BLANCO
    LPC_GPIO0->FIOCLR = 1 << 22; // PRENDO ROJO
    LPC_GPIO3->FIOCLR = 1 << 25; // PRENDO VERDE
    LPC_GPIO3->FIOCLR = 1 << 26; // PRENDO AZUL
    shortDelay();

    i++;
  }
}
