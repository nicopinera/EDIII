/* Secuencias de Colores con el LED RGB
 * Escriba un programa que alterne entre dos o más secuencias de colores en el
 * LED RGB. El tiempo de duración de cada secuencia debe ser considerablemente
 * mayor que el retardo entre los colores dentro de la misma secuencia.
 *
 * Ejemplo:
 * secuencia_A: rojo -> verde -> amarillo (con un retardo corto entre colores).
 * secuencia_B: azul -> magenta -> blanco (con un retardo corto entre colores).
 * La transición de secuencia_A a secuencia_B debe ocurrir después de un tiempo
 * largo (ej. varios segundos).
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
#define REPETICIONES 25

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

void secuenciaB(void) {
  uint8_t i = 0;
  while (i < REPETICIONES) {
    // azul
    GPIO_ClearValue(3, AZUL);
    GPIO_SetValue(3, VERDE);
    GPIO_SetValue(0, ROJO);
    delay();
    // magenta
    GPIO_ClearValue(3, AZUL);
    GPIO_ClearValue(0, ROJO);
    GPIO_SetValue(3, VERDE);
    delay();
    // balnco
    GPIO_ClearValue(0, ROJO);
    GPIO_ClearValue(3, AZUL);
    GPIO_ClearValue(3, VERDE);
    delay();
    i++;
  }
}

void secuenciaA(void) {
  uint8_t i = 0;
  while (i < REPETICIONES) {
    // rojo
    GPIO_ClearValue(0, ROJO);
    GPIO_SetValue(3, AZUL);
    GPIO_SetValue(3, VERDE);
    delay();
    // verde
    GPIO_ClearValue(3, VERDE);
    GPIO_SetValue(3, AZUL);
    GPIO_SetValue(0, ROJO);
    delay();
    // amarillo
    GPIO_ClearValue(0, ROJO);
    GPIO_ClearValue(3, VERDE);
    GPIO_SetValue(3, AZUL);
    delay();
    i++;
  }
}

int main(void) {
  SystemInit();
  configGPIO();

  while (1) {
    secuenciaA();
    secuenciaB();
  }
  return 0;
}
