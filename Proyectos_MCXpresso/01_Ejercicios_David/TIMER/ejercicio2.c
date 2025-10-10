/* Escriba un programa que utilice un Timer para generar una onda cuadrada de 1 Hz.
 * Agregue un botón que, al ser presionado, duplique la frecuencia de la onda.
 * La frecuencia máxima debe ser de 1024 Hz, y la siguiente presión del botón debe reiniciar la frecuencia a 1 Hz.
 Condiciones:
 • Utilice un Timer de su elección y configure su canal de Match para generar la onda.
 • El botón debe estar conectado a un pin GPIO configurado con interrupción externa.
 • La frecuencia debe ser modificada en el Handler de la interrupción.
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
#define TIMERMATCH 12500000 // valor del registro match base para 1Hz - interrumpe en medio periodo para hacer el toggle

volatile int pulsacion = 0; // maximo 10 pulsaciones y reinicio
volatile int mr = TIMERMATCH;
typedef struct {
	uint8_t puerto;
	uint32_t pin;
	uint8_t func; // func del pin
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = { { 1, 28, FUNC_3 }, // MAT0.0
		{ 2, 10, FUNC_1 }, // EINT0
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

void configEINT(void){
	EXTI_Init();
	EXTI_InitTypeDef config_ext;
	config_ext.EXTI_Line = EXTI_EINT0;
	config_ext.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	config_ext.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
	EXTI_Config(&config_ext);
	NVIC_EnableIRQ(EINT0_IRQn);
}
void EINT0_IRQHandler(void){
	pulsacion ++;
	if(pulsacion == 10){
		mr = TIMERMATCH;
		pulsacion = 0; // reseteo
	}
	else{
		mr = mr/2; // divido el registro match
	}
	TIM_UpdateMatchValue(LPC_TIM0, 0, mr); // actualizo el valor del match
	EXTI_ClearEXTIFlag(EXTI_EINT0); // limpio bandera

}

void configTIMER(void) {
	TIM_TIMERCFG_Type config_pre;
	config_pre.PrescaleOption = TIM_PRESCALE_TICKVAL;
	config_pre.PrescaleValue = 0;
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &config_pre); // configuro el pre scaler
	TIM_MATCHCFG_Type config_match;
	config_match.MatchChannel = 0; // canal 0
	config_match.IntOnMatch = DISABLE;
	config_match.StopOnMatch = DISABLE;
	config_match.ResetOnMatch = ENABLE;
	config_match.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;
	config_match.MatchValue = TIMERMATCH;
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