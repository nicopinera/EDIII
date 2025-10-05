/* Similar al ejercicio 4, pero el contador solo debe avanzar un dígito cuando
 * se presiona un botón. Consideración: Implemente un método de debouncing
 * (antirrebote) por software para evitar múltiples conteos por una sola
 * pulsación. Pulsador en P0.0
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define
#define DELAY_TIME 10000000

// Funciones
void configGPIO();
void longDelay();
void shortDelay();
void secuenciaA();
void secuenciaB();

int main(void) {

  SystemInit();
  configGPIO();

  uint32_t segmento = 0;
  const uint32_t digits[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                               0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  uint8_t boton = 0;
  while (1) {
    boton = LPC_GPIO0->FIOPIN & 1; // Tomo el primer bit
    if (boton == 0) {
      shortDelay();
      boton = LPC_GPIO0->FIOPIN & 1; // Vuelvo a leer
      if (boton == 0) {
        LPC_GPIO2->FIOCLR = 0x7F; // APAGO TODOS
        LPC_GPIO2->FIOSET =
            digits[segmento % 16]; // cargo todo el valor, los bit que no toco
                                   // se ponen en 0
        segmento++;
      }
    }

    shortDelay();
  }
  return 0;
}

void configGPIO() {
  // Configuracion de los 7 pines del puerto 2
  LPC_PINCON->PINSEL4 &= ~(0x3FFF); // PRIMEROS 14 BITS EN 0 PARA GPIO
  LPC_GPIO2->FIODIR |= (0x7F);      // LOS PRIMEROS 7 COMO SALIDA
  LPC_PINCON->PINSEL0 &= ~(0b11);   // P0.0 como GPIO
  LPC_GPIO0->FIODIR &= ~(0b1);      // Entrada siempre en 1
}
void shortDelay() {
  for (uint32_t i = 0; i < DELAY_TIME; i++) {
  }
}
