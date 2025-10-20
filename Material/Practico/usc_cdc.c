#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_usb.h"
#include "usbcore.h"
#include "usbdesc.h"
#include "usbuser.h"
#include "cdc.h"

#define UART_PORT   LPC_UART0

void UART_Init(void);
void USB_InitDevice(void);
void UART_SendString(uint8_t *str);

int main(void)
{
    SystemInit();

    UART_Init();
    USB_InitDevice();

    UART_SendString((uint8_t *)"USB CDC Device iniciado\r\n");

    while (1)
    {
        // Enviar datos recibidos por USB hacia UART
        if (CDC_RdOutBufAvailChar() > 0)
        {
            uint8_t c;
            CDC_RdOutBuf(&c, 1);
            UART_Send((LPC_UART_TypeDef *)UART_PORT, &c, 1, BLOCKING);
        }

        // Leer desde UART y enviar a USB
        if (UART_CheckBusy((LPC_UART_TypeDef *)UART_PORT) == RESET)
        {
            uint8_t c;
            if (UART_Receive((LPC_UART_TypeDef *)UART_PORT, &c, 1, NONE_BLOCKING))
            {
                CDC_WrInBuf(&c, 1);
            }
        }

        CDC_Run();  // Procesa tareas USB
    }
}

/* ---------------- UART INIT ---------------- */
void UART_Init(void)
{
    PINSEL_CFG_Type PinCfg;

    // TXD0 y RXD0
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;

    PinCfg.Pinnum = 2; // TXD0
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Pinnum = 3; // RXD0
    PINSEL_ConfigPin(&PinCfg);

    UART_CFG_Type UARTConfigStruct;
    UART_ConfigStructInit(&UARTConfigStruct);
    UARTConfigStruct.Baud_rate = 115200;

    UART_Init(UART_PORT, &UARTConfigStruct);
    UART_TxCmd(UART_PORT, ENABLE);
}

/* ---------------- USB INIT ---------------- */
void USB_InitDevice(void)
{
    USB_Init();
    USB_Connect(TRUE);
}

/* ---------------- UART send string ---------------- */
void UART_SendString(uint8_t *str)
{
    while (*str)
    {
        UART_SendByte((LPC_UART_TypeDef *)UART_PORT, *str++);
    }
}
