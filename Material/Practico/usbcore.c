#include "usbcore.h"
#include "usbhw.h"
#include "usbuser.h"

static uint32_t USB_Connected = 0;
static uint32_t USB_Address = 0;

void USB_Init(void) {
    USB_HW_Init();
    USB_Connected = 1;
}

void USB_Connect(uint32_t con) {
    USB_Connected = con;
    USB_HW_Connect(con);
}

void USB_Poll(void) {
    // Aquí normalmente se procesa cualquier evento de USB, endpoints, SOF, etc.
    // Para ejemplo mínimo no hacemos nada, solo se podrían llamar callbacks
}

uint32_t USB_WriteEP(uint32_t EPNum, uint8_t* pData, uint32_t cnt) {
    // Escribir datos en endpoint
    // Mínimo ejemplo: llamar a la función de hardware
    (void)pData;
    (void)cnt;
    return 0; // indica que se “escribió”
}
