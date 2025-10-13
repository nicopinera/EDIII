/* Desarrolle un programa que controle el muestreo de 100 muestras del ADC.
 * La adquisición de datos es iniciada por una interrupción externa y
 * cronometrada por un temporizador.
 *
 * Pines:
 * Entrada ADC: Utilice el pin AD0.0 (P0.23) para la señal analógica.
 * Botón de Control (E/S): Utilice el pin P2.10 configurado como interrupción
 * externa EINT0. LED de Estado: Utilice el pin P0.22 para indicar el estado del
 * sistema: encendido si el ADC está activo y muestreando, y apagado si está
 * inactivo.
 *
 * Lógica de Muestreo:
 * Activación: El primer flanco de bajada en el botón (EINT0) debe:
 * 		Encender el LED de Estado.
 * 		Configurar e iniciar un Timer  para generar una interrupción o
 * match cada 50 ms. Habilitar el ADC para una sola conversión por evento de
 * Timer. Muestreo: Cada evento de match del Timer 1 debe disparar una nueva
 * conversión del ADC. Almacenamiento: El resultado de cada conversión del ADC
 * debe ser leído por el CPU y almacenado secuencialmente en un array de 100
 * posiciones en la SRAM Bank 1. Finalización Normal: Después de la muestra
 * número 100, el Timer 1 y el ADC deben ser deshabilitados, y el LED de Estado
 * debe apagarse.
 *
 * Lógica de Control (Botón de Pausa/Reinicio):
 * Si se detecta un segundo flanco de bajada en el botón mientras la adquisición
 * está en curso (antes de llegar a 100 muestras): El muestreo debe detenerse
 * inmediatamente. El Timer 1 y el ADC deben ser deshabilitados. El LED de
 * Estado debe apagarse. El sistema queda listo para un nuevo inicio.
 *
 * Condiciones:
 * Asegure que las muestras de 10 bits del ADC se almacenen correctamente en un
 * array de enteros de 16 o 32 bits, garantizando la correcta alineación de los
 * bits de datos (0-11 o 0-12). La gestión de la cuenta y el toggling del estado
 * debe manejarse completamente dentro de la ISR de EINT0 y la ISR del Timer 1.
 *
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_adc.h"
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
#define TIMER_MATCH_PERIODO 250000 // Periodo - 50 ms
#define PRE_SCALER 4               // Ticks del prescaler
#define MAX_MUESTRAS 100

uint16_t *buffer_muestras = (uint16_t *)0x20080000;

volatile int cant_muestras_act = 0;
volatile int en_funcionamiento = 0; // 0 -> no funciona - 1 -> funciona

// configuracion
typedef struct {
  uint8_t puerto;
  uint32_t pin;
  uint8_t func; // func del pin
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert_adc[] = {
    {0, 23, FUNC_1}, // AD0.0
};

Puertos_t puerto_config[] = {
    {2, 10, FUNC_1}, // EINT0
    {0, 22, FUNC_0}, // Led Rojo -> Se prende por bajo
};

const int NUM_PUERTOS_ADC = sizeof(puert_adc) / sizeof(puert_adc[0]);
const int NUM_PUERTOS_LED = sizeof(puerto_config) / sizeof(puerto_config[0]);

void configPUERTOS(void) {
  PINSEL_CFG_Type pin;

  // Configuración de pines para el adc
  for (int i = 0; i < NUM_PUERTOS_ADC; i++) {
    pin.Portnum = puert_adc[i].puerto;
    pin.Pinnum = puert_adc[i].pin; // obtiene el número del pin
    pin.Funcnum = puert_adc[i].func;
    pin.Pinmode = PINSEL_PINMODE_TRISTATE;
    pin.OpenDrain = 0;
    PINSEL_ConfigPin(&pin);
  }
  for (int i = 0; i < NUM_PUERTOS_LED; i++) {
    pin.Portnum = puerto_config[i].puerto;
    pin.Pinnum = puerto_config[i].pin; // obtiene el número del pin
    pin.Funcnum = puerto_config[i].func;
    pin.Pinmode = 0;
    pin.OpenDrain = 0;
    PINSEL_ConfigPin(&pin);
  }
  GPIO_SetDir(0, 1 << 22, 1); // salida
}

void configADC(void) {
  ADC_Init(LPC_ADC, 200000);
  ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);         // Inicia con mat0.1
  ADC_ChannelCmd(LPC_ADC, 0, ENABLE);                // habilito canal 0
  ADC_EdgeStartConfig(LPC_ADC, ADC_START_ON_RISING); // flanco de subida
  ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, ENABLE); // habilito interrupcion canal 0
  NVIC_EnableIRQ(ADC_IRQn);
}

void configEINT0(void) {
  EXTI_InitTypeDef config_eint;
  config_eint.EXTI_Line = EXTI_EINT0;               // externa 0
  config_eint.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE; // por flancos
  config_eint.EXTI_polarity =
      EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE; // flanco de bajada
  EXTI_Init();
  EXTI_Config(&config_eint);
  NVIC_EnableIRQ(EINT0_IRQn);
}

void configTIMER(void) {
  TIM_TIMERCFG_Type config_pre;
  config_pre.PrescaleOption = TIM_PRESCALE_TICKVAL;
  config_pre.PrescaleValue = PRE_SCALER;
  TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &config_pre); // configuro el pre scaler

  TIM_MATCHCFG_Type config_match;
  config_match.MatchChannel = 1; // canal 1 para lanzar el ADC
  config_match.IntOnMatch = DISABLE;
  config_match.StopOnMatch = DISABLE;
  config_match.ResetOnMatch = ENABLE;
  config_match.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE; // Toggle
  config_match.MatchValue = (TIMER_MATCH_PERIODO / 2);
  TIM_ConfigMatch(LPC_TIM0, &config_match); // configuro el match
}

void EINT0_IRQHandler() {
  en_funcionamiento ^= 1; // pongo en funcionamiento o no
  if (en_funcionamiento) {
    TIM_Cmd(LPC_TIM0, ENABLE);   // prendo el contador
    GPIO_ClearValue(0, 1 << 22); // prendo el led
  } else if (en_funcionamiento == 0 && cant_muestras_act < 100) {
    GPIO_SetValue(0, 1 << 22);  // Apago el led
    TIM_Cmd(LPC_TIM0, DISABLE); // apago el timer
    for (int i = 0; i < MAX_MUESTRAS; i++) {
      buffer_muestras[i] = 0; // borro el buffer de 100 muestras
    }
    cant_muestras_act = 0;
  }
}

void ADC_IRQHandler(void) {
  uint32_t valor_adc = ADC_ChannelGetData(LPC_ADC, 0);
  buffer_muestras[cant_muestras_act] = (uint16_t)valor_adc;
  if (cant_muestras_act == MAX_MUESTRAS) {
    GPIO_SetValue(0, 1 << 22);  // Apago el led
    TIM_Cmd(LPC_TIM0, DISABLE); // apago el timer
    cant_muestras_act = 0;
  }
  cant_muestras_act++;
}

int main(void) {
  SystemInit();
  configPUERTOS();
  configTIMER();
  configADC();
  configEINT0();
  while (1) {
  }
  return 0;
}
