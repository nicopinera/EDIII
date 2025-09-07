/*
Secuencia con Interrupción por Botón:
Escriba un programa que ejecute una secuencia de 8 LEDs de forma automática.
Cuando se presione un botón conectado a una entrada GPIO, la secuencia debe
pausarse. Al volver a presionar el botón, la secuencia debe reanudarse.

Condiciones:
Utilice una interrupción por flanco de bajada en el pin del pulsador.
*/
#define TIME 1000000
volatile int detenido = 0;

void configGPIO(void) {
  // P0.0 GPIO - ENTRADA - PULL UP
  LPC_PINCON->PINSEL0 &= ~(0b11);  // GPIO
  LPC_PINCON->PINMODE0 &= ~(0b11); // PULL UP
  LPC_GPIO0->FIODIR &= ~(1 << 0);  // ENTRADA - 0

  // P1.16 A P1.23 GPIO - SALIDA
  LPC_PINCON->PINSEL3 &= ~(0XFFFF);  // GPIO
  LPC_GPIO1->FIODIR |= (0XFF << 16); // SALIDA
}
void configINT(void) {
  // HABILITAR EINT3
  // GPIOINT POR FLANCO - FLANCO DE BAJADA
  LPC_GPIOINT->IO0IntEnF |= (1 << 0); // flanco de bajada
  LPC_GPIOINT->IO0IntClr |= (1 << 0); // limpio bandera
  NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void) {
  detenido ^= 1; // se pasa de 0 a 1 y de 1 a 0 cada vez que interrumpe
  LPC_GPIOINT->IO0IntClr |= (1 << 0); // limpio bandera
}
void delay() {
  for (int i = 0; i < TIME; i++)
    ;
}
int main(void) {
  configGPIO();
  configINT();
  int i = 0;
  while (1) {
    if (!detenido) {
      // Secuencia
      LPC_GPIO1->FIOCLR = (0XFF << 16);    // Apago todos
      LPC_GPIO1->FIOSET = (1 << (16 + i)); // y luego prendo el 16
      i = (i + 1) % 8;                     // aumento para prender el que sigue
      delay();
    }
  }
  return 0;
}
