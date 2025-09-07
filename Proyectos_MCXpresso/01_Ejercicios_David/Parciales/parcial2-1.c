/*
Utilizando Systick e interrupciones externas escribir un código en C que cuente
indefinidamente de 0 a 9.

Un pulsador conectado a Eint0 reiniciará la cuenta a 0 y se mantendrá en ese
valor mientras el pulsador se encuentre presionado.

Un pulsador conectado a Eint1 permitirá detener o continuar la cuenta cada vez
que sea presionado.

Un pulsador conectado a Eint2 permitirá modificar la velocidad de incremento del
contador.

En este sentido, cada vez que se presione ese pulsador el contador pasará a
incrementar su cuenta de cada 1 segundo a cada 1 milisegundo y viceversa.

Considerar que el microcontrolador se encuentra funcionando con un reloj (cclk)
de 16 Mhz.

El código debe estar debidamente comentado y los cálculos realizados claramente
expresados.

En la siguiente figura se muestra una tabla que codifica el display y el esquema
del hardware sobre el que funcionará elprograma.

*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#define SEG 15999999
#define MS 15999

// variables
uint32_t digitos[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,
                        0x6D, 0x7D, 0x07, 0x7F, 0x67};
uint32_t index = 0;
uint8_t frenar = 0;
uint8_t velocidad = 0;
// funciones
void configGPIO(void) {
  // CONFIGURAR P0.0 A P0.6 GPIO SALIDA
  LPC_PINCON->PINSEL0 &= ~(0x3FFF); // GPIO
  LPC_GPIO0->FIODIR |= (0x7F);      // SALIDA

  // configurar eint0
  LPC_PINCON->PINSEL4 |= (1 << 20);
  LPC_PINCON->PINMODE4 |= (0b11 << 20); // pull down

  // configurar eint1
  LPC_PINCON->PINSEL4 |= (1 << 22);
  LPC_PINCON->PINMODE4 |= (0b11 << 22); // pull down

  // configurar eint2
  LPC_PINCON->PINSEL4 |= (1 << 24);
  LPC_PINCON->PINMODE4 |= (0b11 << 24); // pull down
}
void configINT(void) {
  LPC_SC->EXTMODE &= ~(1 << 0); // LA EINT0 ES POR NIVEL

  LPC_SC->EXTMODE |= (1 << 1);  // LA EINT1 ES POR FLANCO
  LPC_SC->EXTPOLAR |= (1 << 1); // POR FLANCO ASCENDENTE

  LPC_SC->EXTMODE |= (1 << 2);  // LA EINT2 ES POR FLANCO
  LPC_SC->EXTPOLAR |= (1 << 2); // POR FLANCO ASCENDENTE

  NVIC_EnableIRQ(EINT0_IRQn); // habilita externa 0 (p2.10)
  NVIC_EnableIRQ(EINT1_IRQn); // habilita externa 1 (p2.11)
  NVIC_EnableIRQ(EINT2_IRQn); // habilita externa 2 (p2.12)
}
void configST(void) {
  LPC_SysTick->CTRL = (0x7); // HABILITO EL SYSTICK - HABILITO INTERRUPCION -
                             // SELECCIONO FUENTE DE CLOCK
  LPC_SysTick->VAL = 0;
  LPC_SysTick->LOAD = SEG;
  // LPC_SysTick->CTRL &= ~(1 << 16); // Limpio la bandera
}
void EINT0_IRQHandler(void) {
  // INTERRUMPE POR NIVEL
  int estado = (LPC_GPIO2->FIOPIN >> 10) & 0b1; // tomo el estado del pin
  if (estado) {                                 // si es uno esta apretado
    frenar = 1;
    index = 0;
  } else {
    frenar = 0;
  }
}
void EINT1_IRQHandler(void) {
  frenar ^= 1; // cambio el valor de 0 a 1 o de 1 a 0
  LPC_SC->EXTINT = (1 << 1);
}
void EINT2_IRQHandler(void) {
  velocidad ^= 1; // si es 0 es cada 1 [s] y si es 1 es cada 1 [ms]
  if (velocidad) {
    LPC_SysTick->LOAD = MS; // cada 1 ms
  } else {
    LPC_SysTick->LOAD = SEG; // cada 1 s
  }
  LPC_SC->EXTINT = (1 << 2);
}

void SysTick_Handler() {
  if (!frenar) {
    index = (index + 1) % 10;
  }
}

#include <cr_section_macros.h>

int main(void) {
  configGPIO();
  configINT();
  configST();
  while (1) {

    LPC_GPIO0->FIOPIN = digitos[index]; // muestro el numero
  }

  return 0;
}
