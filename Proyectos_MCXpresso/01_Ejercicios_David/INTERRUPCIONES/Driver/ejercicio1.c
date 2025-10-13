/* Escriba un programa muy sencillo que utilice un pin del Puerto 2 como
 * interrupción externa (EINT0). Cada vez que se active la interrupción (por
 * flanco de bajada), un LED conectado a otro pin debe cambiar de estado
 * (encenderse si estaba apagado, y apagarse si estaba encendido).
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#endif

#define FUNC_0 0
#define FUNC_1 1
#define FUNC_2 2
#define FUNC_3 3
#define OUTPUT 4
#define INPUT 5

volatile int flag = 0;

typedef struct {
  uint8_t puerto;
  uint32_t pin;
  uint8_t func; // func del pin
  uint8_t io;
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = {
    {2, 10, FUNC_1, INPUT},  // p2.10 EINT0 Entrada
    {0, 22, FUNC_0, OUTPUT}, // p0.22 GPIO Salida
};

const int NUM_PUERTOS = sizeof(puert) / sizeof(puert[0]);

void configPUERTOS(void) {
  PINSEL_CFG_Type pin;

  // Configuración de pines

  for (int i = 0; i < NUM_PUERTOS; i++) {
    pin.Portnum = puert[i].puerto;
    pin.Pinnum = puert[i].pin; // obtiene el número del pin
    pin.Funcnum = puert[i].func;
    pin.Pinmode = 0;
    pin.OpenDrain = 0;
    PINSEL_ConfigPin(&pin);
    switch (puert[i].io) {
    case 4:
      GPIO_SetDir(puert[i].puerto, 1 << puert[i].pin, 1);
      break;
    case 5:
      GPIO_SetDir(puert[i].puerto, 1 << puert[i].pin, 0);
      break;
    }
  }
}

void configEINT(void) {
  EXTI_InitTypeDef config_eint;
  config_eint.EXTI_Line = EXTI_EINT0;
  config_eint.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE; // flancos
  config_eint.EXTI_polarity =
      EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE; // bajada
  EXTI_Init();
  EXTI_Config(&config_eint);
  NVIC_EnableIRQ(EINT0_IRQn);
}

void EINT0_IRQHandler(void) {
  flag ^= 1;
  EXTI_ClearEXTIFlag(EXTI_EINT0); // limpio bandera
}

int main(void) {
  SystemInit();
  configGPIO();
  configEINT();

  while (1) {
    switch (flag) {
    case 0: // apago
      GPIO_ClearValue(puert[1].puerto, 1 << puert[1].pin);
      break;
    case 1: // prendo
      GPIO_SetValue(puert[1].puerto, 1 << puert[1].pin);
      break;
    }
  }
  return 0;
}
