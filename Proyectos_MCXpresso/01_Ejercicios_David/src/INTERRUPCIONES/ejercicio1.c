/* Escriba un programa muy sencillo que utilice un pin del Puerto 2 como interrupción externa (EINT0).
 * Cada vez que se active la interrupción (por flanco de bajada), un LED conectado a otro pin debe
 * cambiar de estado (encenderse si estaba apagado, y apagarse si estaba encendido).
 *
 * Pines: Utiliza P2.10 para el botón (EINT0) y P0.22 para el LED.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define
#define DELAY_TIME 10000000

volatile uint8_t flag = 0; // bandera

// Funciones
void configGPIO();
void configInterrupcion();
void shortDelay()
{
	for (int i = 0; i < DELAY_TIME; i++)
		;
}
void EINT0_IRQHandler();

int main(void)
{
	SystemInit();
	configGPIO();
	configInterrupcion();

	while (1)
	{
		if (flag)
		{
			LPC_GPIO0->FIOCLR = (1 << 22); // Prendido
		}
		else
		{
			LPC_GPIO0->FIOSET = (1 << 22); // Apagado
		}
	}
	return 0;
}

void configGPIO()
{
	LPC_PINCON->PINSEL4 |= (1 << 20);  // 1 EN BIT 20
	LPC_PINCON->PINSEL4 &= ~(1 << 21); // 0 EN BIT 21
	// P2.10 COMO EINT0
	LPC_PINCON->PINSEL1 &= ~(0b11 << 12); // P0.22 COMO GPIO

	LPC_GPIO2->FIODIR &= ~(1 << 10); // P2.10 COMO ENTRADA
	LPC_GPIO0->FIODIR |= (1 << 22);	 // P0.22 COMO SALIDA
}
void configInterrupcion()
{
	// CONFIGURA EL MODO
	LPC_SC->EXTMODE |= (1 << 0);   // BIT 0 EN 1 PARA FLANCOS
	LPC_SC->EXTPOLAR &= ~(1 << 0); // FLANCO DE BAJADA

	// LIMPIO LA BANDERA
	LPC_SC->EXTINT |= (1 << 0); // LIMPIO PRIMER BIT

	NVIC_EnableIRQ(EINT0_IRQn); // HABILITO LA INTERRUPCION
}
void EINT0_IRQHandler()
{
	flag ^= 1; // para alternar de 1 a 0 y de 0 a 1

	// LIMPIO LA BANDERA
	LPC_SC->EXTINT |= (1 << 0); // LIMPIO PRIMER BIT
}
