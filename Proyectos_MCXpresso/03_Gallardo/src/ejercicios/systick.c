/*Systick
 * contador de 24 bits
 * */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

int main2() {
  SystemInit(); // COnfigura el CClock en 100MHz (frecuencia del core)
  // SysTick->CTRL; Prendido o apagado, interrupcion y la fuente del clock

  // Tenemos que ver en que valor esta el divisor de frecuencia del core
  // para saber que valor cargarle tenemos que
  //  Tfinal = (1/Fcore)* (Vload+1) -> Vload = (Tfinal)/(1/Fcore) - 1
  // SysTick->LOAD=Vload;

  // Puedo saber el valor del reloj
  // SysTick->VAL;

  // SI quiero que interrumpa cada 20 Hz, el tiempo es 1/20 = 50ms

  return 0;
}
