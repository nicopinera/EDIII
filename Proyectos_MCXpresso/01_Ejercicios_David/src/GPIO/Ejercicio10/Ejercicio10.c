/* Escriba un programa que realice un promedio móvil con los últimos 8 datos leídos del puerto 0.
 * Entrada: Los 8 bits menos significativos del Puerto 0, leídos cada cierto tiempo (usando un delay).
 * Salida: El resultado del promedio, también de 8 bits, debe mostrarse en los 8 pines más bajos del Puerto 2.
 * Consideración: Un promedio móvil se calcula sumando los últimos n valores y dividiendo por n. En este caso, n=8.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define
#define DELAY_TIME 10000000

// Funciones
void configGPIO();
void shortDelay() {
	for (int i = 0; i < DELAY_TIME; i++)
		;
}
#define N 8

uint8_t buffer[N];    // Buffer circular para últimos N valores
uint8_t indice = 0;   // Índice actual del buffer
uint8_t total = 0;    // Suma acumulada

uint8_t promedioMovil(uint8_t nuevo_valor) {
	// Restar el valor más viejo del total
	total -= buffer[indice];

	// Agregar nuevo valor al buffer y al total
	buffer[indice] = nuevo_valor;
	total += nuevo_valor;

	// Avanzar índice circular
	indice = (indice + 1) % N;

	// Calcular y retornar promedio
	return total / N;
}

int main(void) {
	SystemInit();
	configGPIO();

	// Inicializar buffer con ceros
	for (int i = 0; i < N; i++) {
		buffer[i] = 0;
	}

	while (1) {
		uint8_t nuevo_valor = LPC_GPIO0->FIOPIN & 0xFF;
		uint8_t promedio = promedioMovil(nuevo_valor);

		LPC_GPIO2->FIOPIN = promedio;
		shortDelay(); // Espera entre muestras
	}
	return 0;
}

void configGPIO() {
	// Configuracion P0.0 a P0.7 como entrada
	// Configuracion P2.0 a P2.7 como salida
	LPC_PINCON->PINSEL0 &= ~(0xFFFF); // CONFIGURO GPIO
	LPC_PINCON->PINSEL4 &= ~(0x3FFF); // CONFIGURO GPIO

	LPC_GPIO0->FIODIR &= ~(0xFF); // P0.0 A P0.7 COMO ENTRADA
	LPC_GPIO2->FIODIR |= (0xFF); // P2.0 A P2.7 COMO SALIDA

}
