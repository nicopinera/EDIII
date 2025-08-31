/* Realice el ejercicio del contador hexadecimal (1.7), pero en esta ocasión, el incremento de la cuenta
 * debe ser manejado por una interrupción por flanco de subida en un pin GPIO.
 *
 * Pines: Utilice un pin del Puerto 0 como entrada para el pulsador y 7 pines del Puerto 2 para el display de 7 segmentos.
 *
 * Configure el pin de entrada para que genere una interrupción solo cuando se detecte un flanco de subida (paso de 0 a 1).
 * La lógica de incremento del contador debe residir en el Handler de la Interrupción (ISR).
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define

// Variables
volatile uint8_t contador = 0;
volatile uint32_t digits[16] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
		0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };

// Funciones
void configGPIO();
void configInterrupcion();
void EINT3_IRQHandler();

int main(void) {
	SystemInit();
	configGPIO();
	configInterrupcion();

	LPC_GPIO2->FIOCLR = 0x7F; // LIMPIO TODO, EMPIEZAN APAGADOS

	while (1) {
		LPC_GPIO2->FIOPIN = digits[contador];
	}
	return 0;
}

void configGPIO() {
	LPC_PINCON->PINSEL0 &= ~(0B11); //P0.0 COMO GPIO
	LPC_PINCON->PINSEL4 &= ~(0X3FFF); // P2.0 A P2.6 COMO GPIO
	LPC_GPIO0->FIODIR &= ~(1 << 0); // P0.0 COMO ENTRADA
	LPC_GPIO2->FIODIR |= (0x7F); // P2.0 A P2.6 COMO SALIDA

}
void configInterrupcion() {
	// CONFIGURA EL MODO
	LPC_GPIOINT->IO0IntEnR |= (0B1); // SE HABILITA FLANCO DE SUBIDA
	LPC_GPIOINT->IO0IntClr |= (0B1); // LIMPIO LA BANDERA

	NVIC_EnableIRQ(EINT3_IRQn); // HABILITO LA INTERRUPCION
}
void EINT3_IRQHandler() {
	if ((LPC_GPIOINT->IntStatus & 0b1)) {
		if (LPC_GPIOINT->IntStatus & 0x1) {
			LPC_GPIOINT->IO0IntClr |= (0B1); // LIMPIO LA BANDERA
			contador = (contador + 1) % 16;
			return;
		}

	}

}
