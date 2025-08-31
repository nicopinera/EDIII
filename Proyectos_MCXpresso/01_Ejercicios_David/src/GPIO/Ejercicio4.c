/*4. Utilice 7 pines GPIO para controlar un display de 7 segmentos.
 *El programa debe mostrar de manera cíclica y automática los
 *16 dígitos hexadecimales (0-F) de forma ascendente.
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

int main(void)
{

    SystemInit();
    configGPIO();

    uint32_t segmento = 0;
    const uint32_t digits[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,
                                 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    while (1)
    {
        // Ejercicio 4
        LPC_GPIO2->FIOCLR = 0x7F;                  // APAGO TODOS
        LPC_GPIO2->FIOSET = digits[segmento % 16]; // cargo todo el valor, los bit que no toco se ponen en 0
        segmento++;
        shortDelay();
    }
    return 0;
}

void configGPIO()
{
    // Configuracion de los 7 pines del puerto 2
    LPC_PINCON->PINSEL4 &= ~(0x3FFF); // PRIMEROS 14 BITS EN 0 PARA GPIO
    LPC_GPIO2->FIODIR |= (0x7F);      // LOS PRIMEROS 7 COMO SALIDA
}
void shortDelay()
{
    for (uint32_t i = 0; i < DELAY_TIME; i++)
    {
    }
}
