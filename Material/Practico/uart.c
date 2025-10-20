#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include <stdio.h>
#include <string.h>

#define UART_PORT LPC_UART0

void UART_Init(void);
void UART_SendString(uint8_t *str);
void UART_IRQHandler(void);

volatile uint8_t rx_buffer[64];
volatile uint8_t rx_index = 0;

int main(void) {
    SystemInit();
    UART_Init();

    UART_SendString((uint8_t *)"UART0 at 115200bps\r\n");

    while (1) {
        // Echo simple: si hay datos recibidos, reenviarlos
        if (rx_index > 0) {
            UART_SendString((uint8_t *)"You sent: ");
            UART_SendString((uint8_t *)rx_buffer);
            UART_SendString((uint8_t *)"\r\n");
            rx_index = 0;
        }
    }
}

/*----------------------------------------------
 * UART 115200bps, 8N1
 *---------------------------------------------*/
void UART_Init(void) {
    PINSEL_CFG_Type PinCfg;

    // TXD0 y RXD0 -> P0.2 y P0.3
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 2;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 3;
    PINSEL_ConfigPin(&PinCfg);

    UART_CFG_Type UARTConfigStruct;
    UART_ConfigStructInit(&UARTConfigStruct);  // 115200 8N1
    UART_Init(UART_PORT, &UARTConfigStruct);

    UART_TxCmd(UART_PORT, ENABLE);
    UART_IntConfig(UART_PORT, UART_INTCFG_RBR, ENABLE);
    NVIC_EnableIRQ(UART0_IRQn);
}

/*----------------------------------------------
 * Send string (blockeante)
 *---------------------------------------------*/
void UART_SendString(uint8_t *str) {
    UART_Send(UART_PORT, str, strlen((char *)str), BLOCKING);
}

/*----------------------------------------------
 * IRQ handler — almacena received chars
 *---------------------------------------------*/
void UART_IRQHandler(void) {
    uint8_t c;
    if (UART_Receive(UART_PORT, &c, 1, NONE_BLOCKING)) {
        if (rx_index < sizeof(rx_buffer) - 1) {
            rx_buffer[rx_index++] = c;
            rx_buffer[rx_index] = '\0';
        }
    }
}
