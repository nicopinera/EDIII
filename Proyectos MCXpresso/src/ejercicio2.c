// Ejercicio 2

/* En los pines P2.0 a P2.7 se encuentra conectado un display de 7 segmentos.
 * Utilizando la variable numDisplay [10] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67}
 * que codifica los números del 0 a 9 para ser mostrados en el display, realizar un programa que muestre
 * indefinidamente la cuenta de 0 a 9 en dicho display.
 *
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#define DELAY_TIME2 100000
void configuracionGPIO2();
void delay2();

int main2(void) {
	configuracionGPIO2();
	int i = 0;
	const uint32_t numDisplay [10] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

    while(1) {
    	LPC_GPIO2->FIOCLR = (0XFF); // APAGO TODOS
    	LPC_GPIO2->FIOSET = numDisplay[i%10];
    	i++;
    	delay2();
    }
    return 0 ;
}

void delay2(){
	for(uint32_t i = 0; i<DELAY_TIME2 ; i++)
		for(uint32_t j = 0; j<DELAY_TIME2 ; j++);
}

void configuracionGPIO2(){
	//P2.0 a P2.7 -> PINSEL0 -> [15:0] 16 bits de los 8 puertos
	// SE ESTABLECE COMO GPIO
	LPC_PINCON->PINSEL4 &= ~(0xFFFF); // PONGO 16 '0' PARA SETEAR GPIO

	LPC_GPIO2->FIODIR |= (0XFF); // PONGO LOS 10 PRIMEROS EN '1' PARA PONER SALIDA
}


