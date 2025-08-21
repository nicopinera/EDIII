// Clase 3
/* - Los PLL son divisores de frecuencia para
 *
 *	Ejercicio
 *	del P0.0 al P0.3 se toma un numero N de 4 bit
 *	por el P0.22 tiene que prender el tiempo determinado del PWM segun el numero N
 *	f=20hz
 *
 *	PWM - modulacion por ancho de pulso, cambias el tiempo en alto de un pulso rectangular
 *	manteniendo la frecuencia fija. va del 0 al 100
 * */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void configGPIO(); // configuracion
void prender(uint8_t  numero); //funcion para PWM

int main(void) {
	SystemInit(); //configura los divisores de frecuencia (PLL)
    configGPIO();
	volatile uint8_t numero = 0;
    while(1) {
		numero = (LPC_GPIO0->FIOPIN) & (0b1111);
		prender(numero);
    }
    return 0 ;
}

void configGPIO(){
	//P0.0 a P0.3 como GPIO y el P0.22
	LPC_PINCON->PINSEL0 =0; //~(0XFF)
	LPC_PINCON->PINSEL1 =0;

	//ENTRADA P0.0 AL P0.3 Y EL P0.22 COMO ENTRADA
	LPC_GPIO0 -> FIODIR &= (~(0b1111)|(1<<22));
}

void prender(uint8_t numero){
	uint8_t porcentaje = 0;
	uint32_t delay = 1000000; //tiempo maximo - periodo
	uint32_t prendido = 0;
	uint32_t apagado = 0;
	if(numero== 0){
		porcentaje = 0;
	}
	else{
		porcentaje = numero * 0.06;
	}
	prendido = delay * porcentaje;
	apagado = delay - prendido;

	LPC_GPIO0->FIOSET = (1<<22);
	for(uint32_t i = 0;i<prendido;i++);

	LPC_GPIO0->FIOCLR = (1<<22);
	for(uint32_t i = 0;i<apagado;i++);

}
