// Ejercicios con drivers

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#endif

#define FRECUENCIA_ADC 60000 // el doble de la maxima q es 30khz
#define FUNC_0 0
#define FUNC_1 1
#define FUNC_2 2
#define FUNC_3 3
#define MAX_MUESTRAS 16

GPDMA_LLI_Type dmaLista_ADC1 = {0};
GPDMA_LLI_Type dmaLista_ADC2 = {0};
uint32_t *buffer1 = (uint32_t *)0x20070000;
uint32_t *buffer2 = (uint32_t *)0x20080000;
volatile int buf = 0;
#include <cr_section_macros.h>

typedef struct {
  uint8_t puerto;
  uint8_t pin;
  uint8_t func; // convertor
} Pines;

Pines pin_led[] = {
    {0, 25, FUNC_1}, // Pin canal entrada
    {0, 26, FUNC_2}, // Pin salida DAC
};

const int NUMERO_LED = sizeof(pin_led) / sizeof(pin_led[0]);

void configGPIO() {
  for (int i = 0; i < NUMERO_LED; i++) {
    PINSEL_CFG_Type pinc_conf;
    pinc_conf.Portnum = pin_led[i].puerto;
    pinc_conf.Pinnum = pin_led[i].pin;
    pinc_conf.Funcnum = pin_led[i].func;
    pinc_conf.Pinmode = PINSEL_PINMODE_TRISTATE;
    pinc_conf.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&pinc_conf);
  }
}

void configADC() {
  ADC_Init(LPC_ADC, FRECUENCIA_ADC);
  ADC_BurstCmd(LPC_ADC, ENABLE); // habilito el modo burst
  ADC_ChannelCmd(LPC_ADC, 2, ENABLE);
}

void configDAC() {
  DAC_Init(LPC_DAC); // pone todo en 0
  DAC_CONVERTER_CFG_Type config_dac;
  config_dac.DMA_ENA = 1;
  config_dac.CNT_ENA = 0;
  config_dac.DBLBUF_ENA = 0;
  DAC_ConfigDAConverterControl(LPC_DAC, &config_dac);
  
}

void configDMA() {
  GPDMA_Channel_CFG_Type dma_config;
  dma_config.ChannelNum = 0;
  dma_config.TransferSize = 16;
  dma_config.TransferWidth = 16;
  dma_config.SrcMemAddr = LPC_ADC->ADGDR;
  dma_config.DstMemAddr = (uint32_t)buffer1;
  dma_config.TransferType = GPDMA_TRANSFERTYPE_P2M;
  dma_config.SrcConn = GPDMA_CONN_ADC;
  dma_config.DMALLI = &dmaLista_ADC1;

  dmaLista_ADC1.SrcAddr = LPC_ADC->ADGDR;
  dmaLista_ADC1.DstAddr = (uint32_t)buffer1;
  dmaLista_ADC1.NextLLI = &dmaLista_ADC2;
  dmaLista_ADC1.Control = MAX_MUESTRAS | 1 << 15 | 1 << 18 | 1 << 27 | 1 << 31;

  dmaLista_ADC2.SrcAddr = LPC_ADC->ADGDR;
  dmaLista_ADC2.DstAddr = (uint32_t)buffer2;
  dmaLista_ADC2.NextLLI = &dmaLista_ADC1;
  dmaLista_ADC2.Control = MAX_MUESTRAS | 1 << 15 | 1 << 18 | 1 << 27 | 1 << 31;

  GPDMA_init();
  GPDMA_Setup(&dma_config);
  GPDMA_ChannelCmd(0, ENABLE);
  NVIC_EnableIRQ(DMA_IRQn);
}

void DMA_IRQHandler(void) {
  uint32_t total = 0;
  uint32_t promedio = 0; // o float?
  for (int i = 0; i < MAX_MUESTRAS; i++) {
    switch (buf) {
    case 0:
      total += (buffer1[i]>>4) & 0xFFF;
      break;
    case 1:
      total += (buffer2[i]>>4)&0xFFF;
      break;
    }
  }
  promedio = (total / MAX_MUESTRAS)>>2;
  DAC_UpdateValue(LPC_DAC, promedio);
  buf ^=1; // cambio de buffer para la proxima
  GPDMA_ClearIntPending(GPDMA_STATCLR_INTERR,0);
}

int main(void) {
  SystemInit();
  configDAC();
  configADC();
  configDMA();

  while (1) {
  }
  return 0;
}
