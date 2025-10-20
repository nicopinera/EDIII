#ifndef __USBUSER_H__
#define __USBUSER_H__

#include "lpc17xx.h"

void USB_Reset_Event(void);
void USB_EndPoint1(uint32_t event);
void USB_EndPoint2(uint32_t event);
void USB_EndPoint3(uint32_t event);
void USB_SOF_Event(void);
void USB_DevStatus_Event(uint32_t event);

#endif
