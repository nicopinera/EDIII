#include "LPC17xx.h"

#define BUFFER_SIZE (8 * 1024)    // 8 KB
#define MAX_SAMPLES (BUFFER_SIZE) // 1 byte por tick (si alcanza)
#define GPIO_INT_PIN (1 << 19)    // Ejemplo: P2.13/EINT3 -> revisar según caso

volatile uint32_t tick_us = 0; // contador de ticks en us
volatile uint8_t buffer[BUFFER_SIZE];
volatile uint32_t index_buffer = 0;

void SysTick_Handler(void) { tick_us++; }

// --- Configuración de SysTick en base de 1us ---
void systick_init(void) {
  SysTick->LOAD = (SystemCoreClock / 1000000) - 1; // 1 us
  SysTick->VAL = 0;
  SysTick->CTRL =
      (1 << 0) | (1 << 1) | (1 << 2); // ENABLE, TICKINT, CLKSOURCE=CPU
}

// --- Configuración de GPIO interrupción ---
void gpio_int_init(void) {
  LPC_GPIOINT->IO2IntEnR |= GPIO_INT_PIN; // habilita flanco ascendente
  LPC_GPIOINT->IO2IntEnF |= GPIO_INT_PIN; // habilita flanco descendente
  NVIC_EnableIRQ(EINT3_IRQn);
}

// --- Handler de interrupción GPIO ---
void EINT3_IRQHandler(void) {
  if (LPC_GPIOINT->IO2IntStatR & GPIO_INT_PIN ||
      LPC_GPIOINT->IO2IntStatF & GPIO_INT_PIN) {
    if (index_buffer < MAX_SAMPLES) {
      buffer[index_buffer++] = (uint8_t)(tick_us & 0xFF);
      // almaceno solo LSB de tick_us para simplificar ejemplo
    }
    LPC_GPIOINT->IO2IntClr = GPIO_INT_PIN; // limpiar flag
  }
}

int main(void) {
  systick_init();
  gpio_int_init();

  while (1) {
    // El buffer se va llenando automáticamente en la ISR
    // Podrías procesarlo o enviar por UART más adelante
  }
}
