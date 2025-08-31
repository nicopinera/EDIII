/* MODULO 1 - GPIO Y PINSEL
 *
 * 1. Identificacion de Registros PINSEL y PINMODE:
 * Si tenemos el puerto Px.y y debemos configurarlo
 * Los registros seran:
 * [Registro]	(2*x) -- (2*x)-1
 * [Pines]		y<=15 -- y>=16
 * [Bits]		2*y	  -- 2(y-16)
 * 				2*y+1 -- 2(y-16)+1
 *
 * Si tenemos el pin P1.8
 * Tendriamos que seleccionar entre el PINSEL2 O PINSEL3 (PINMODE2 O PINMODE3)
 * Como el pin es el 8, es menor a a 15 seleccionamos PINSEL2 (PINMODE2)
 * Los bit que tendremos que modificar son el 16 y 17 del registro PINSEL2 Y PINMODE2
 *
 * 2. Escriba un programa que haga parpadear un color en el LED RGB integrado en la placa.
 *
 * Pines: Rojo (P0.22), Verde (P3.25), Azul (P3.26). Los LEDs son activos por bajo
 * (se encienden con un 0 lógico).
 *
 * Modularice el código utilizando funciones como confiGPIO() y delay().
 * Implemente un retardo por software (con un bucle for).
 * Defina el tiempo del retardo con una macro (#define).
 *
 * 3. Escriba un programa que alterne entre dos o más secuencias de colores en el LED RGB. El tiempo de
 * duración de cada secuencia debe ser considerablemente mayor que el retardo entre los colores dentro
 * de la misma secuencia.
 * Ejemplo:
 * secuencia_A: rojo -> verde -> amarillo (con un retardo corto entre colores).
 * secuencia_B: azul -> magenta -> blanco (con un retardo corto entre colores).
 * La transición de secuencia_A a secuencia_B debe ocurrir después de un tiempo largo
 * (ej. varios segundos).
 *
 *4. Utilice 7 pines GPIO para controlar un display de 7 segmentos.
 *El programa debe mostrar de manera cíclica y automática los
 *16 dígitos hexadecimales (0-F) de forma ascendente.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define
#define DELAY_TIME 10000000

//Funciones
void configGPIO();
void longDelay();
void shortDelay();
void secuenciaA();
void secuenciaB();

int main(void) {

	SystemInit();
	configGPIO();

	uint32_t segmento = 0;
	const uint32_t digits[16] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,
			0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };

	while (1) {
		// Ejercicio 2
		//LPC_GPIO3->FIOCLR = (1 << 26); // Prendido
		//delay();
		//LPC_GPIO3->FIOSET = (1 << 26); // Apagado
		//delay();

		// Ejercicio 3
		//secuenciaA();
		//longDelay();
		//secuenciaB();
		//longDelay();

		// Ejercicio 4
		LPC_GPIO2->FIOCLR = 0x7F; // APAGO TODOS
		LPC_GPIO2->FIOSET = digits[segmento % 16]; // cargo todo el valor, los bit que no toco se ponen en 0

		segmento++;
		shortDelay();
	}
	return 0;
}

void configGPIO() {
	//se configurara para que parpadee el azul - P3.26
	LPC_PINCON->PINSEL7 &= ~(3 << 20); // UN 00 EN LOS BIT 21 Y 20 -> SELECCIONO GPIO
	LPC_GPIO3->FIODIR |= (1 << 26); // SE COLOCA COMO SALIDA

	// Configuracion del P3.25 - led verde
	LPC_PINCON->PINSEL7 &= ~(3 << 18);
	LPC_GPIO3->FIODIR |= (1 << 25);

	// Configuracion del P0.22 - Led Rojo
	LPC_PINCON->PINSEL1 &= ~(3 << 12);
	LPC_GPIO0->FIODIR |= (1 << 22);

	// Configuracion de los 7 pines del puerto 2
	LPC_PINCON->PINSEL4 &= ~(0x3FFF); // PRIMEROS 14 BITS EN 0 PARA GPIO
	LPC_GPIO2->FIODIR |= (0x7F); // LOS PRIMEROS 7 COMO SALIDA
}
void shortDelay() {
	for (uint32_t i = 0; i < DELAY_TIME; i++) {
	}
}

void longDelay() {
	for (uint32_t i = 0; i < DELAY_TIME; i++) {
		for (uint32_t i = 0; i < DELAY_TIME; i++) {
			for (uint32_t i = 0; i < DELAY_TIME; i++) {

			}
		}
	}
}

void secuenciaA() {
	// rojo -> verde -> amarillo
	uint8_t i = 0;
	while (i < 10) {
		// rojo
		LPC_GPIO0->FIOCLR = 1 << 22; // PRENDO ROJO
		LPC_GPIO3->FIOSET = 3 << 25; // APAGO VERDE Y AZUL
		shortDelay();

		// verde
		LPC_GPIO0->FIOSET = 1 << 22; // APAGO ROJO
		LPC_GPIO3->FIOSET = 1 << 26; // APAGO AZUL
		LPC_GPIO3->FIOCLR = 1 << 25; // PRENDO VERDE
		shortDelay();

		// amarillo
		LPC_GPIO0->FIOCLR = 1 << 22; // PRENDO ROJO
		LPC_GPIO3->FIOCLR = 1 << 25; // PRENDO VERDE
		LPC_GPIO3->FIOSET = 1 << 26; // APAGO AZUL
		shortDelay();

		i++;
	}
}

void secuenciaB() {
	// rojo -> verde -> amarillo
	uint8_t i = 0;
	while (i < 10) {
		// AZUL
		LPC_GPIO0->FIOSET = 1 << 22; // APAGO ROJO
		LPC_GPIO3->FIOSET = 1 << 25; // APAGO VERDE
		LPC_GPIO3->FIOCLR = 1 << 26; // PRENDO AZUL
		shortDelay();

		// MAGENTA
		LPC_GPIO0->FIOCLR = 1 << 22; // PRENDO ROJO
		LPC_GPIO3->FIOCLR = 1 << 26; // PRENDO AZUL
		LPC_GPIO3->FIOSET = 1 << 25; // APAGO VERDE
		shortDelay();

		// BLANCO
		LPC_GPIO0->FIOCLR = 1 << 22; // PRENDO ROJO
		LPC_GPIO3->FIOCLR = 1 << 25; // PRENDO VERDE
		LPC_GPIO3->FIOCLR = 1 << 26; // PRENDO AZUL
		shortDelay();

		i++;
	}
}
