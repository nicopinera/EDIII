/* Escriba un programa que utilice el SysTick para gestionar dos tareas con diferentes periodos de tiempo.
• Tareas:
o Un LED debe parpadear cada 500 ms.
o Una secuencia de 4 LEDs debe avanzar un paso cada 200 ms.
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_systick.h"
#endif

#define FUNC_0 0
#define FUNC_1 1
#define FUNC_2 2
#define FUNC_3 3
#define OUTPUT 4
#define INPUT 5
#define TIMESYSTICK 100 // ms

volatile int bandera = 0; // prende o apaga el led
volatile int cuenta_int = 0; // cuenta de interrupciones
volatile int led = 0; // indica que led se prende -> maximo 4

typedef struct {
	uint8_t puerto;
	uint32_t pin;
	uint8_t func; // func del pin
	uint8_t io;
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] =
		{ { 0, 1, FUNC_0, OUTPUT }, { 0, 2, FUNC_0, OUTPUT }, { 0, 3, FUNC_0,
				OUTPUT }, { 0, 4, FUNC_0, OUTPUT }, { 0, 22, FUNC_0, OUTPUT }, // se prende por bajo
		};

const int NUM_PUERTOS = sizeof(puert) / sizeof(puert[0]);

void configPUERTOS(void) {
	PINSEL_CFG_Type pin;

	// Configuración de pines

	for (int i = 0; i < NUM_PUERTOS; i++) {
		pin.Portnum = puert[i].puerto;
		pin.Pinnum = puert[i].pin; // obtiene el número del pin
		pin.Funcnum = puert[i].func;
		pin.Pinmode = 0;
		pin.OpenDrain = 0;
		PINSEL_ConfigPin(&pin);
		switch (puert[i].io) {
		case 4:
			GPIO_SetDir(puert[i].puerto, 1 << puert[i].pin, 1);
			break;
		case 5:
			GPIO_SetDir(puert[i].puerto, 1 << puert[i].pin, 0);
			break;

		}
	}
}

void prenderTodos(void) {
	for (int i = 0; i < NUM_PUERTOS; i++) {
		GPIO_SetValue(puert[i].puerto, 1 << puert[i].pin);
	}
}

void apagarTodos(void) {
	for (int i = 0; i < NUM_PUERTOS; i++) {
		GPIO_ClearValue(puert[i].puerto, 1 << puert[i].pin);
	}
}

void desplazamiento(void) {
	// ida
	for (int i = 0; i < NUM_PUERTOS; i++) {
		apagarTodos();
		GPIO_SetValue(puert[i].puerto, 1 << puert[i].pin);
	}
}
void configSYSTICK(void) {
	// COnfigurar para interrumpir cada 100 ms
	SYSTICK_InternalInit(TIMESYSTICK);
	SYSTICK_Cmd(ENABLE); // habilito
	SYSTICK_IntCmd(ENABLE); // habilito interrupcion
}

void SysTick_Handler(void) {
	cuenta_int++;
	if (!(cuenta_int % 2)) { // ingresa cada dos interrupciones -> 200ms
		apagarTodos();
		GPIO_SetValue(puert[led].puerto, 1 << puert[led].pin);
		led = (led + 1) % 4;
	}

	if (cuenta_int == 5) {
		bandera ^= 1;
		switch (bandera) {
		case 0: // apago
			GPIO_SetValue(0, 1 << 22);
			break;
		case 1: // prendo
			GPIO_ClearValue(0, 1 << 22);
			break;
		}
	}
	SYSTICK_ClearCounterFlag(); // limpio bandera por las dudas
}

int main(void) {
	SystemInit();
	configPUERTOS();
	configSYSTICK();

	while (1) {

	}
	return 0;
}