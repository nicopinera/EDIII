/*
Utilizando interrupciones por GPIO realizar un código en C que permita,
mediante 4 pines de entrada GPIO, leer y guardar un número compuesto por 4 bits.
P0.0 A P0.3

Dicho número puede ser cambiado por un usuario mediante 4 switches, los cuales
cuentan con sus respectivas resistencias de pull up externas.

El almacenamiento debe realizarse en una variable del tipo array de forma tal
que se asegure tener disponible siempre los últimos 10 números elegidos
por el usuario, garantizando además que el número ingresado más antiguo, de este
conjunto de 10, se encuentre en el elemento 9 y el número actual en el elemento
0 de dicho array.

La interrupción por GPIO empezará teniendo la máxima prioridad de interrupción
posible y cada 200 números ingresados deberá disminuir en 1 su prioridad hasta
alcanzar la mínima posible.

Llegado este momento, el programa deshabilitará todo tipo de interrupciones
producidas por las entradas GPIO. Tener en cuenta que el código debe estar
debidamente comentado.
*/

#include "LPC17xx.h"

#define MAX 10
#define SEG 15999999 // 1 segundo con cclk = 16 MHz

// variables
volatile int numeros_ingresados = 0; // cantidad total de muestras
volatile int prioridad = 0;          // prioridad actual de la interrupción
volatile int ingresando_numero =
    0; // flag para indicar que SysTick está habilitado

// buffer en RAM (dirección arbitraria segura)
volatile uint8_t buffer[MAX];

// valor actual leído de P0.0-P0.3
volatile uint8_t valor_actual = 0;

// === FUNCIONES AUXILIARES ===
void insertarBuffer(uint8_t valor) {
  // desplazo todos los valores una posición hacia atrás
  for (int i = MAX - 1; i > 0; i--) {
    buffer[i] = buffer[i - 1];
  }
  buffer[0] = valor; // inserto el nuevo en posición 0
}

// === CONFIGURACIÓN ===
void configurarGPIO(void) {
  // P0.0 a P0.3 como entradas GPIO
  LPC_PINCON->PINSEL0 &= ~(0xFF); // función GPIO
  LPC_GPIO0->FIODIR &= ~(0xF);    // entradas
}

void configurarInterrupciones(void) {
  // habilito interrupción por flanco en P0.0-P0.3 (GPIOINT EINT3)
  LPC_GPIOINT->IO0IntEnR |= 0xF; // flanco ascendente
  LPC_GPIOINT->IO0IntEnF |= 0xF; // flanco descendente
  LPC_GPIOINT->IO0IntClr = 0xF;  // limpio banderas

  NVIC_SetPriority(EINT3_IRQn, prioridad); // máxima prioridad
  NVIC_EnableIRQ(EINT3_IRQn);
}

void configurarSysTick(void) {
  SysTick->CTRL = 0; // deshabilitado inicialmente
  SysTick->VAL = 0;
  SysTick->LOAD = SEG; // cuenta de 1 segundo
  SysTick->CTRL =
      SysTick_CTRL_TICKINT_Msk; // interrupción habilitada, pero no el conteo
}

// === HANDLERS ===
void EINT3_IRQHandler(void) {
  // al detectar cambio en las entradas -> habilito SysTick
  ingresando_numero = 1;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // arranco el conteo

  LPC_GPIOINT->IO0IntClr = 0xF; // limpio banderas
}

void SysTick_Handler(void) {
  if (ingresando_numero) {
    // tomo muestra de P0.0-P0.3
    valor_actual = (uint8_t)(LPC_GPIO0->FIOPIN & 0xF);

    // guardo en buffer
    insertarBuffer(valor_actual);

    // incremento contador de muestras
    numeros_ingresados++;

    // deshabilito SysTick hasta la próxima detección
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    ingresando_numero = 0;
  }
}

int main(void) {
  configurarGPIO();
  configurarInterrupciones();
  configurarSysTick();

  while (1) {
    // cada 200 números ingresados -> bajar prioridad
    if (numeros_ingresados != 0 && (numeros_ingresados % 200 == 0)) {
      prioridad++;
      if (prioridad < (1 << __NVIC_PRIO_BITS)) {
        NVIC_SetPriority(EINT3_IRQn, prioridad);
      } else {
        // prioridad mínima alcanzada -> deshabilito interrupción
        NVIC_DisableIRQ(EINT3_IRQn);
      }
    }
  }

  return 0;
}
