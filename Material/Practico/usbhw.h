#ifndef __USBHW_H__
#define __USBHW_H__

#include "lpc17xx.h"

void USB_HW_Init(void);
void USB_HW_SetAddress(uint32_t addr);
void USB_HW_Connect(uint32_t con);

#endif
