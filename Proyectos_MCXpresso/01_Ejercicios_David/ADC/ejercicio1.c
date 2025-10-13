/* Escriba un programa que configure un canal del ADC para leer un voltaje variable.
 * Pines: Utilice el pin AD0.0 (P0.23) para la entrada analógica (conectado, por ejemplo, a un potenciómetro).
 * Salida: El valor de 12 bits leído debe ser escalado a un valor de 1 bit para encender o apagar un LED.
 * El LED debe encenderse si la lectura supera la mitad del rango (2048).
 *
 * Condiciones:
 * Configure el ADC para disparar con el MAT0.1 cada 50 ms.
 * No se deben usar interrupciones de timer.
 * */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_adc.h"
#endif

#define FUNC_0 0
#define FUNC_1 1
#define FUNC_2 2
#define FUNC_3 3
#define TIMERMATCH0 250000 // valor del registro match para periodo - periodo de 50 ms - debo cargar la mitad para toggle
#define PRE_SCALER 4       // Ticks del prescaler
#define MITAD_SCALA 2048   // Mitad de la escala del ADC

typedef struct
{
    uint8_t puerto;
    uint32_t pin;
    uint8_t func; // func del pin
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert[] = {
    {0, 0, FUNC_0}, // LED
};

const int NUM_PUERTOS = sizeof(puert) / sizeof(puert[0]);

void configPUERTOS(void)
{
    PINSEL_CFG_Type pin;

    // Configuración de pines
    for (int i = 0; i < NUM_PUERTOS; i++)
    {
        pin.Portnum = puert[i].puerto;
        pin.Pinnum = puert[i].pin; // obtiene el número del pin
        pin.Funcnum = puert[i].func;
        pin.Pinmode = 0;
        pin.OpenDrain = 0;
        PINSEL_ConfigPin(&pin);
        GPIO_SetDir(puert[0].puerto, 1 << puert[0].pin, 1); // salida
    }
    pin.Portnum = 0;
    pin.Pinnum = 23;
    pin.Funcnum = FUNC_1; // AD0.0
    pin.Pinmode = PINSEL_PINMODE_TRISTATE;
    pin.OpenDrain = 0;
    PINSEL_ConfigPin(&pin);
}

void configADC(void)
{
    ADC_Init(LPC_ADC, 200000);
    ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);         // Inicia con mat0.1
    ADC_ChannelCmd(LPC_ADC, 0, ENABLE);                // habilito canal 0
    ADC_EdgeStartConfig(LPC_ADC, ADC_START_ON_RISING); // flanco de subida
    ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, ENABLE);      // habilito interrupcion
    NVIC_EnableIRQ(ADC_IRQn);
}

void configTIMER(void)
{
    TIM_TIMERCFG_Type config_pre;
    config_pre.PrescaleOption = TIM_PRESCALE_TICKVAL;
    config_pre.PrescaleValue = PRE_SCALER;
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &config_pre); // configuro el pre scaler

    TIM_MATCHCFG_Type config_match;
    config_match.MatchChannel = 1; // canal 1
    config_match.IntOnMatch = DISABLE;
    config_match.StopOnMatch = DISABLE;
    config_match.ResetOnMatch = ENABLE;
    config_match.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE; // Toggle
    config_match.MatchValue = (TIMERMATCH0 / 2);
    TIM_ConfigMatch(LPC_TIM0, &config_match); // configuro el match
    TIM_Cmd(LPC_TIM0, ENABLE);                // prendo el contador
}
void ADC_IRQHandler(void)
{
    uint32_t valor_adc = ADC_ChannelGetData(LPC_ADC, 0); // tomo el dato de un canal - ya me lo devuelve >>4 y &0xFFF
    GPIO_ClearValue(puert[0].puerto, 1 << puert[0].pin);
    if (valor_adc >= MITAD_SCALA)
    {
        GPIO_SetValue(puert[0].puerto, 1 << puert[0].pin);
    }
}

int main(void)
{
    SystemInit();
    configPUERTOS();
    configTIMER();
    configADC();
    while (1)
    {
    }
    return 0;
}