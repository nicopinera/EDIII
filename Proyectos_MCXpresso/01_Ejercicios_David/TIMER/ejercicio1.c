/* Desarrolle un programa que configure un Timer para hacer parpadear un LED
 cada 1 segundo, utilizando la función de Match para alternar el estado del pin
 de salida. Condiciones: • Utilice el Timer0 de la LPC1769. • Conecte el LED al
 pin de match MR0.0 del Timer0.
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"
#endif

#define FUNC_0 0
#define FUNC_1 1
#define FUNC_2 2
#define FUNC_3 3
#define TIMERMATCH 5000000 // valor del registro match

typedef struct {
  uint8_t puerto;
  uint32_t pin;
  uint8_t func; // func del pin
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = {
    {1, 28, FUNC_3}, // MAT0.0
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

void configTIMER(void) {
  TIM_TIMERCFG_Type config_pre;
  config_pre.PrescaleOption = TIM_PRESCALE_TICKVAL;
  config_pre.PrescaleValue = 4; // 4 ticks aumenta en 1 el timer
  TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &config_pre); // configuro el pre scaler
  TIM_MATCHCFG_Type config_match;
  config_match.MatchChannel = 0; // canal 0
  config_match.IntOnMatch = DISABLE;
  config_match.StopOnMatch = DISABLE;
  config_match.ResetOnMatch = ENABLE;
  config_match.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;
  config_match.MatchValue = TIMERMATCH;
  TIM_ConfigMatch(LPC_TIM0, &config_match); // configuro el match
  TIM_Cmd(LPC_TIM0, ENABLE);                // prendo el contador
}

int main(void) {
  SystemInit();
  configPUERTOS();
  configTIMER();

  while (1) {
  }
  return 0;
}
