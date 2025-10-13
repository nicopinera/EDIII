/* Contador de Bits de un Puerto
 * Escriba un programa que lea el estado de todos los pines disponibles del
 * Puerto 0 y cuente cuántos de ellos están en un nivel alto (1 lógico). El
 * resultado debe mostrarse en binario utilizando 5 LEDs conectados a los pines
 * menos significativos del Puerto 2.
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#endif

#define INPUT 0
#define OUTPUT 1

typedef struct {
  uint8_t puerto;
  uint32_t pin;
  uint8_t io; // Input / Output
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = {
    {2, 0, OUTPUT}, // p2.0
    {2, 1, OUTPUT}, // p2.1
    {2, 2, OUTPUT}, // p2.2
    {2, 3, OUTPUT}, // p2.3
    {2, 4, OUTPUT}, // p2.4
};

const int NUM_PUERTOS = sizeof(puert) / sizeof(puert[0]);

void configGPIO(void) {
  PINSEL_CFG_Type pin;
  for (int i = 0; i < 32; i++) {
    pin.Portnum = 0;
    pin.Pinnum = i;  // todos
    pin.Funcnum = 0; // gpio
    pin.Pinmode = 0; // pull up
    pin.OpenDrain = 0;
    PINSEL_ConfigPin(&pin);

    GPIO_SetDir(0, 1 << i, INPUT);
  }

  // Configuración de pines

  for (int i = 0; i < NUM_PUERTOS; i++) {
    pin.Portnum = puert[i].puerto;
    pin.Pinnum = puert[i].pin; // obtiene el número del pin
    pin.Funcnum = 0;
    pin.Pinmode = 0;
    pin.OpenDrain = 0;
    PINSEL_ConfigPin(&pin);

    GPIO_SetDir(puert[i].puerto, 1 << puert[i].pin, puert[i].io);
  }
}

int contar() {
  int contador = 0;
  int estado = GPIO_ReadValue(0);
  for (int i = 0; i < 32; i++) {
    if (estado >> i & 0b1) {
      contador++;
    }
  }
  return contador;
}

int main(void) {
  SystemInit();
  configGPIO();

  while (1) {
    int bits = contar();
    GPIO_SetValue(2, bits);
  }
  return 0;
}
