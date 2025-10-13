/* Contador Hexadecimal Automático de 1 Dígito
 * Utilice 7 pines GPIO para controlar un display de 7 segmentos. El programa
 * debe mostrar de manera cíclica y automática los 16 dígitos hexadecimales
 * (0-F) de forma ascendente.
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#endif

#define DELAY 1000000

typedef struct {
  uint8_t puerto;
  uint32_t pin;
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = {
    {0, 0}, // p0.0
    {0, 1}, // p0.1
    {0, 2}, // p0.2
    {0, 3}, // p0.3
    {0, 4}, // p0.4
    {0, 5}, // p0.5
    {0, 6}, // p0.6
};

const int NUM_PUERTOS = sizeof(puert) / sizeof(puert[0]);

void configGPIO(void) {
  // Configuración de pines
  PINSEL_CFG_Type pin;
  for (int i = 0; i < NUM_PUERTOS; i++) {
    pin.Portnum = puert[i].puerto;
    pin.Pinnum = puert[i].pin; // obtiene el número del pin
    pin.Funcnum = 0;
    pin.Pinmode = 0;
    pin.OpenDrain = 0;
    PINSEL_ConfigPin(&pin);

    GPIO_SetDir(puert[i].puerto, 1 << puert[i].pin, 1);
  }
}
void apagarTodo(void) {
  for (int i = 0; i < NUM_PUERTOS; i++) {
    GPIO_ClearValue(puert[i].puerto, 1 << puert[i].pin);
  }
}

void delay(void) {
  for (int i = 0; i < DELAY; i++)
    ;
}

int main(void) {
  SystemInit();
  configGPIO();

  uint32_t segmento = 0;
  const uint32_t digits[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                               0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

  while (1) {
    apagarTodo();
    LPC_GPIO0->FIOSET =
        digits[segmento %
               16]; // cargo todo el valor, los bit que no toco se ponen en 0
    segmento++;
    delay();
  }
  return 0;
}
