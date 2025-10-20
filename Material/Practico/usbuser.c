#include "usbuser.h"
#include "usbcore.h"
#include "cdc.h"

void USB_Reset_Event(void)
{
    CDC_Reset();
}

void USB_EndPoint1(uint32_t event) { CDC_EndPoint1(event); }
void USB_EndPoint2(uint32_t event) { CDC_EndPoint2(event); }
void USB_EndPoint3(uint32_t event) { CDC_EndPoint3(event); }

void USB_SOF_Event(void) { CDC_SOF_Event(); }

void USB_DevStatus_Event(uint32_t event)
{
    if (event & USB_EVENT_SUSPEND)
        ; // manejar suspend
    if (event & USB_EVENT_RESET)
        CDC_Reset();
}
