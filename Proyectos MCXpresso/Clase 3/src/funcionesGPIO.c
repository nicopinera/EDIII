/*
 * En el presente archivo se estableceran las funciones para los puertos GPIO
 * Crear funciones de configuración. Paso el puerto, paso el pin, y entrada-salida.
 * Crear funciones de seteo.
 * Crear funciones de status.
 *
 * Para poner en salida un pin → |= (1 << pin)
 * Para poner en entrada un pin → &= ~(1 << pin)
 */
#include "funcionesGPIO.h"

void configurarGPIO(uint8_t puerto, uint8_t pin, uint8_t IO){
	switch(puerto){
	case 0:
		if(IO == 0){ // IO = 0 -> entrada
			LPC_GPIO0->FIODIR &= ~(1<<pin);
		}
		else{ //IO = 1 -> salida
			LPC_GPIO0->FIODIR |= (1<<pin);
		}
		break;
	case 1:
		if(IO == 0){ // IO = 0 -> entrada
			LPC_GPIO1->FIODIR &= ~(1<<pin);
		}
		else{ //IO = 1 -> salida
			LPC_GPIO1->FIODIR |= (1<<pin);
		}
		break;
	case 2:
		if(IO == 0){ // IO = 0 -> entrada
			LPC_GPIO2->FIODIR &= ~(1<<pin);
		}
		else{ //IO = 1 -> salida
			LPC_GPIO2->FIODIR |= (1<<pin);
		}
		break;
	case 3:
		if(IO == 0){ // IO = 0 -> entrada
			LPC_GPIO3->FIODIR &= ~(1<<pin);
		}
		else{ //IO = 1 -> salida
			LPC_GPIO3->FIODIR |= (1<<pin);
		}
		break;
	case 4:
		if(IO == 0){ // IO = 0 -> entrada
			LPC_GPIO4->FIODIR &= ~(1<<pin);
		}
		else{ //IO = 1 -> salida
			LPC_GPIO4->FIODIR |= (1<<pin);
		}
		break;
	}
}
void setteoGPIO(uint8_t puerto, uint8_t pin){
	switch(puerto){
	case 0:
		LPC_GPIO0->FIOSET = (IO<<pin);
		break;
	case 1:
		LPC_GPIO1->FIOSET = (IO<<pin);
		break;
	case 2:
		LPC_GPIO2->FIOSET = (IO<<pin);
		break;
	case 3:
		LPC_GPIO3->FIOSET = (IO<<pin);
		break;
	case 4:
		LPC_GPIO4->FIOSET = (IO<<pin);
		break;
	}
}
uint8_t statusGPIO(uint8_t puerto, uint8_t pin){
	uint8_t estadoPin;
	switch(puerto){
	case 0:
		estadoPin = (LPC_GPIO0->FIOPIN >> pin)& 1;
		break;
	case 1:
		estadoPin = (LPC_GPIO1->FIOPIN >> pin)& 1;
		break;
	case 2:
		estadoPin = (LPC_GPIO2->FIOPIN >> pin)& 1;
		break;
	case 3:
		estadoPin = (LPC_GPIO3->FIOPIN >> pin)& 1;
		break;
	case 4:
		estadoPin = (LPC_GPIO4->FIOPIN >> pin)& 1;
		break;
	}
	return estadoPin;
}

