/*
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

// Define
#define DELAY_TIME 10000000

// Funciones
void configGPIO();
void shortDelay();

int main(void)
{

	SystemInit();
	configGPIO();

	while (1)
	{
	}
	return 0;
}

void configGPIO()
{
}
void shortDelay()
{
	for (uint32_t i = 0; i < DELAY_TIME; i++)
	{
	}
}
