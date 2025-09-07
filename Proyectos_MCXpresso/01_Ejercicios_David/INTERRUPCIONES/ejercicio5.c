/*
Secuencia con Interrupción por Botón:
Escriba un programa que ejecute una secuencia de 8 LEDs de forma automática.
Cuando se presione un botón conectado a una entrada GPIO, la secuencia debe
pausarse. Al volver a presionar el botón, la secuencia debe reanudarse.

Condiciones:
Utilice una interrupción por flanco de bajada en el pin del pulsador.
*/
#define TIME 1000000
volatile detenido = 0;

void configGPIO(void) {
  // P0.0 GPIO - ENTRADA -PULL UP
  LPC_PINCON->PINSEL0 &= ~(0b11);  // GPIO
  LPC_PINCON->PINMODE0 &= ~(0b11); // PULL UP
  LPC_GPIO0->FIODIR &= ~(1 << 0);  // ENTRADA - 0

  // P1.16 A P1.23 GPIO - SALIDA
}
void configINT(void) {
  // HABILITAR EINT3
  // GPIOINT POR FLANCO - FLANCO DE BAJADA
}

void EINT3_IRQHandler(void) {}
void delay() {
  for (int i = 0; i < TIME; i++) {
    for (int j = 0; j < TIME; j++)
      ;
  }
}
int main(void) {
  while (1) {
    if (!detenido) {
      // Secuencia

      delay();

      delay();
    }
  }

  return 0;
}
