/* Escriba un programa que utilice una interrupción externa (EINT1) para incrementar un contador binario
 de 4 bits que se muestra en 4 LEDs. El contador debe resetearse a cero automáticamente cada 2
 segundos, utilizando el SysTick.
 • Pines: Pulsador en EINT1, 4 LEDs en un puerto GPIO.
 • Lógica:
 o La interrupción externa (EINT1) incrementa el contador.
 o El SysTick se configura para generar un reset de la cuenta cada 2 segundos.
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

volatile int cuenta_int = 0; // cuenta de interrupciones
volatile int contador = 0; // contador de pulsador

typedef struct {
	uint8_t puerto;
	uint32_t pin;
	uint8_t func; // func del pin
	uint8_t io;
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = { { 0, 1, FUNC_0, OUTPUT }, // led
		{ 0, 2, FUNC_0, OUTPUT }, // led
		{ 0, 3, FUNC_0, OUTPUT }, // led
		{ 0, 4, FUNC_0, OUTPUT }, // led
		{ 2, 11, FUNC_1, INPUT }, // EINT1
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

void configEINT(void) {
	EXTI_Init();
	EXTI_InitTypeDef config_eint;
	config_eint.EXTI_Line = EXTI_EINT1;
	config_eint.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	config_eint.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE; // flanco de bajada
	EXTI_Config(&config_eint);
	NVIC_EnableIRQ(EINT1_IRQn);
}

void EINT1_IRQHandler(void) {
	contador = (contador + 1) % 16; // 2**4 -> cuenta hasta 16
	EXTI_ClearEXTIFlag(EXTI_EINT1); // limpio bandera
}

void configSYSTICK(void) {
	// COnfigurar para interrumpir cada 100 ms
	SYSTICK_InternalInit(TIMESYSTICK);
	SYSTICK_Cmd(ENABLE); // habilito
	SYSTICK_IntCmd(ENABLE); // habilito interrupcion
}

void SysTick_Handler(void) {
	cuenta_int++;
	if (cuenta_int == 20) { // ingresa cada dos segundos -> 20 int de 100 ms

		for (int i = 0; i < NUM_PUERTOS; i++) {
			GPIO_ClearValue(puert[i].puerto, 1 << puert[i].pin);
		}
		cuenta_int = 0;
	}
	SYSTICK_ClearCounterFlag(); // limpio bandera por las dudas
}

int main(void) {
	SystemInit();
    configEINT();
	configPUERTOS();
	configSYSTICK();

	while (1) {

	}
	return 0;
}