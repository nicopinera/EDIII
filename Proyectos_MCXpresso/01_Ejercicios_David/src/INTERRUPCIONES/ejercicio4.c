/*
Múltiples Interrupciones
Cree un programa que use dos fuentes de interrupción para controlar secuencias
de LEDs diferentes.

Configuración:
Configure el pin P0.0 para generar una interrupción por flanco de subida.
Configure una interrupción externa (EINT1) en el pin P2.11 por flanco de bajada.

Lógica:
Cuando se detecta la interrupción del pin P0.0, se debe ejecutar una secuencia
de 4 LEDs o 4 números si está utilizando un display de 7 segmentos Cuando se
detecta la interrupción externa del pin P2.11, se debe ejecutar una secuencia de
parpadeo diferente.

Condiciones: Configure la interrupción externa (EINT1) para que tenga una mayor
prioridad que la interrupción del GPIO del Puerto 0.
*/

#define TIME 1000000

volatile int secuencia = 0;

void configurarGPIO(void) {
  // P0.0 GPIO DE ENTRADA - PULL DOWN
  LPC_PINCON->PINSEL0 &= ~(0b11); // GPIO
  LPC_GPIO0->FIODIR &= ~(1 << 0); // ENTRADA - 0
  LPC_PINCON->PINMODE0 |= (0b11); // PULL DOWN

  // P2.11 EINT1 - PULL UP
  LPC_PINCON->PINSEL4 |= (1 << 22);      // EINT1
  LPC_PINCON->PINMODE4 &= ~(0b11 << 22); // PULL UP
  LPC_GPIO2->FIODIR &= ~(1 << 11);       // ENTRADA - 0

  // P1.16 A P1.19 PARA LOS LEDS - GPIO - SALIDA
  LPC_PINCON->PINSEL3 &= ~(0XFF);   // GPIO
  LPC_GPIO1->FIODIR |= (0xF << 16); // SALIDA
  LPC_GPIO1->FIOCLR |= (0xF << 16); // APAGO TODO
}
void configurarINT(void) {
  // P0.0 POR FLANCO SUBIDA
  LPC_GPIOINT->IO0IntEnR |= (1 << 0);
  LPC_GPIOINT->IO0IntClr |= (1 << 0);

  // EINT1 POR FLANCO DE BAJADA
  LPC_SC->EXTMODE |= (1 << 1);   // FLANCOS
  LPC_SC->EXTPOLAR &= ~(1 << 1); // FLANCO DE BAJADA
  LPC_SC->EXTINT |= (1 << 1);    // LIMPIAR BANDERA

  // Settear prioridad
  NVIC_SetPriority(EINT1_IRQn, 3);
  NVIC_SetPriority(EINT3_IRQn, 6);

  // Habilitar interrupciones
  NVIC_EnableIRQ(EINT1_IRQn); // EINT1
  NVIC_EnableIRQ(EINT3_IRQn); // GPIO
}

void EINT1_IRQHandler(void) {
  secuencia = 1;
  LPC_SC->EXTINT |= (1 << 1); // LIMPIAR BANDERA
}

void EINT3_IRQHandler(void) {
  secuencia = 0;
  LPC_GPIOINT->IO0IntClr |= (1 << 0); // limpiar bandera
}

void shortDelay(void) {
  for (int i = 0; i < TIME; i++)
    ;
}

int main(void) {
  SystemInit();
  configurarGPIO();
  while (1) {
    switch (secuencia) {
    case 0:
      // secuencia 1 - Prenden todos
      LPC_GPIO1->FIOSET = (0xF << 16); // PRENDO TODOS
      shortDelay();
      LPC_GPIO1->FIOCLR = (0xF << 16); // APAGO TODOS
      shortDelay();
      break;
    case 1:
      // secuencia 2
      // se prenden intercalados
      LPC_GPIO1->FIOCLR = (0b1111 << 16); // APAGO TODOS
      LPC_GPIO1->FIOSET = (0b1010 << 16);
      shortDelay();
      LPC_GPIO1->FIOCLR = (0b1111 << 16); // APAGO TODOS
      LPC_GPIO1->FIOSET = (0b0101 << 16);
      shortDelay();
      break;
    default:
      break;
    }
  }
  return 0;
}
