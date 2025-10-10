/* Cree un generador de PWM donde el ciclo de trabajo se controle con un botón.
 * El PWM debe iniciar con un 50% de ciclo de trabajo. Cada vez que se presione el botón, el ciclo de trabajo debe incrementarse en un 10%.
 * Al alcanzar el 100%, la siguiente pulsación debe reiniciar el ciclo de trabajo a 0%.
 *
 * Condiciones:
 * Utilice el Timer2 para generar la señal PWM.
 * Conecte el botón a un pin GPIO con interrupción externa para controlar el incremento del ciclo de trabajo.
 * El periodo de la onda debe ser fijo, mientras que el ancho del pulso debe ser ajustable.

 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"
#endif

#define FUNC_0 0
#define FUNC_1 1
#define FUNC_2 2
#define FUNC_3 3
#define TIMERMATCH0 10000000 // valor del registro match para periodo

volatile uint32_t match1_reg = TIMERMATCH / 2; // inicia al 50%

typedef struct {
	uint8_t puerto;
	uint32_t pin;
	uint8_t func; // func del pin
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = { { 1, 28, FUNC_3 }, // MAT0.0
		{ 2, 10, FUNC_1 }, // EINT0 -> controla l ciclo de trabajo
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
	}
}

void configEINT(void) {
	EXTI_Init();
	EXTI_InitTypeDef config_ext;
	config_ext.EXTI_Line = EXTI_EINT0;
	config_ext.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	config_ext.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
	EXTI_Config(&config_ext);
	NVIC_EnableIRQ(EINT0_IRQn);
}
void EINT0_IRQHandler(void) {
	match1_reg = (int) (match1_reg + (TIMERMATCH0 * 0.10)); // incremento 10%
	if (match1_reg >= TIMERMATCH0) { // verifico si llego al valor maximo
		match1_reg = 0; // reinicio
	}
	TIM_UpdateMatchValue(LPC_TIM0, 1, match1_reg); // actualizo el valor del match en el canal 1 que maneja el duty
	EXTI_ClearEXTIFlag(EXTI_EINT0); // limpio bandera
}

void configTIMER(void) {
	TIM_TIMERCFG_Type config_pre;
	config_pre.PrescaleOption = TIM_PRESCALE_TICKVAL;
	config_pre.PrescaleValue = 0;
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &config_pre); // configuro el pre scaler
	TIM_MATCHCFG_Type config_match;

	// canal 0 -> periodo fijo de la onda -> prende el led cuando inicia le periodo
	config_match.MatchChannel = 0; // canal 0
	config_match.IntOnMatch = DISABLE;
	config_match.StopOnMatch = DISABLE;
	config_match.ResetOnMatch = ENABLE;
	config_match.ExtMatchOutputType = TIM_EXTMATCH_HIGH;
	config_match.MatchValue = TIMERMATCH0;
	TIM_ConfigMatch(LPC_TIM0, &config_match); // configuro el match

	// canal 1 -> duty - apaga el led
	config_match.MatchChannel = 1; // canal 0
	config_match.IntOnMatch = DISABLE;
	config_match.StopOnMatch = DISABLE;
	config_match.ResetOnMatch = DISABLE;
	config_match.ExtMatchOutputType = TIM_EXTMATCH_LOW;
	config_match.MatchValue = match1_reg;
	TIM_ConfigMatch(LPC_TIM0, &config_match); // configuro el match
	TIM_Cmd(LPC_TIM0, ENABLE); // prendo el contador
}

int main(void) {
	SystemInit();
	configPUERTOS();
	configTIMER();
	configEINT();
	while (1) {

	}
	return 0;
}
