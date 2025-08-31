/* MODULO 1 - GPIO Y PINSEL
 *
 * 1. Identificacion de Registros PINSEL y PINMODE:
 * Si tenemos el puerto Px.y y debemos configurarlo
 * Los registros seran:
 * [Registro]	(2*x) -- (2*x)-1
 * [Pines]		y<=15 -- y>=16
 * [Bits]		2*y	  -- 2(y-16)
 * 				2*y+1 -- 2(y-16)+1
 *
 * Si tenemos el pin P1.8
 * Tendriamos que seleccionar entre el PINSEL2 O PINSEL3 (PINMODE2 O PINMODE3)
 * Como el pin es el 8, es menor a a 15 seleccionamos PINSEL2 (PINMODE2)
 * Los bit que tendremos que modificar son el 16 y 17 del registro PINSEL2 Y PINMODE2
 */
