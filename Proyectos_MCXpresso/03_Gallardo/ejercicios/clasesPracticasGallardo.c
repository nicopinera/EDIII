/* CLASES PRACTICAS DE GALLARDO
 *
 * 1. Se detecta si hay un auto por EINT
 * GPIO input - interrumpe EINT P2[10] SI NO EXCISTE PODEMOS PONER INTGPIO
 * si interrumpe, hay un auto
 *
 * 2. Se valida el ticket por un pin P0[0]
 * va a ser un pin de entrada
 * interrumpe si el ticket es valido
 * Si es invalido- prende un led en P1[4]
 *
 * 3. Se levanta o no la barrera - P0[15]
 * GPIO - salida
 *
 * Time = GPIO interrumpe
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// funciones
void configuracionGPIO(void) {
  // P0.15 GPIO DE SALIDA - BARRERA
  LPC_PINCON->PINSEL0 &= ~(0B11 << 30); // GPIO
  LPC_GPIO0->FIODIR |= (1 << 15);       // SALIDA

  // P0.0 GPIO ENTRADA - VALIDA EL TICKET
  LPC_PINCON->PINSEL &= ~(0b11); // GPIO
  LPC_GPIO0->FIODIR &= ~(0b1);   // ENTRADA

  // P1.4 GPIO SALIDA - LED
  LPC_PINCON->PINSEL2 &= ~(0B11 << 8);
  LPC_GPIO1->FIODIR |= (1 << 4);

  // P2.10 EINT0 - DETECTA EL AUTO
  LPC_PINCON->PINSEL4 |= (1 << 20); // EINT0
}
void configuracionInretrrupciones(void) {
  // CONFIGURAR EINT0
}

// variables
uint8_t state = 0;
//  0 = todo detenido (ticket invalido tambien); 1 = auto presente;
// 2 = auto validado; 3 = abre barrera; 4 =

int tiempo_barrera = 0;

int main(void) {
  SystemInit();
  configuracionGPIO();
  configuracionInretrrupciones();

  while (1) {
    switch (state) {
    case 0:
      // todo detenido - no hago nada
      break;
    case 1:
      // auto detectado - HABILITO INTERRUPCION DE VALIDACION
      // HABILITO UN TIMER DE VALIDACION - 15S
      break;
    case 2:
      // STEAMOS EL SYSTICK CON EL TIEMPO DE CONFIG DE BARRERA

    case 3:
      // LEVANTAMOS BARRERA DURANTE EL TIEMPO tiempo_barrera
      break;
    case 4:
      /* code */
      break;
    default:
      break;
    }
  }
  return 0;
}
