#include "LPC17xx.h"
#include "usbdesc.h"
#include "usbcore.h"
#include "cdc.h"

// Descriptores del dispositivo USB CDC
const uint8_t USB_DeviceDescriptor[] = {
    0x12,                       // bLength
    USB_DEVICE_DESCRIPTOR_TYPE, // bDescriptorType
    0x10, 0x01,                 // bcdUSB 1.1
    0x02,                       // bDeviceClass (CDC)
    0x00,                       // bDeviceSubClass
    0x00,                       // bDeviceProtocol
    0x40,                       // bMaxPacketSize0
    0xC0, 0x16,                 // idVendor  (0x16C0 = NXP ejemplo)
    0xDC, 0x05,                 // idProduct (0x05DC)
    0x00, 0x01,                 // bcdDevice
    0x01,                       // iManufacturer
    0x02,                       // iProduct
    0x03,                       // iSerialNumber
    0x01                        // bNumConfigurations
};

// Descriptor de configuración (CDC)
const uint8_t USB_ConfigDescriptor[] = {
    // Configuración
    0x09, USB_CONFIGURATION_DESCRIPTOR_TYPE,
    67, 0x00,     // Total length
    0x02,         // Interfaces
    0x01,         // Config #
    0x00,         // iConfig
    0xC0,         // Self powered
    0x32,         // 100 mA

    // Interface 0 (CDC Comm)
    0x09, USB_INTERFACE_DESCRIPTOR_TYPE,
    0x00, 0x00, 0x01,
    0x02, 0x02, 0x01, 0x00,

    // Header Func Descriptor
    0x05, 0x24, 0x00, 0x10, 0x01,

    // Call Management Func Descriptor
    0x05, 0x24, 0x01, 0x00, 0x01,

    // ACM Func Descriptor
    0x04, 0x24, 0x02, 0x02,

    // Union Func Descriptor
    0x05, 0x24, 0x06, 0x00, 0x01,

    // Endpoint (IN) para notificaciones
    0x07, USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x81, 0x03, 0x08, 0x00, 0xFF,

    // Interface 1 (Data)
    0x09, USB_INTERFACE_DESCRIPTOR_TYPE,
    0x01, 0x00, 0x02, 0x0A, 0x00, 0x00, 0x00,

    // Endpoint OUT
    0x07, USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x02, 0x02, 0x40, 0x00, 0x00,

    // Endpoint IN
    0x07, USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x82, 0x02, 0x40, 0x00, 0x00,
};

// Cadenas descriptivas
const uint8_t USB_StringDescriptor[] = {
    0x04, 0x03, 0x09, 0x04, // Idioma (Inglés)
    0x0E, 0x03, 'N', 0x00, 'X', 0x00, 'P', 0x00,
    0x10, 0x03, 'L', 0x00, 'P', 0x00, 'C', 0x00, ' ', 0x00, 'C', 0x00, 'D', 0x00, 'C', 0x00
};
