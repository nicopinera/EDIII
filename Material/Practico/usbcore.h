#ifndef __USBCORE_H__
#define __USBCORE_H__

#include "lpc17xx.h"

#define USB_DEVICE_DESCRIPTOR_TYPE        0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE 0x02
#define USB_INTERFACE_DESCRIPTOR_TYPE     0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE      0x05

#define USB_EVENT_RESET   (1 << 0)
#define USB_EVENT_SUSPEND (1 << 1)

void USB_Init(void);
void USB_Connect(uint32_t con);
void USB_Poll(void);
uint32_t USB_WriteEP(uint32_t EPNum, uint8_t* pData, uint32_t cnt);

#endif
