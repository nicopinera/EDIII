/* CLASES PRACTICAS DE GALLARDO
 * 
 * Se puede habilitar la interrupcion por ambos flancos en GPIO
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#define MAX 100 // Maximo tamaño buffer

volatile uint8_t lectura = 0; // para leer el buffer
volatile uint8_t escritura = 0; // escribir el buffer

uint8_t* buffer(uint8_t*)
0x2007C000;

void configGPIO() {
	/*
	 * CONFIGURAR P0.0 A P0.3 COMO GPIO ENTRADA
	 * CONFIGURAR P1.0 COMO GPIO SALIDA
	 * */
	LPC_PINCON->PINSEL0 &= ~(0XFF); // GPIO P0.0 A P0.3
	LPC_GPIO0->FIODIR &= ~(0XF); // ENTRADA
	LPC_PINCON->PINMODE0 = (0XAA); // SIN NINGUNA RESISTENCIA

	LPC_PINCON->PINSEL2 &= ~(0B11); // GPIO P1.0
	LPC_GPIO1->FIODIR |= (0B1); // SALIDA
}
void configInterrupciones() {
	/*
	 * DESHABILITAR INTERRUPCIONES
	 * Configurar interrupciones por GPIO - EINT3
	 * Ambos flancos
	 * P0.0 A P0.3
	 * HABILITAR INTERRUPCIONES
	 * */
	NVIC_DisableIRQ(EINT3_IRQn); // DESHABILITAR LA EXTERNA
	LPC_GPIOINT->IO0IntEnR |= (0XF); // HABILITAR FLANCO SUBIDA
	LPC_GPIOINT->IO0IntEnF |= (0XF); // HABILITAR FLANCO BAJADA
	LPC_GPIOINT->IO0IntClr |= (0XF); // LIMPIO BANDERA
	NVIC_EnableIRQ(EINT3_IRQn); // HABILITAR INTERRUPCION

}

void shortDelay() {
	for (int i = 0; i < 10000000; i++)
		;
}

void EINT3_IRQHandler() {
	if (LPC_GPIOINT->IntStatus & 1) { // INTERRUPCION DE P0
		buffer[escritura] = LPC_GPIO0->FIOPIN & 0B1111; // GUARDO VALORES DE LOS 4 PINES
	}
	escritura = (escritura + 1) % MAX; // MAXIMO VALOR DE ESCRITURA
}

int main(void) {
	SystemInit();
	configGPIO();
	configInterrupciones();
	uint8_t valor = 0;
	while (1) {
		valor = buffer[lectura]; // valor que hay que sacar

		for (uint8_t i = 0; i < 4; i++) {
			LPC_GPIO1->FIOPIN = (valor << i) & 0b1;
			shortDelay();
		}
		if (!escritura) {
			lectura = 0;
		} else {
			lectura = (lectura + 1) % escritura;

		}

	}
	return 0;
}
