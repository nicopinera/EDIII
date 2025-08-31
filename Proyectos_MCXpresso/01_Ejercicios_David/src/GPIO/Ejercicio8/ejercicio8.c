/* Escriba un programa que lea un valor de 4 bits en formato binario (0-F) de 4 pines de entrada. Este valor
 * debe ser decodificado y mostrado en un display de 7 segmentos.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define
#define DELAY_TIME 10000000

// Funciones
void configGPIO();
void shortDelay();

int main(void) {

	SystemInit();
	configGPIO();

	uint32_t segmento = 0;
	const uint32_t digits[16] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,
			0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };
	uint8_t numero = 0;
	while (1) {
		numero = LPC_GPIO0->FIOPIN & 0xF; // Tomo los primeros 4
		LPC_GPIO2->FIOPIN = digits[numero];
		shortDelay();
	}
	return 0;
}

void configGPIO() {
	// Configuracion de los 7 pines del puerto 2
	LPC_PINCON->PINSEL4 &= ~(0x3FFF); // PRIMEROS 14 BITS EN 0 PARA GPIO
	LPC_GPIO2->FIODIR |= (0x7F);      // LOS PRIMEROS 7 COMO SALIDA

	LPC_PINCON->PINSEL0 &= ~(0xFF); // P0.0 a P0.3 como GPIO
	LPC_GPIO0->FIODIR &= ~(0xF);
}
void shortDelay() {
	for (uint32_t i = 0; i < DELAY_TIME; i++) {
	}
}
