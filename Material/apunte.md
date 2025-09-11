# Apuntes clases Practicas

## Clase Ayarde

### Repaso programacion en C

La programacion es una serie de pasos que el programador debe seguir

1. Diseño del algoritmo
2. Codificacion
3. Compilacion
4. Ejecucion

## Clases Gallardo

### Clase 1 (Grabada)

Estructura para configurar pines.

```cpp
// SELECCIONAR FUNCION
LPC_PINCON->PINSEL;

//SELECCIONAR FUNCIONAMIENTO DE RESISTENCIAS
LPC_PINCON->PINMODE;
```

Estructura para trabajar con modo GPIO el puerto x:

```cpp
// SETEAR DIRECCION
LPC_GPIOx->FIODIR;

// SETEAR EN 1
LPC_GPIOx->FIOSET;

// SETEAR EN 0
LPC_GPIOx->FIOCLR;

// DEVUELVE EL VALOR DE LOS PINES ACTUALES
LPC_GPIOx->FIOPIN;
```

### Clase 2 (Grabada)

Excepciones: Es una condicion que cambia el flujo normal de control de un programa. Son cambios de flujo producidas por hardware dentro del core.

Los perifericos generan interrupciones, por ejemplo el ADC, GPIO, etc.

Se caracterizan por:

- Numero de excepcion
- IRQ number o Exception number
- Prioridad configurable
- Sincronas o asincronas

El bloque de hardware que controla las interrupciones es el NVIC, mejorando la latencia de atencion de las interrupciones o excepciones.

#### Registros del NVIC

```cpp
// HABILITA INTERRUPCINES
NVIC->ISER[0-1];

// LIMPIA INTERRUPCIONES
NVIC->ICER[0-1];

// NOS DICE SOBRE INTERRUPCIONES PENDIENTES
NVIC->ISPR[0-1];

// NOS LIMPIA LAS INTERRUPCIONES PENDIENTES
NVIC->ICPR[0-1];

// NOS PERMITE SETTEAR LA PRIORIDAD
NVIC->IPR[0-8];
```

Primero se deshabilitan, se habilitan la interrupcion del periferico, se limpia la bandera de la interrupcion, se establece la prioridad y se vuelve a habilitar. Funciones del CMSIS para trabajar con interrupciones:

```cpp
// HABILITAR INTERRUPCION
void NVIC_EnableIRQ(IRQn_Type IRQn);

// DESHABILITAR INTERRUPCION
void NVIC_DisableIRQ(IRQn_Type IRQn);

// Settea la proridad de la interrupcion
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
```

##### Configuracion

1. Quien interrumpe? Hay que ver las fuentes de interrupcion posibles
2. COn que prioridad?
3. Habilitar la interrupcion requerida
4. Que va a hacer la interrupcion? Definir la funcion del handler para cada interrupcion

#### Interrupciones por GPIO

Los GPIO por si solo pueden interrumpir por flancos, el handler que atiende esas interrupciones es el de EINT3. Ademas debo habilitar las interrupciones externas 3 y ahi moverme con las de GPIO.

Se pueden habilitar las interrupciones por ambos flancos (ascendente o descendente)

```cpp
// HABILITA FLANCO ASCENDENTE
LPC_GPIOINT->IntEnR;

// Habilita Flanco Descendente
LPC_GPIOINT->IntEnF;

// Muestra quien interrumpio por flanco ascendente
LPC_GPIOINT->IntStatR;

//Muestra quien interrumpio por flanco descedente
LPC_GPIOINT->IntStatF;
```

> Ejercicio 2: Hay 4 llaves que te forman un numero binario, hay que sacar ese numero por un pin, sin que el numero se corte cuando entra un numero nuevo, almacenando los datos en un buffer cirtuclar

#### SysTick

Es un contador descendente propio del core, que viene preconfigurado para interrumpir cada $10[ms]$. Se puede precargar con un valor para que interrumpa cada cierto tiempo. Al ser un bloque propio del core, no genera interrupciones sino excepciones.

```cpp
// Prende y apaga el contador, habilita o no la interrupcion, selecciona la fuente del clock y tiene la bandera de la interrupcion
LPC_SysTick->STCTRL;

// Settea el valor que se va a precargar cada vez que llegue a cero
LPC_SysTick->STRELOAD;

// Valor actual del contador
LPC_SysTick->STCURR;

// registro de calibracion para interrumpir cada 10 ms si el clock es de 100 MHz
LPC_SysTick->STCALIB;

```

```CPP
SystemInit(); // COnfigura el CClock en 100MHz (frecuencia del core)
  // SysTick->CTRL; Prendido o apagado, interrupcion y la fuente del clock

  // Tenemos que ver en que valor esta el divisor de frecuencia del core
  // para saber que valor cargarle tenemos que
  //  Tfinal = (1/Fcore)* (Vload+1) -> Vload = (Tfinal)/(1/Fcore) - 1
  // SysTick->LOAD=Vload;

  // Puedo saber el valor del reloj
  // SysTick->VAL;

  // SI quiero que interrumpa cada 20 Hz, el tiempo es 1/20 = 50ms
```

### Parcial 1 - 2025

El primer ejercicio la forma de la señal se puede guardar en una variable e ir mostrando cada vez que se interrumpe. Con eso te evita controlar los if

```c
#define sequence_length = (sizeof(sequence)/sizeof(uint8_t)) // largo de la secuencia
```

El segundo ejercicio era por flanco, no por nivel.

**Timer**: cada timer tiene el Tc (contador) y PR (pre scaler). Todos son de 32 bits. La LPC tiene 4 timers. Los pre scaler tambien son de 32 bits.

$$T_{interrupcion} = ((V_{valorTimerMax}-V_{preCargado})*PR+1)*T_{clock}= ((2^{32}-V_{precargado})*PR+1)*T_{clock}$$

Hay 4 registros **MATCH**, cuando **TC==MATCH** puedo hacer que interrumpa, o que se resetee el contador, o ambas.

Tenemos 4 timer que tienen la misma estructura de registros

- PR: Almacena el valor del pre scaler
- PC: Se incrementa en cada pulso de clock, cuando alcanza el valor de PR, vuelve a 0 y se aumenta el TC
- TC: Contador
- TCR: Registro de control
- CTCR: Count Control Register
- MCR: Match control register, configuracion de los match
- IR: Interrupt Register
- EMR: External Match Register

1. PONER VALOR EN `LPC_TIMx->CTCR`
2. DEFINIR `LPC_TIMx->PR`
3. CARGAR EN MATCH REGISTER SI ES NECESARIO
4. CARGAR EL VALOR EN `LPC_TIMx->MCR` SI USAS MATCH

En los timer se utilizan los periferical clock **PCLOCK**, sale del clock del core dividido, puede dividirse por /4, /2, que no divida y /8 . El Timer del sistema por default esta en $100 [MHz]$ lo que corresponde a $0.01[\mu s]$ cada tick del clock. Podes configurar que cuando timer llegue a determinado valor, interrumpa a traves de los registros match.

Puede interrumpir por match o por capture, para cualquiera le das dos hay que habilitar la interrupcion general del timer. Capture, captura un flanco ascendente o descendente e interrumpe. 

> Perido: Entre dos flancos ascendentes
> Duty cicly: Entre un flanco ascendente y otro descendente

### Drivers

Hay que crear instancias de estructuras, las cuales despues se pasan por referencia (&) a una funcion para configurar.
