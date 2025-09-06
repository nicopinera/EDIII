/* Contador Reversible con Interrupción Externa
 * Realice un contador hexadecimal automático (1.4), pero esta vez utilice una
 * interrupción externa para controlar el sentido de la cuenta.
 *
 * Pines: Utilice un display de 7 segmentos conectado a un puerto de su
 * elección. Un pulsador, conectado a un pin configurado como interrupción
 * externa (EINT0, EINT1 o EINT2), controlará la dirección.
 *
 * Lógica:
 * Inicialmente, el contador debe avanzar de forma automática (ascendente).
 * Cuando el botón se mantenga presionado, el contador debe detenerse. Al soltar
 * el botón, el sentido de la cuenta debe invertirse (descendente) a partir del
 * valor actual. Al volver a presionar el botón y soltarlo, la cuenta debe
 * volver a ser ascendente.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define

// Variables
volatile uint8_t contador = 0;
volatile uint8_t sentido = 0;
volatile uint32_t digits[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                                0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
volatile uint8_t pausado = 0;

// Funciones
void configGPIO();
void configInterrupcion();
void EINT3_IRQHandler();
void shortdelay() {
  for (int i = 0; i < 100000; i++)
    ;
}
void contadorAumDec() {
  if (sentido) { // 1 = descendente
    contador = (contador - 1) % 16;
  } else { // 0= ascendente
    contador = (contador + 1) % 16;
  }
}

int main(void) {
  SystemInit();
  configGPIO();
  configInterrupcion();

  LPC_GPIO2->FIOCLR = 0x7F; // LIMPIO TODO, EMPIEZAN APAGADOS

  while (1) {
    LPC_GPIO0->FIOPIN = digits[contador];
    contadorAumDec();
    shortdelay();
    shortdelay();
  }
  return 0;
}
void configGPIO() {
  // P2.10 COMO EINT0
  LPC_PINCON->PINSEL4 |= (1 << 20);  // 1 EN BIT 20
  LPC_PINCON->PINSEL4 &= ~(1 << 21); // 0 EN BIT 21
  LPC_GPIO2->FIODIR &= ~(1 << 10);   // P2.10 COMO ENTRADA

  LPC_PINCON->PINSEL0 &= ~(0X3FFF); // P0.0 A P0.6 COMO GPIO
  LPC_GPIO0->FIODIR |= (0X7F);      // TODOS COMO SALIDA
}
void configInterrupcion() {
  NVIC_DisableIRQ(EINT0_IRQn); // DESACTIVO INTERRUPCION
  // CONFIGURA EL MODO
  LPC_SC->EXTPOLAR |= (1 << 0);

  // LIMPIO LA BANDERA
  LPC_SC->EXTINT |= (1 << 0); // LIMPIO PRIMER BIT

  NVIC_EnableIRQ(EINT0_IRQn); // HABILITO LA INTERRUPCION
}
void EINT0_IRQHandler() {
  // Leer el estado actual del pin P2.10
  uint8_t estadoPin;
  if ((LPC_GPIO2->FIOPIN >> 10) & 0B1) {
    estadoPin = 1;
  } else {
    estadoPin = 0;
  }

  if (estado_pin == 0) {
    // Flanco de BAJADA - botón PRESIONADO
    pausado = 1;
  } else {
    // Flanco de SUBIDA - botón SOLTADO
    sentido ^= 1; // Alternar entre 0 y 1
    pausado = 0;
  }

  // LIMPIAR la bandera de interrupción
  LPC_SC->EXTINT |= (1 << 0);
}
