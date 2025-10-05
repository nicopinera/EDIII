
# Apuntes de Clases Prácticas

## 1. Introducción a la Programación en C

La programación consiste en una serie de pasos que el programador debe seguir:

1. **Diseño del algoritmo**
2. **Codificación**
3. **Compilación**
4. **Ejecución**

---

## 2. Configuración de Pines y GPIO (Clase 1)

### 2.1. Configuración de Pines

Para seleccionar la función y el modo de los pines:

```cpp
// Seleccionar función del pin
LPC_PINCON->PINSEL;
// Seleccionar funcionamiento de resistencias
LPC_PINCON->PINMODE;
```

### 2.2. Uso de GPIO

Para trabajar con el puerto GPIO x:

```cpp
// Setear dirección (entrada/salida)
LPC_GPIOx->FIODIR;
// Poner en 1 los pines seleccionados
LPC_GPIOx->FIOSET;
// Poner en 0 los pines seleccionados
LPC_GPIOx->FIOCLR;
// Leer el valor actual de los pines
LPC_GPIOx->FIOPIN;
```

---

## 3. Excepciones e Interrupciones (Clase 2)

Una **excepción** es una condición que cambia el flujo normal de control de un programa, generalmente producida por hardware dentro del core.

Los periféricos pueden generar interrupciones (ej: ADC, GPIO, etc). Se caracterizan por:

- Número de excepción
- IRQ number o Exception number
- Prioridad configurable
- Síncronas o asíncronas

El bloque de hardware que controla las interrupciones es el **NVIC**, mejorando la latencia de atención.

### 3.1. Registros del NVIC

```cpp
// Habilita interrupciones
NVIC->ISER[0-1];
// Limpia interrupciones
NVIC->ICER[0-1];
// Indica interrupciones pendientes
NVIC->ISPR[0-1];
// Limpia interrupciones pendientes
NVIC->ICPR[0-1];
// Permite setear la prioridad
NVIC->IPR[0-8];
```

**Secuencia típica:**
1. Deshabilitar interrupciones
2. Habilitar la interrupción del periférico
3. Limpiar la bandera de interrupción
4. Establecer prioridad
5. Volver a habilitar

**Funciones CMSIS útiles:**

```cpp
void NVIC_EnableIRQ(IRQn_Type IRQn);      // Habilitar interrupción
void NVIC_DisableIRQ(IRQn_Type IRQn);     // Deshabilitar interrupción
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority); // Setear prioridad
```

#### 3.2. Pasos para Configurar Interrupciones

1. Identificar la fuente de interrupción
2. Definir la prioridad
3. Habilitar la interrupción requerida
4. Definir la función handler para la interrupción

### 3.3. Interrupciones por GPIO

Los GPIO pueden interrumpir por flancos. El handler que atiende estas interrupciones es el de **EINT3**. Se deben habilitar las interrupciones externas 3 y luego configurar las de GPIO.

Se pueden habilitar interrupciones por ambos flancos (ascendente o descendente):

```cpp
// Habilita flanco ascendente
LPC_GPIOINT->IntEnR;
// Habilita flanco descendente
LPC_GPIOINT->IntEnF;
// Indica quién interrumpió por flanco ascendente
LPC_GPIOINT->IntStatR;
// Indica quién interrumpió por flanco descendente
LPC_GPIOINT->IntStatF;
```

> **Ejercicio:** Hay 4 llaves que forman un número binario. Hay que sacar ese número por un pin, sin que se corte cuando entra un número nuevo, almacenando los datos en un buffer circular.

---

## 4. SysTick

El **SysTick** es un contador descendente propio del core, preconfigurado para interrumpir cada $10\,ms$. Se puede precargar con un valor para ajustar el periodo de interrupción. Al ser propio del core, genera excepciones, no interrupciones.

```cpp
// Control del contador, habilita interrupción, selecciona fuente de clock y bandera de interrupción
LPC_SysTick->STCTRL;
// Valor de recarga
LPC_SysTick->STRELOAD;
// Valor actual del contador
LPC_SysTick->STCURR;
// Registro de calibración (para 10ms si el clock es 100MHz)
LPC_SysTick->STCALIB;
```

**Ejemplo de inicialización:**

```cpp
SystemInit(); // Configura el CClock en 100MHz
// SysTick->CTRL: Prendido/apagado, interrupción y fuente de clock
// Para calcular el valor de carga:
// Tfinal = (1/Fcore) * (Vload+1)  =>  Vload = (Tfinal)/(1/Fcore) - 1
// SysTick->LOAD = Vload;
// Leer valor del reloj:
// SysTick->VAL;
// Si quiero que interrumpa a 20Hz: T = 1/20 = 50ms
```

---

## 5. Timers

Cada timer tiene un contador (TC) y un preescaler (PR), ambos de 32 bits. La LPC tiene 4 timers. Los timers pueden interrumpir por **match** o por **capture**.

**Fórmula del periodo de interrupción:**

$$T_{interrupcion} = ((V_{valorTimerMax}-V_{preCargado})*PR+1)*T_{clock}= ((2^{32}-V_{precargado})*PR+1)*T_{clock}$$

**Registros principales:**

| Registro | Descripción |
|----------|-------------|
| PR       | Preescaler: almacena el valor máximo del contador de preescala |
| PC       | Preescaler Counter: se incrementa con cada PCLK, controla la resolución |
| TC       | Timer Counter: contador principal |
| TCR      | Timer Control Register: habilita/deshabilita/reinicia el timer |
| CTCR     | Count Control Register: selecciona modo timer/contador |
| MCR      | Match Control Register: configuración de los match |
| IR       | Interrupt Register: banderas de interrupción |
| EMR      | External Match Register: control de salidas externas |

**Secuencia típica de configuración:**
1. Establecer `LPC_TIMx->CTCR`
2. Definir `LPC_TIMx->PR`
3. Cargar valores en los registros de match si es necesario
4. Configurar `LPC_TIMx->MCR` si se usan match/interrupciones
5. Reiniciar PR y TC
6. Habilitar el timer (`LPC_TIMx->TCR = 0x01`)
7. Deshabilitar el timer (`LPC_TIMx->TCR = 0x00`)

**Configuración del reloj periférico:**

| Timer | Registro | Bits      |
|-------|----------|-----------|
| 0     | PCLKSEL0 | [3:2]     |
| 1     | PCLKSEL0 | [5:4]     |
| 2     | PCLKSEL1 | [13:12]   |
| 3     | PCLKSEL1 | [15:14]   |

Valores posibles:

- [00] – PCLK = CCLK/4 (por defecto)
- [01] – PCLK = CCLK
- [10] – PCLK = CCLK/2
- [11] – PCLK = CCLK/8

**Notas:**
- Los timers 0/1/3 solo tienen dos salidas de coincidencia, el timer 2 tiene cuatro.
- Los registros de match pueden detener, reiniciar o solo interrumpir el timer.
- Los registros de capture permiten capturar el valor de TC ante un flanco.

---

## 6. Drivers

Se deben crear instancias de estructuras, que luego se pasan por referencia (`&`) a funciones de configuración.

---

## 7. ADC

- Resolucion: 12 bits
- Rango de 3.3 [v] a GND
- El clock maximo es de 13 [MHz] que deriva del PCLOCK = CCLOCK/4 | CCLOCK | CCLOCK/2 | CCLOCK/8
- Convierte a  200 [KHz]
- Tiene 8 canales de conversion. Que dividen la frecuencia de conversion por la cantidad de canales habilitados