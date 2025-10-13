/* Escriba un programa que lea tres canales analógicos distintos de forma secuencial.
 * Pines: Utilice los pines AD0.0 (P0.23), AD0.1 (P0.24) y AD0.2 (P0.25) como entradas.
 *
 * Lógica:
 * Utilice 4 LEDs para implementar un vúmetro por cada canal, de forma en que se encienda un LED adicional por cada 25% del nivel máximo.
 *
 * Condiciones:
 * Utilice un timer para disparar las conversiones de manera secuencial, con un intervalo entre conversión de 50 ms.
 *
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
#endif

#define FUNC_0 0
#define FUNC_1 1
#define FUNC_2 2
#define FUNC_3 3
#define TIMERMATCH0 250000 // valor del registro match para periodo - periodo de 50 ms - debo cargar la mitad para toggle
#define PRE_SCALER 4       // Ticks del prescaler

typedef struct
{
    uint8_t puerto;
    uint32_t pin;
    uint8_t func; // func del pin
} Puertos_t;

// Definimos en un arreglo
Puertos_t puert_adc[] = {
    {0, 23, FUNC_1}, // AD0.0
    {0, 24, FUNC_1}, // AD0.1
    {0, 25, FUNC_1}, // AD0.2
};

Puertos_t puerto_led[] = {
    {0, 0, FUNC_0},  // LED 1- Canal 0 - se prende si 25%<adc_val<50%
    {0, 1, FUNC_0},  // LED 2 - Canal 0 - se prende si 50%<adc_val<75%
    {0, 2, FUNC_0},  // LED 3 - Canal 0 - se prende si 75%<adc_val<100%
    {0, 3, FUNC_0},  // LED 4 - Canal 0 - se prende si adc_val==100%
    {0, 4, FUNC_0},  // LED 1- Canal 1 - se prende si 25%<adc_val<50%
    {0, 5, FUNC_0},  // LED 2 - Canal 1 - se prende si 50%<adc_val<75%
    {0, 6, FUNC_0},  // LED 3 - Canal 1 - se prende si 75%<adc_val<100%
    {0, 7, FUNC_0},  // LED 4 - Canal 1 - se prende si adc_val==100%
    {0, 8, FUNC_0},  // LED 1- Canal 2 - se prende si 25%<adc_val<50%
    {0, 9, FUNC_0},  // LED 2 - Canal 2 - se prende si 50%<adc_val<75%
    {0, 10, FUNC_0}, // LED 3 - Canal 2 - se prende si 75%<adc_val<100%
    {0, 11, FUNC_0}, // LED 4 - Canal 2 - se prende si adc_val==100%
};
int canal_activo = 0; // marca el canal activo 0,1,2
const int NUM_PUERTOS_ADC = sizeof(puert_adc) / sizeof(puert_adc[0]);
const int NUM_PUERTOS_LED = sizeof(puerto_led) / sizeof(puerto_led[0]);

void configPUERTOS(void)
{
    PINSEL_CFG_Type pin;

    // Configuración de pines para el adc
    for (int i = 0; i < NUM_PUERTOS_ADC; i++)
    {
        pin.Portnum = puert_adc[i].puerto;
        pin.Pinnum = puert_adc[i].pin; // obtiene el número del pin
        pin.Funcnum = puert_adc[i].func;
        pin.Pinmode = PINSEL_PINMODE_TRISTATE;
        pin.OpenDrain = 0;
        PINSEL_ConfigPin(&pin);
    }
    for (int i = 0; i < NUM_PUERTOS_LED; i++)
    {
        pin.Portnum = puerto_led[i].puerto;
        pin.Pinnum = puerto_led[i].pin; // obtiene el número del pin
        pin.Funcnum = puerto_led[i].func;
        pin.Pinmode = 0;
        pin.OpenDrain = 0;
        PINSEL_ConfigPin(&pin);
        GPIO_SetDir(puerto_led[i].puerto, 1 << puerto_led[i].pin, 1); // salida
    }
}

void configADC(void)
{
    ADC_Init(LPC_ADC, 200000);
    ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);         // Inicia con mat0.1
    ADC_ChannelCmd(LPC_ADC, 0, ENABLE);                // habilito canal 0
    ADC_EdgeStartConfig(LPC_ADC, ADC_START_ON_RISING); // flanco de subida
    ADC_IntConfig(LPC_ADC, ADC_ADGINTEN, ENABLE);      // habilito interrupcion global
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
    ADC_ChannelCmd(LPC_ADC, canal_activo, DISABLE); // deshabilito el canal activo

    // Apago todos los leds
    for (int i = 0; i < (canal_activo * 4 + 4); i++)
    {
        GPIO_ClearValue(puerto_led[i].puerto, 1 << puerto_led[i].pin);
    }

    float max_val = 4095.0f; // maxima escala del adc
    uint32_t valor_adc = ADC_ChannelGetData(LPC_ADC, canal_activo);
    uint32_t valor_final = (uint32_t)((valor_adc * 100.0f) / max_val);

    switch (canal_activo)
    {
    case 0: // canal 0
        if (valor_final >= 25)
        {
            GPIO_SetValue(puerto_led[0].puerto, 1 << puerto_led[0].pin); // prendo primer led
        }
        if (valor_final >= 50)
        {
            GPIO_SetValue(puerto_led[1].puerto, 1 << puerto_led[1].pin);
        }
        if (valor_final >= 75)
        {
            GPIO_SetValue(puerto_led[2].puerto, 1 << puerto_led[2].pin);
        }
        if (valor_final == 100)
        {
            GPIO_SetValue(puerto_led[3].puerto, 1 << puerto_led[3].pin);
        }
        break;
    case 1: // canal 1
        if (valor_final >= 25)
        {
            GPIO_SetValue(puerto_led[4].puerto, 1 << puerto_led[4].pin); // prendo primer led
        }
        if (valor_final >= 50)
        {
            GPIO_SetValue(puerto_led[5].puerto, 1 << puerto_led[5].pin);
        }
        if (valor_final >= 75)
        {
            GPIO_SetValue(puerto_led[6].puerto, 1 << puerto_led[6].pin);
        }
        if (valor_final == 100)
        {
            GPIO_SetValue(puerto_led[7].puerto, 1 << puerto_led[7].pin);
        }
        break;
    case 2: // canal 2
        if (valor_final >= 25)
        {
            GPIO_SetValue(puerto_led[8].puerto, 1 << puerto_led[8].pin); // prendo primer led
        }
        if (valor_final >= 50)
        {
            GPIO_SetValue(puerto_led[9].puerto, 1 << puerto_led[9].pin);
        }
        if (valor_final >= 75)
        {
            GPIO_SetValue(puerto_led[10].puerto, 1 << puerto_led[10].pin);
        }
        if (valor_final == 100)
        {
            GPIO_SetValue(puerto_led[11].puerto, 1 << puerto_led[11].pin);
        }
        break;
    }

    canal_activo = (canal_activo + 1) % 3;
    ADC_ChannelCmd(LPC_ADC, canal_activo, ENABLE); // Habilito el proximo canal
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