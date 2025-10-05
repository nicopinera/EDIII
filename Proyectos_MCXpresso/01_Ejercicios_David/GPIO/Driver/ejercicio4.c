/* Identificación de Nivel (Control de LED con Pulsador)
 * Escriba un programa que identifique la presión de un pulsador conectado a un pin GPIO configurado
 * como entrada. Utiliza otro pin de salida para encender un LED solo mientras el botón está presionado.
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#endif

#define DELAY 1000000
#define INPUT 0
#define OUTPUT 1

typedef struct {
	uint8_t puerto;
	uint32_t pin;
	uint8_t io; // Input / Output
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = { { 0, 0 ,INTPUT}, // p0.0 -> ENTRADA DEL PULSADOR
		{ 0, 1 ,OUTPUT}, // p0.1 -> SALIDA AL LED
		};

const int NUM_PUERTOS = sizeof(puert) / sizeof(puert[0]);

void configGPIO(void) {
	// Configuración de pines
	PINSEL_CFG_Type pin;
	for (int i = 0; i < NUM_PUERTOS; i++) {
		pin.Portnum = puert[i].puerto;
		pin.Pinnum = puert[i].pin; // obtiene el número del pin
		pin.Funcnum = 0;
		pin.Pinmode = 0;
		pin.OpenDrain = 0;
		PINSEL_ConfigPin(&pin);

		GPIO_SetDir(puert[i].puerto, 1 << puert[i].pin, puert[i].io);
	}
}

int main(void) {
	SystemInit();
	configGPIO();

	while (1) {
		uint32_t puerto0 = GPIO_ReadValue(0); // PUERTO 0
		if( puerto0 & 0b1 == 0 ){
			GPIO_SetValue(puert[1].puerto, puert[1].pin); // prende por alto (pin - led - gnd)
			// activo por alto (vcc - led - pin)
		}
		GPIO_ClearValue(puert[1].puerto, puert[1].pin);
	}
	return 0;
}
