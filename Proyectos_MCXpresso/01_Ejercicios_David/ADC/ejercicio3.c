/* Desarrolle un programa que permita al usuario controlar de forma independiente la intensidad (brillo) de los tres colores
 * (Rojo, Verde, Azul) de un LED RGB, utilizando tres potenciómetros. La modulación de intensidad debe implementarse mediante PWM.
 *
 * Pines:
 * Entradas ADC (Potenciómetros): Utilice tres canales de ADC disponibles, AD0.0 (P0.23), AD0.1 (P0.24) y AD0.2 (P0.25).
 * Salidas PWM (LED RGB): Utilice los pines de Match del Timer 2 (que soporta hasta 4 canales de Match) para generar las señales PWM.
 * Asigne MR2.0, MR2.1 y MR2.2 para controlar los pines del LED RGB (Rojo, Verde, Azul).
 *
 *  Lógica:
 *  Configure el ADC para escanear y convertir los tres canales de entrada de forma secuencial.
 *  Configure el Timer 2 para una frecuencia PWM fija.
 *  El valor de 10 bits de cada lectura de ADC debe ser mapeado y utilizado directamente para establecer el ciclo de trabajo
 * (duty cycle) de la señal PWM correspondiente a cada color.
 *  	ADC0.0 → Controla el Duty Cycle de PWM (MR2.0) - Color Rojo.
 *  	ADC0.1 → Controla el Duty Cycle de PWM (MR2.1) - Color Verde.
 *  	ADC0.2 → Controla el Duty Cycle de PWM (MR2.2) - Color Azul.
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
#define TIMER_MATCH_PERIODO 500000 // Periodo - 100 ms - las muestras
#define PRE_SCALER 4               // Ticks del prescaler

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
    {0, 6, FUNC_3}, // MAT2.0 -> Rojo
    {0, 7, FUNC_3}, // MAT2.1 -> Verde
    {0, 8, FUNC_3}, // MAT2.2 -> Azul
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
    TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &config_pre); // configuro el pre scaler

    TIM_MATCHCFG_Type config_match;
    config_match.MatchChannel = 1; // canal 1 para lanzar el ADC
    config_match.IntOnMatch = DISABLE;
    config_match.StopOnMatch = DISABLE;
    config_match.ResetOnMatch = ENABLE;
    config_match.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE; // Toggle
    config_match.MatchValue = (TIMER_MATCH_PERIODO / 2);
    TIM_ConfigMatch(LPC_TIM0, &config_match); // configuro el match

    // Configurar Timer 2, canal 0 - 1 - 2
    for (int i = 0; i < 3; i++)
    {
        config_match.MatchChannel = i;
        config_match.IntOnMatch = DISABLE;
        config_match.StopOnMatch = DISABLE;
        switch (i)
        {
        case 0:
        case 1:
        case 2:
            config_match.ResetOnMatch = DISABLE;
            config_match.ExtMatchOutputType = TIM_EXTMATCH_LOW;  // Lo baja
            config_match.MatchValue = (TIMER_MATCH_PERIODO / 2); // Empezamos con 50%
            break;
        case 3: // Controla el periodo de la señal - 100 ms
            config_match.ResetOnMatch = ENABLE;
            config_match.ExtMatchOutputType = TIM_EXTMATCH_HIGH; // Pone en alto
            config_match.MatchValue = (TIMER_MATCH_PERIODO);     // cada 100 ms
            break;
        }
        TIM_ConfigMatch(LPC_TIM2, &config_match); // configuro el match
    }

    TIM_Cmd(LPC_TIM0, ENABLE); // prendo el contador
    TIM_Cmd(LPC_TIM2, ENABLE); // prendo el contador
}

void ADC_IRQHandler(void)
{
    ADC_ChannelCmd(LPC_ADC, canal_activo, DISABLE); // deshabilito el canal activo

    float max_val = 4095.0f; // maxima escala del adc
    uint32_t valor_adc = ADC_ChannelGetData(LPC_ADC, canal_activo);
    uint32_t valor_final = (uint32_t)((valor_adc * TIMER_MATCH_PERIODO) / max_val);

    switch (canal_activo)
    {
    case 0: // canal 0
        TIM_UpdateMatchValue(LPC_TIM2, canal_activo, valor_final);
        break;
    case 1: // canal 1
        TIM_UpdateMatchValue(LPC_TIM2, canal_activo, valor_final);
        break;
    case 2: // canal 2
        TIM_UpdateMatchValue(LPC_TIM2, canal_activo, valor_final);
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