#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

/*	GPIO
 * Tenemos 5 puertos - de P0 a P4
 * No todos son de 32 bits o 32 pines
 *
 * tenemos registros de control de cada pin
 * muchos pines son compartidos, entonces debo configurar o controlar a quien se le asigna el pin
 *
 * tenemos hasta 4 funciones de cada pin
 * PINSEL selecciona el modo, lo asigno a GPIO o algun periferico
 * PINMODE -> si esta como entrada en GPIO puedo asignar resistencias pull up-down, repetidor (mantener ultimo estado logico)
 *
 *Los GPIO se establecen con los registros
 *FIODIR: Define la direccion 0-> entrada - 1-> salida
 *FIOPIN: Estado actual de los pines
 *FIOSET: Pongo en 1 los pines que quiero modificar su salida, si pongo un 0 no cambia
 *FIOCLR: Pongo un 1 en los pines que quiero limpiar, 0 no cambia nada
 * *
 *Configuracion del LED RGB de la placa
 *son catodo, se activan x 0
 *
 *
 * */

int main(void) {


    volatile static int i = 0 ;

    while(1) {

    }
    return 0 ;
}
