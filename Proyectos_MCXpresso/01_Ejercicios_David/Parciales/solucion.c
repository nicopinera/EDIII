/*
 * Ejercicio: Captura de Estados GPIO en AHB SRAM Bank 0
 *
 * Descripción:
 * - Lee el estado de P0[7:0] cada 100ms usando SysTick
 * - Almacena las lecturas en AHB SRAM Bank 0 (0x2007C000) 13 of 851
 * - P2.0 = LED indicador de buffer lleno
 * - P2.1 = Botón para reiniciar captura
 *
 * Memoria AHB SRAM Bank 0 :
 * - Dirección base: 0x2007C000
 * - Tamaño: 16 KB
 * - Uso: Buffer circular de 256 muestras
 */

#include "LPC17xx.h"

// Configuración de memoria AHB SRAM Bank 0
#define AHB_SRAM_BASE 0x2007C000 // Dirección base AHB SRAM Bank 0
#define BUFFER_SIZE 256          // Número de muestras a almacenar

// Configuración de pines
#define LED_FULL_PIN 0       // P2.0 - LED buffer lleno
#define RESET_BUTTON_PIN 1   // P2.1 - Botón reset
#define INPUT_PORT_MASK 0xFF // P0[7:0] - Puerto de entrada

// Configuración de timing
#define SAMPLE_PERIOD_MS 100 // Período de muestreo

// Punteros para acceso a AHB SRAM
volatile uint8_t *const ahb_buffer = (uint8_t *)AHB_SRAM_BASE; // primeros 256 bytes

volatile uint8_t *const ahb_index = (uint8_t *)(AHB_SRAM_BASE + BUFFER_SIZE); // un solo valor que voy a actualizar

volatile uint8_t *const ahb_sample_count = (uint8_t *)(AHB_SRAM_BASE + BUFFER_SIZE + 1); // 258
volatile uint32_t *const ahb_timestamp = (uint32_t *)(AHB_SRAM_BASE + BUFFER_SIZE + 1);  // 259

// Variables globales en RAM normal
volatile uint32_t milliseconds = 0;
volatile uint32_t last_sample_time = 0;
volatile uint8_t buffer_full = 0; // si esta lleno el buffer o no

// Prototipos
void config_GPIO(void);
void config_SysTick(void);
void capture_sample(void);
void reset_buffer(void);
void check_reset_button(void);

int main(void)
{
    // Inicializar sistema
    SystemInit(); // 100 MHz, PLL, etc

    // Configurar periféricos
    config_GPIO();
    config_SysTick();

    // Inicializar buffer en AHB SRAM
    reset_buffer();

    // Bucle principal
    while (1)
    {
        // Verificar botón de reset
        check_reset_button();

        // Capturar muestra cada SAMPLE_PERIOD_MS
        if ((milliseconds - last_sample_time) >= SAMPLE_PERIOD_MS)
        {
            last_sample_time = milliseconds;

            if (!buffer_full)
            {
                capture_sample();
            }
        }
    }

    return 0;
}

/**
 * Handler de interrupción SysTick - cada 1ms
 */
void SysTick_Handler(void)
{
    milliseconds++; // no tardar tanto tiempo dentro del handler
}

/**
 * Captura una muestra del puerto P0[7:0] y la almacena en AHB SRAM
 */
void capture_sample(void)
{
    // Leer estado actual del puerto P0[7:0]
    uint8_t port_value = LPC_GPIO0->FIOPIN & INPUT_PORT_MASK;

    // Obtener índice actual desde AHB SRAM
    uint8_t current_index = *ahb_index;

    // Almacenar muestra en AHB SRAM
    ahb_buffer[current_index] = port_value;

    // Almacenar timestamp de la muestra
    ahb_timestamp[current_index] = milliseconds;

    // Incrementar índice (buffer circular)
    current_index = (current_index + 1) % BUFFER_SIZE; //
    *ahb_index = current_index;

    // Incrementar contador de muestras
    (*ahb_sample_count)++;

    // Verificar si el buffer está lleno
    if (*ahb_sample_count >= BUFFER_SIZE)
    {
        buffer_full = 1;
        // Encender LED indicador
        LPC_GPIO2->FIOSET = (1 << LED_FULL_PIN);
    }
}

/**
 * Reinicia el buffer y variables de control en AHB SRAM
 */
void reset_buffer(void)
{
    // Limpiar buffer de datos
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        ahb_buffer[i] = 0;
        ahb_timestamp[i] = 0;
    }

    // Reiniciar variables de control en AHB SRAM
    *ahb_index = 0;
    *ahb_sample_count = 0;

    // Reiniciar flags
    buffer_full = 0;

    // Apagar LED indicador
    LPC_GPIO2->FIOCLR = (1 << LED_FULL_PIN);
}

/**
 * Verifica si se presionó el botón de reset
 */
void check_reset_button(void)
{
    static uint8_t last_button_state = 1;
    uint8_t current_button_state = (LPC_GPIO2->FIOPIN >> RESET_BUTTON_PIN) & 1;

    // Detectar flanco descendente (botón presionado)
    if (last_button_state == 1 && current_button_state == 0)
    {
        reset_buffer();
    }

    last_button_state = current_button_state;
}

/**
 * Configuración de GPIO
 */
void config_GPIO(void)
{
    // === Puerto 0: Entrada de datos ===
    // P0[7:0] como GPIO entrada
    LPC_PINCON->PINSEL0 &= ~0x0000FFFF;    // Bits [15:0] = 0 para GPIO
    LPC_GPIO0->FIODIR &= ~INPUT_PORT_MASK; // P0[7:0] como entradas

    // Habilitar pull-up en P0[7:0]
    LPC_PINCON->PINMODE0 &= ~0x0000FFFF; // Pull-up habilitado

    // === Puerto 2: Control ===
    // P2.0 (LED) y P2.1 (Botón) como GPIO
    LPC_PINCON->PINSEL4 &= ~((3 << 0) | (3 << 2));

    // P2.0 como salida (LED)
    LPC_GPIO2->FIODIR |= (1 << LED_FULL_PIN);

    // P2.1 como entrada (Botón)
    LPC_GPIO2->FIODIR &= ~(1 << RESET_BUTTON_PIN);

    // Pull-up en P2.1 (botón)
    LPC_PINCON->PINMODE4 &= ~(3 << 2); // Pull-up habilitado

    // Inicializar LED apagado
    LPC_GPIO2->FIOCLR = (1 << LED_FULL_PIN);
}

/**
 * Configuración del SysTick para 1ms
 */
void config_SysTick(void)
{
    SysTick_Config(SystemCoreClock / 1000);
}