// Ejercicios con drivers

// Configurar timer para que nos habilite el adc MAT0.1 (100ms) para pedir una
// muestra en funcion de la muestra si es de 0-40 prendo el verde - 41 a 60 para
// amarillo y si tengo 10 muestras seguidas mayor a 60 grados prendo el rojo

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#endif
#define FRECUENCIA_ADC 200000
#define MATH_VALOR 2500000

#include <cr_section_macros.h>

typedef struct {
  uint8_t puerto;
  uint8_t pin;
} Pines;

Pines pin_led[] = {
    {0, 0}, // led verde
    {0, 1}, // led amarillo
    {0, 2}, // led rojo
};

const int NUMERO_LED = sizeof(pin_led) / sizeof(pin_led[0]);
volatile int cuentas_mayor_60 = 0;

void configGPIO() {
  // COnfigurar 3 pines de salida para leds
  for (int i = 0; i < NUMERO_LED; i++) {
    PINSEL_CFG_Type pin_led_config;
    pin_led_config.Portnum = pin_led[i].puerto;
    pin_led_config.Pinnum = pin_led[i].pin;
    pin_led_config.Funcnum = PINSEL_FUNC_0;
    pin_led_config.Pinmode = PINSEL_PINMODE_PULLUP;
    pin_led_config.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&pin_led_config);
    GPIO_SetDir(pin_led[i].puerto, 1 << pin_led[i].pin,
                1); // definis como salida
    GPIO_ClearValue(pin_led[i].puerto, 1 << pin_led[i].pin); // limpias
  }
  // NO NECESITO CONFIGURAR EL PIN MAT0.1
  // Configurar la entrada del ADC
  PINSEL_CFG_Type pin_adc_config;
  pin_adc_config.Portnum = 0;
  pin_adc_config.Pinnum = 23;
  pin_adc_config.Funcnum = PINSEL_FUNC_1; // ADC0.0
  pin_adc_config.Pinmode = PINSEL_PINMODE_TRISTATE;
  pin_adc_config.OpenDrain = PINSEL_PINMODE_NORMAL;
  PINSEL_ConfigPin(&pin_adc_config);
}

void configTIMER() {
  TIM_TIMERCFG_Type config_pre;
  config_pre.PrescaleOption = 0;
  config_pre.PrescaleValue = 0;
  /*
   * Tambien puedo configurar el prescaler para que lo cuente en microsegundos
   * y le pongo un valor de 1000 para que aumente una cuenta cada 1 ms
   * y el match
   * */

  TIM_MATCHCFG_Type config_math;
  config_math.MatchChannel = 1;
  config_math.IntOnMatch = DISABLE;
  config_math.StopOnMatch = DISABLE;
  config_math.ResetOnMatch = ENABLE;
  config_math.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;
  config_math.MatchValue = MATH_VALOR / 2;

  TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &config_pre);
  TIM_ConfigMatch(LPC_TIM0, &config_math);
  TIM_Cmd(LPC_TIM0, ENABLE);
}

void configADC() {
  ADC_Init(LPC_ADC, FRECUENCIA_ADC);
  ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);
  ADC_ChannelCmd(LPC_ADC, 0, ENABLE);
  ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, ENABLE);
  NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler() {
  // Constantes
  const float ADC_MAX_VAL = 4096.0f; // Máximo valor del ADC de 12 bits

  // 1. Obtengo el valor y lo ajusto (esto ya lo tenías)
  uint32_t lectura_adc = ADC_GlobalGetData(LPC_ADC) >> 4;
  lectura_adc &= (0xfff);

  // 2. CONVERSIÓN A TEMPERATURA (°C)
  // Temp (°C) = (Lectura / ADC_MAX_VAL)
  float temp_float = ((float)lectura_adc / ADC_MAX_VAL);

  // 3. Convierto a entero para usarlo en las comparaciones
  uint32_t valor_t = (uint32_t)(temp_float * 100);

  GPIO_ClearValue(0, 1 << 0);
  GPIO_ClearValue(0, 1 << 1);
  GPIO_ClearValue(0, 1 << 2);

  if (valor_t <= 40) {
    // prendo led verde
    GPIO_SetValue(0, 1 << 0);
    cuentas_mayor_60 = 0;
  } else if (valor_t >= 41 && valor_t <= 60) {
    // prendo led amarillo
    GPIO_SetValue(0, 1 << 1);
    cuentas_mayor_60 = 0;
  } else {
    cuentas_mayor_60++;
    if (cuentas_mayor_60 >= 10) {
      // prendo led rojo
      GPIO_SetValue(0, 1 << 2);
    }
  }
}

int main(void) {
  SystemInit();
  configGPIO();
  configADC();

  while (1) {
  }
  return 0;
}
