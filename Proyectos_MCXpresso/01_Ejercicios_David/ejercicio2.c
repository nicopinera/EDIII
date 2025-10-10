// Ejercicios con drivers

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include  "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"
#endif

#define FRECUENCIA_ADC 60000 // el doble de la maxima q es 30khz
#define ES_ADC 1
#define ES_DAC 0
#define MAX_MUESTRAS 16

GPDMA_LLI_Type dmaLista_ADC1 = { 0 };
GPDMA_LLI_Type dmaLista_ADC2 = { 0 };
uint16_t *buffer1 = (uint16_t*) 0x20070000;
uint16_t *buffer2 = (uint16_t*) 0x20080000;
volatile int buf = 0;
#include <cr_section_macros.h>

typedef struct {
	uint8_t puerto;
	uint8_t pin;
	uint8_t con; // convertor
} Pines;

Pines pin_led[] = { { 0, 25, ES_ADC }, // Pin canal entrada
		{ 0, 26, ES_DAC }, // Pin salida DAC
		};

const int NUMERO_LED = sizeof(pin_led) / sizeof(pin_led[0]);
volatile int cuentas_mayor_60 = 0;

void configGPIO() {
	// COnfigurar 3 pines de salida para leds
	for (int i = 0; i < NUMERO_LED; i++) {
		PINSEL_CFG_Type pin_led_config;
		pin_led_config.Portnum = pin_led[i].puerto;
		pin_led_config.Pinnum = pin_led[i].pin;
		if (pin_led_config.con == ES_ADC) {
			pin_led_config.Funcnum = PINSEL_FUNC_1; // canal 2 del adc

		} else if (pin_led_config.con == Es_DAC) { // AOUT del DAC
			pin_led_config.Funcnum = PINSEL_FUNC_2;
		}
		pin_led_config.Pinmode = PINSEL_PINMODE_TRISTATE;
		pin_led_config.OpenDrain = PINSEL_PINMODE_NORMAL;
		PINSEL_ConfigPin(&pin_led_config);
	}

}

void configADC() {
	ADC_Init(LPC_ADC, FRECUENCIA_ADC);
	ADC_BurstCmd(LPC_ADC, ENABLE); // habilito el modo burst
	ADC_ChannelCmd(LPC_ADC, 2, ENABLE);
}

void configDAC() {
	DAC_CONVERTER_CFG_Type config_dac;
	config_dac.DMA_ENA = 1;
	config_dac.CNT_ENA = 0;
	config_dac.DBLBUF_ENA = 0;
	DAC_ConfigDAConverterControl(LPC_DAC, &config_dac);
	DAC_Init(LPC_DAC);

}

void configDMA() {
	GPDMA_Channel_CFG_Type dma_config;
	dma_config.ChannelNum = 0;
	dma_config.TransferSize = 16;
	dma_config.TransferWidth = 16;
	dma_config.SrcMemAddr = LPC_ADC->ADGDR;
	dma_config.DstMemAddr = (uint32_t) buffer1;
	dma_config.TransferType = GPDMA_TRANSFERTYPE_P2M;
	dma_config.SrcConn = GPDMA_CONN_ADC;
	dma_config.DMALLI = &dmaLista_ADC1;

	dmaLista_ADC1.SrcAddr = LPC_ADC->ADGDR;
	dmaLista_ADC1.DstAddr = (uint32_t) buffer1;
	dmaLista_ADC1.NextLLI = &dmaLista_ADC2;
	dmaLista_ADC1.Control = 16 | 1 << 15 | 1 << 18 | 1 << 27 | 1 << 31;

	dmaLista_ADC2.SrcAddr = LPC_ADC->ADGDR;
	dmaLista_ADC2.DstAddr = (uint32_t) buffer2;
	dmaLista_ADC2.NextLLI = &dmaLista_ADC1;
	dmaLista_ADC2.Control = 16 | 1 << 15 | 1 << 18 | 1 << 27 | 1 << 31;

	GPDMA_init();
	GPDMA_Setup(&dma_config);
	GPDMA_ChannelCmd(0, ENABLE);
	NVIC_EnableIRQ(DMA_IRQn);
}

void DMA_IRQHandler(void) {
	int total = 0;
	int promedio = 0; // o float?
	for (int i = 0; i < MAX_MUESTRAS; i++) {
		switch (buf) {
		case 0:
			total += buffer1[i];
			break;
		case 1:
			total += buffer2[i];
			break;
		}
	}
	promedio = total / MAX_MUESTRAS;
	DAC_UpdateValue(LPC_DAC, promedio);
	buf = (buf + 1) % 2;
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
