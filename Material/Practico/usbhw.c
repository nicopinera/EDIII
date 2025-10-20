#include "usbhw.h"
#include "LPC17xx.h"

void USB_HW_Init(void) {
    // Habilitar reloj USB
    LPC_SC->PCONP |= (1 << 31); // USB PCLK
    
    // Configurar PLL USB a 48MHz (simplificado)
    LPC_USB->USBClkCtrl = 0x01; 
    while (!(LPC_USB->USBClkSt & 0x01));
    
    // Enable USB interrupts en NVIC
    NVIC_EnableIRQ(USB_IRQn);
}

void USB_HW_SetAddress(uint32_t addr) {
    (void)addr;
    // en un ejemplo mínimo no hacemos nada
}

void USB_HW_Connect(uint32_t con) {
    (void)con;
    // en un ejemplo mínimo no hacemos nada
}

// ISR del USB
void USB_IRQHandler(void) {
    // Llamar a los callbacks definidos en usbuser.c según evento
    // Ejemplo mínimo
}
