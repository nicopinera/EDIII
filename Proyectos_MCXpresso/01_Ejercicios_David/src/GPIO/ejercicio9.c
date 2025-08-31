/* Utilice dos grupos de 4 pines de entrada (A_in, B_in) y un interruptor para seleccionar una operación.
 * El interruptor (conectado a un pin) debe decidir si los valores de A_in y B_in se suman o se restan.
 * El valor absoluto del resultado debe mostrarse en 4 LEDs de salida.
 * Utilice un LED adicional para mostrar si se genera un overflow en la suma o si el resultado de la resta es negativo
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

int main(void) {
	SystemInit();
	configGPIO();
	uint8_t A_in, B_in, interruptor; // Enteros sin signo
	int8_t resultado; // Entero con signo
	uint8_t valor_absoluto;
	uint8_t carry = 0;
	uint8_t negativo = 0;

	while (1) {
		A_in = LPC_GPIO0->FIOPIN & 0xF;
		B_in = (LPC_GPIO0->FIOPIN >> 4) & 0xF;
		interruptor = LPC_GPIO1->FIOPIN & 0b1;

		// Limpiar banderas anteriores
		LPC_GPIO1->FIOCLR = (0x3F << 1); // Limpiar bits 1-6 (LEDs y banderas)

		switch (interruptor) {
		case 0: // Suma
			resultado = A_in + B_in; // se guarda con signo - ultimo bit es el del signo
			carry = (resultado > 15);    // Overflow en suma
			valor_absoluto = resultado & 0xF; // Tomar solo 4 bits
			break;

		case 1: // Resta
			resultado = A_in - B_in;
			negativo = (resultado < 0);
			if (resultado < 0) {
				valor_absoluto = -resultado;
			} else {
				valor_absoluto = resultado;
			}
			break;
		}

		// Mostrar valor absoluto en LEDs (bits 1-4)
		LPC_GPIO1->FIOSET = (valor_absoluto << 1);

		// Activar banderas
		if (carry){
			LPC_GPIO1->FIOSET = (1 << 5);
		}
		if (negativo){
			LPC_GPIO1->FIOSET = (1 << 6);
		}
	}
	return 0;
}

void configGPIO() {
	// Configuracion P0.0 a P0.7 como salida
	//p0.0 a p0.3 A_in
	//p0.4 a p0.7 B_in
	//p1.0 interruptor (0=suma - 1=resta)
	//P1.1 a P1.4 -> salida
	//p1.5 -> carry
	//p1.6 -> negativo
	LPC_PINCON->PINSEL0 &= ~(0xFFFF); // CONFIGURO GPIO
	LPC_PINCON->PINSEL2 &= ~(0x3FFF); // CONFIGURO GPIO
	LPC_GPIO0->FIODIR &= ~(0xFF); // P0.0 A P0.7 COMO ENTRADA
	LPC_GPIO1->FIODIR &= ~(0b1); //P1.0 ENTRADA
	LPC_GPIO1->FIODIR |= (0b111111 << 1); //P1.1 A P1.6 CON SALIDA

}
void shortDelay() {
	for (uint32_t i = 0; i < DELAY_TIME; i++) {
	}
}
