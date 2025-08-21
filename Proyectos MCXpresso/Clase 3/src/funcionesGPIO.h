/*
 * En el presente header se estableceran las funciones para los puertos GPIO
 * Crear funciones de configuración. Paso el puerto, paso el pin, y entrada-salida.
 * Crear funciones de seteo.
 * Crear funciones de status.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifndef FUNCIONESGPIO_H_
#define FUNCIONESGPIO_H_

#include <stdio.h>

void configurarGPIO(uint8_t puerto, uint8_t pin, uint8_t IO);
void setteoGPIO(uint8_t puerto, uint8_t pin);
void statusGPIO(uint8_t puerto, uint8_t pin);




#endif /* FUNCIONESGPIO_H_ */
