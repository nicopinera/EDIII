// Ejercicio 1

/* Una famosa empresa de calzados a incorporado a sus zapatillas 10 luces leds comandadas por un microcontrolador LPC1769 y ha pedido a
 * su grupo de ingenieros que diseñen 2 secuencias de luces que cada cierto tiempo se vayan intercalando
 * (secuencia A - secuencia B- secuencia A- ... ). Como todavía no se ha definido la frecuencia a la cual va a
 * funcionar el CPU del microcontrolador, las funciones de retardos que se incorporen deben tener como parámetros de entrada
 * variables que permitan modificar el tiempo de retardo que se vaya a utilizar finalmente. Se pide escribir el código que
 * resuelva este pedido,considerando que los leds se encuentran conectados en los puertos
 * P0,0 al P0.9.
 *
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void delay(uint32_t tiempo); //variable entera sin signo de 32 bytes
void configuracionGPIO();
void secuencia(uint8_t sec);
int main(void) {
	configuracionGPIO();

    while(1) {
    	//secuencia A ->se prenden intercaladas
    	secuencia(0);
    	delay(100);

    	//secuencia B -> se prende la mitad
    	secuencia(1);
    	delay(100);
    }
    return 0 ;
}

void delay(uint32_t tiempo){
	for(uint32_t contador = 0;contador<tiempo;contador++)
		for(uint32_t contador2 = 0;contador2<tiempo;contador2++);
};

void configuracionGPIO(){
	//P0.0 a P0.9 -> PINSEL0 -> [19:0] 20 bits de los 10 puertos
	// SE ESTABLECE COMO GPIO
	LPC_PINCON->PINSEL0 &= ~(0XFFFFF); // PONGO 20 '0' PARA SETEAR GPIO

	LPC_GPIO0->FIODIR |= (0X3FF); // PONGO LOS 10 PRIMEROS EN '1' PARA PONER SALIDA

	LPC_GPIO0->FIOCLR = (0X3FF); // APAGO TODOS LIMPIANDOLOS (LOS PONGO EN 0)
}

void secuencia(uint8_t sec){
	LPC_GPIO0->FIOCLR = (0X3FF);
	switch (sec){
		case 0:
			LPC_GPIO0->FIOSET = (0b1010101010);
			break;
		case 1:
			LPC_GPIO0->FIOSET = (0b1111100000);
			break;
		default:
			break;
	}
}
