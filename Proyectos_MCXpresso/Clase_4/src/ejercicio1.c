/* En un pin de entrada entra una serie de bit con un tiempo T
 * Por un pin lo saco solo los bit pares
 * Por otro solo saco los bit impares
 * Pero con periodo (T/2)
 *
 * SI entra 0xF628 se frena
 * Si entra 0x28F6 se vuelve a empezar
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>


int main3(void){
	SystemInit();
	return 0;
}
