/* Escriba una rutina simple para configurar e iniciar un Systick que pueda interrumpir cada 10 ms. Escriba también una rutina mínima del handler.
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
#define TIMESYSTICK 10 // ms

volatile int bandera = 0;

typedef struct {
	uint8_t puerto;
	uint32_t pin;
	uint8_t func; // func del pin
	uint8_t io;
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = { { 0, 0, FUNC_0, OUTPUT },
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

void configSYSTICK(void) {
	// COnfigurar para interrumpir cada 10 ms
	SYSTICK_InternalInit(TIMESYSTICK);
	SYSTICK_Cmd(ENABLE); // habilito
	SYSTICK_IntCmd(ENABLE); // habilito interrupcion
}

void SysTick_Handler(void) {
	bandera ^= 1;
	SYSTICK_ClearCounterFlag(); // limpio bandera por las dudas
}

int main(void) {
	SystemInit();
	configPUERTOS();
	configSYSTICK();

	while (1) {
		switch(bandera){
		case 0: // apago
			GPIO_ClearValue(0, 1<<0);
			break;
		case 1: // prendo
			GPIO_SetValue(0, 1<<0);
			break;
		}

	}
	return 0;
}