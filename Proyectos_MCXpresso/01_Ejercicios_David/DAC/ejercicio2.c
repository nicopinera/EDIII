/* Consigna: Escriba un programa para generar una onda sinusoidal de baja frecuencia y
 * demuestre el uso de la funcionalidad de bajo consumo del DAC.
 *
 * Lógica:
 * Defina un array en la memoria con al menos 32 valores discretos que representen los puntos de una onda sinusoidal completa.
 * Configure el DAC en modo de bajo consumo.
 * Configure el Timer 1 para generar una interrupción que se active a una frecuencia constante (la frecuencia de muestreo).
 *
 * Condiciones:
 * La lógica de actualización del DAC (leer el siguiente valor del array y escribirlo en el registro DACR)
 * debe residir completamente en el Handler de Interrupción del Timer 1.
 * La onda sinusoidal debe tener una frecuencia final de 50 Hz.
 *
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#endif

#define FUNC_0 0
#define FUNC_1 1
#define FUNC_2 2
#define FUNC_3 3
#define PRE_SCALER 4 // Ticks del prescaler
#define MAX_MUESTRAS 32
#define F_ONDA 50

uint16_t buffer[MAX_MUESTRAS] = { 512, 611, 707, 796, 873, 937, 984, 1013, 1023,
		1013, 984, 937, 873, 796, 707, 611, 512, 412, 316, 227, 150, 86, 39, 10,
		0, 10, 39, 86, 150, 227, 316, 412 };

volatile int muestra_actual = 0;
uint32_t valor_precarga = 0;
// configuracion
typedef struct {
	uint8_t puerto;
	uint32_t pin;
	uint8_t func; // func del pin
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert_DAC[] = { { 0, 26, FUNC_2 }, // AOUT
		};

const int NUM_PUERTOS_DAC = sizeof(puert_DAC) / sizeof(puert_DAC[0]);

void configPUERTOS(void) {
	PINSEL_CFG_Type pin;

	// Configuración de pines para el DAC
	for (int i = 0; i < NUM_PUERTOS_DAC; i++) {
		pin.Portnum = puert_DAC[i].puerto;
		pin.Pinnum = puert_DAC[i].pin; // obtiene el número del pin
		pin.Funcnum = puert_DAC[i].func;
		pin.Pinmode = PINSEL_PINMODE_TRISTATE;
		pin.OpenDrain = 0;
		PINSEL_ConfigPin(&pin);
	}
}

void configDAC(void) {
	DAC_Init(LPC_DAC);
	DAC_SetBias(LPC_DAC, 1); // bajo consumo - Ftrabajo 400khz
}

void configTIMER(void) {
	TIM_TIMERCFG_Type config_pre;
	config_pre.PrescaleOption = TIM_PRESCALE_TICKVAL;
	config_pre.PrescaleValue = PRE_SCALER;
	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &config_pre); // configuro el pre scaler

	valor_precarga = (SystemCoreClock / 4) / (F_ONDA * MAX_MUESTRAS);

	TIM_MATCHCFG_Type config_match;
	config_match.MatchChannel = 0; // canal 1 para lanzar el ADC
	config_match.IntOnMatch = ENABLE;
	config_match.StopOnMatch = DISABLE;
	config_match.ResetOnMatch = ENABLE;
	config_match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING; // Nada
	config_match.MatchValue = (valor_precarga);
	TIM_ConfigMatch(LPC_TIM1, &config_match); // configuro el match
	NVIC_EnableIRQ(TIMER1_IRQn);

}

void TIMER1_IRQHandler() {
	DAC_UpdateValue(LPC_DAC, buffer[muestra_actual]);
	muestra_actual = (muestra_actual + 1) % MAX_MUESTRAS;
}

int main(void) {
	SystemInit();
	configPUERTOS();
	configTIMER();
	configDAC();
	while (1) {

	}
	return 0;
}