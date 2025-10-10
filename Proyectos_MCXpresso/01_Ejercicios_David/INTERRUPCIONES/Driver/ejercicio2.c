/* Escriba un programa muy sencillo que utilice un pin del Puerto 2 como interrupción externa (EINT0).
 * Cada vez que se active la interrupción (por flanco de bajada), un LED conectado a otro pin debe
 * cambiar de estado (encenderse si estaba apagado, y apagarse si estaba encendido).
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_exti.h"
#endif

#define FUNC_0 0
#define FUNC_1 1
#define FUNC_2 2
#define FUNC_3 3
#define OUTPUT 4
#define INPUT 5
#define DELAYTIME 1000000

volatile uint8_t contador = 0;
const uint32_t digits[16] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
		0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71 };

typedef struct {
	uint8_t puerto;
	uint32_t pin;
	uint8_t func; // func del pin
	uint8_t io;
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = { { 2, 0, FUNC_0, OUTPUT }, // 7 segmentos
		{ 2, 1, FUNC_0, OUTPUT }, // 7 segmentos
		{ 2, 2, FUNC_0, OUTPUT }, // 7 segmentos
		{ 2, 3, FUNC_0, OUTPUT }, // 7 segmentos
		{ 2, 4, FUNC_0, OUTPUT }, // 7 segmentos
		{ 2, 5, FUNC_0, OUTPUT }, // 7 segmentos
		{ 2, 6, FUNC_0, OUTPUT }, // 7 segmentos
		{ 0, 0, FUNC_0, INPUT }, //p0.0 GPIO entrada para interrupcion
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
	// COnfigurar Interrupcion para P0.0 por flanco de subida
	GPIO_IntCmd(0, 1<<0, 0); // puerto 0, pin 0, flanco de subida
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void) {
	if(GPIO_GetIntStatus(0, 1<<0, 0)){
		contador = (contador+1)%16;
	}
	GPIO_ClearInt(0, 1<<0); // limpio bandera
}

int main(void) {
	SystemInit();
	configPUERTOS();
	configEINT();

	while (1) {
		LPC_GPIO2->FIOPIN = digits[contador];
	}
	return 0;
}
