/* Utilización del LED RGB Integrado
 * Escriba un programa que haga parpadear un color en el LED RGB integrado en la
 * placa. Pines: Rojo (P0.22), Verde (P3.25), Azul (P3.26). Los LEDs son activos
 * por bajo (se encienden con un 0 lógico). Condiciones: Modularice el código
 * utilizando funciones como confiGPIO() y delay(). Implemente un retardo por
 * software (con un bucle for). Defina el tiempo del retardo con una macro
 * (#define).
 *
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#endif

#define DELAY 1000000
#define ROJO (1 << 22)
#define VERDE (1 << 25)
#define AZUL (1 << 26)

void configGPIO(void) {
  PINSEL_CFG_Type pin;
  // Configuracion P0.22
  pin.Portnum = PINSEL_PORT_0;
  pin.Pinnum = PINSEL_PIN_22;
  pin.Funcnum = PINSEL_FUNC_0;
  pin.Pinmode = PINSEL_PINMODE_PULLUP;
  pin.OpenDrain = PINSEL_PINMODE_NORMAL;
  PINSEL_ConfigPin(&pin);
  // configuracion p3.25
  pin.Portnum = PINSEL_PORT_3;
  pin.Pinnum = PINSEL_PIN_25;
  pin.Funcnum = PINSEL_FUNC_0;
  pin.Pinmode = PINSEL_PINMODE_PULLUP;
  pin.OpenDrain = PINSEL_PINMODE_NORMAL;
  PINSEL_ConfigPin(&pin);
  // configuracion p3.26
  pin.Portnum = PINSEL_PORT_3;
  pin.Pinnum = PINSEL_PIN_26;
  pin.Funcnum = PINSEL_FUNC_0;
  pin.Pinmode = PINSEL_PINMODE_PULLUP;
  pin.OpenDrain = PINSEL_PINMODE_NORMAL;
  PINSEL_ConfigPin(&pin);

  // Direccion de los pines -> salida
  GPIO_SetDir(0, ROJO, 1);  // SALIDA
  GPIO_SetDir(3, VERDE, 1); // SALIDA
  GPIO_SetDir(3, AZUL, 1);  // SALIDA
}

void delay(void) {
  for (int i = 0; i < DELAY; i++)
    ;
}

int main(void) {
  SystemInit();
  configGPIO();

  while (1) {
    // APAGO TODOS
    GPIO_SetValue(0, ROJO);
    GPIO_SetValue(3, AZUL);
    GPIO_SetValue(3, VERDE);
    delay();
    // PRENDO ROJO
    GPIO_ClearValue(0, ROJO);
    GPIO_SetValue(3, AZUL);
    GPIO_SetValue(3, VERDE);
    delay();
    // PRENDO AZUL
    GPIO_SetValue(0, ROJO);
    GPIO_ClearValue(3, AZUL);
    GPIO_SetValue(3, VERDE);
    delay();
    // PRENDO VERDE
    GPIO_SetValue(0, ROJO);
    GPIO_SetValue(3, AZUL);
    GPIO_ClearValue(3, VERDE);
    delay();
  }
  return 0;
}
