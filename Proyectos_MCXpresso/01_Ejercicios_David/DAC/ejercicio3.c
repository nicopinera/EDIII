/* • Consigna: Desarrolle un programa que simule un "pseudo-contador" de 10 bits en la salida del DAC.
 *
 * Lógica:
 * La salida del DAC debe aumentar gradualmente de 0% a 100% del Vref.
 * Al llegar al 100% del Vref (valor máximo), la salida debe disminuir bruscamente a 0% y reiniciar el ciclo.
 *
 * Condiciones:
 * El tiempo de incremento (0% a 100%) debe ser de 2 segundos.
 * El tiempo de decremento (100% a 0%) debe ser instantáneo (por la escritura del nuevo valor).
 * La temporización debe ser controlada por el Timer 2 configurado para generar una
 * interrupción match a una frecuencia que te permita completar los 1024 pasos en 2 segundos.
 * Calcule la frecuencia de interrupción necesaria para lograr los 2 segundos exactos.
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
#define MAX_MUESTRAS 1024

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

	valor_precarga = 10000; // genera interrupciones cada 2 ms

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
	DAC_UpdateValue(LPC_DAC, muestra_actual);
	muestra_actual = (muestra_actual + 1) % MAX_MUESTRAS;
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
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