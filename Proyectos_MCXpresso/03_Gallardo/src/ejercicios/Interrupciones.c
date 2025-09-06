/*
 * Excepciones son todas
 * Las interrupciones son solo de perifericos
 * Tienen un numero de excepciones, un IRQ, una prioridad y pueden ser sincronas
 *o asincronas
 *
 * El NVIC concentra las interrupciones, controla quien y como interrumpe
 *
 * Hay que configurar y saber:
 * - Quien interrumpe
 * - Que prioridad
 * - Habilitar la interrupcion
 * - Que vas a hacer la interrupcion
 *
 *	IPRx para configurar las prioridades
 *	ISERx para habilitar la interrupcion (con 1 se habilita, 0 no hace nada)
 *
 *	Estructura NVIC->
 *
 *	Funciones
 *	void NVIC_EnableIRQ(IRQn); -> habilita la interrupcion pedida
 *	void NVIC_DisableIRQ(IRQn); -> Desahbilita funciones
 *
 *	Handler
 *	cuando los definimos nosotros, se toma a esa como prioridad
 *	SysTick_Handler(void);
 *	EINTx_IRQHandler(void);
 *
 *	LPC_GPIOINT ->
 *
 *	Tenemos 4 interrupciones externas (0 al 3)
 *	SiAdemas podemos, habilitando la EINT 3, habilitamos las interrupciones
 *por GPIO Porque comparten el mismo lugar en la tabla.
 *
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
void configGPIO();            // configuracion
void prender(uint8_t numero); // funcion para PWM

int main(void) {
  SystemInit(); // configura los divisores de frecuencia (PLL)
  configGPIO();
  volatile uint8_t numero = 0;
  while (1) {
    numero = (LPC_GPIO0->FIOPIN) & (0b1111);
    prender(numero);
  }
  return 0;
}

void configGPIO() {

  // P0.0 a P0.3 como GPIO y el P0.22
  LPC_PINCON->PINSEL0 = 0; //~(0XFF)
  LPC_PINCON->PINSEL1 = 0;

  // ENTRADA P0.0 AL P0.3 Y EL P0.22 COMO SALIDA
  LPC_GPIO0->FIODIR &= (~(0b1111) | (1 << 22));

  LPC_PINCON->PINSEL4 |=
      (0b01 << 26); // Configuro como interrupcion Externa al pin P2.13

  LPC_GPIOINT->IO0IntEnR |= 0b1111; // Configurar interrupciones de P0.0 al P0.4

  LPC_SC->EXTMODE = (0 << 3); // Interrumpe por nivel

  NVIC_Enable(
      EINT3_IRQn); // habilitio la externa 3 para poder configurar los pines
}

void prender(uint8_t numero) {
  uint8_t porcentaje = 0;
  uint32_t delay = 1000000; // tiempo maximo - periodo
  uint32_t prendido = 0;
  uint32_t apagado = 0;
  if (numero == 0) {
    porcentaje = 0;
  } else {
    porcentaje = numero * 0.06;
  }
  prendido = delay * porcentaje;
  apagado = delay - prendido;

  LPC_GPIO0->FIOCLR = (1 << 22); // SE PRENDE POR BAJO
  for (uint32_t i = 0; i < prendido; i++)
    ;

  LPC_GPIO0->FIOSET = (1 << 22); // SE APAGA POR ALTO
  for (uint32_t i = 0; i < apagado; i++)
    ;
}

void
