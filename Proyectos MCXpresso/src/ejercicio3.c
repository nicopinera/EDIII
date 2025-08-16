/* EJERCICIO 3
 * Configurar el pin P0.4 como entrada digital con resistencia de pull down y
 * utilizarlo para decidir si el valor representado por los pines P0.0 al P0.3 van a ser sumados
 * o restados al valor guardado en la variable "acumulador".
 * El valor inicial de "acumulador" es 0
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void configGPIO();

int main3(void){
	configGPIO();
	uint8_t acumulador = 0;
	uint8_t valor = 0;
	while(1){
		valor = (LPC_GPIO0->FIOPIN >> 4) & 0x01;
		if(valor){
			//P0.4 = 1 -> se suma
			acumulador += (LPC_GPIO0->FIOPIN) & 0xF; //Sumo los 4 primeros
		}
		else{
			//P0.4 = 0 -> se resta
			acumulador -= (LPC_GPIO0->FIOPIN) & 0xF;
		}
	}
	return 0;
}

void configGPIO(){
	//CONFIGURAR COMO GPIO DEL P0.0	AL P0.4
	LPC_PINCON->PINSEL0 &= ~(0x3FF); // PRIMEROS 10 BITS EN 0 PARA GPIO

	// BIT 9 Y 8 LOS TENGO QUE PONER EN 1 PARA RESISTENCIA DE PULL DOWN
	// LOS OTROS NO TIENEN NINGUNA RESISTENCIA
	LPC_PINCON->PINMODE0 |= (0x3AA);

	LPC_GPIO0->FIODIR  |= 0; //TODOS COMO ENTRADA
}
