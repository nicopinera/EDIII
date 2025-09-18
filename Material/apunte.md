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

#### **Timer**: cada timer tiene el Tc (contador) y PR (pre scaler). Todos son de 32 bits. La LPC tiene 4 timers. Los pre scaler tambien son de 32 bits.

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

Cada módulo de temporizador tiene su propio contador de temporizador (TC) y registro de preescala (PR) asociado. Cuando un temporizador se reinicia y se habilita, el TC se establece en 0 y se incrementa en 1 cada 'PR+1' ciclos de reloj, donde PR es el valor almacenado en el registro de preescala. Cuando alcanza su valor máximo, se restablece a 0 y, por lo tanto, reinicia el conteo. El registro de preescala se utiliza para definir la resolución del temporizador. Si PR es 0 , el TC se incrementa cada 1 ciclo de reloj del reloj periférico .

Si PR = 1 , el TC se incrementa cada 2 ciclos de reloj del reloj periférico, y así sucesivamente . Al establecer un valor apropiado en PR, podemos hacer que el temporizador incremente o cuente: cada ciclo de reloj periférico, 1 microsegundo, 1 milisegundo o 1 segundo, y así sucesivamente.

Cada temporizador tiene cuatro registros de coincidencia de 32 bits y cuatro registros de captura de 32 bits . Los temporizadores 0, 1 y 3 tienen dos salidas de coincidencia, mientras que el temporizador 2 tiene cuatro.
Registro de partidos

Un registro de coincidencia contiene un valor específico definido por el usuario. Al iniciarse el temporizador, cada vez que se incrementa el TC, el valor del TC se compara con el registro de coincidencia. Si coincide, se puede reiniciar el temporizador o generar una interrupción según lo definido por el usuario. En este tutorial, solo nos ocuparemos de los registros de coincidencia.

Los registros de coincidencias se pueden utilizar para:

- Detener el temporizador en caso de coincidencia (es decir, cuando el valor en el registro de conteo es el mismo que el del registro de coincidencia) y activar una interrupción opcional.
- Restablecer el temporizador en el partido y activar una interrupción opcional.
- Contar continuamente y provocar una interrupción en el partido.

##### Salida de coincidencia externa

Cuando un registro de coincidencia correspondiente (MRx) es igual al contador del temporizador (TC), la salida de coincidencia se puede controlar utilizando el registro de coincidencia externo (EMR) para: alternar, ir a ALTO, ir a BAJO o no hacer nada.
Registro de captura

Como su nombre indica, se utiliza para capturar la señal de entrada. Cuando ocurre un evento de transición en un pin de captura, se puede usar para copiar el valor de TC en cualquiera de los cuatro registros de captura o para generar una interrupción. Por lo tanto, también se pueden usar para demodular señales PWM. No los usaremos en este tutorial, ya que solo nos ocuparemos del uso del bloque Temporizador como temporizador. Los veremos en un tutorial posterior.
Registros utilizados para la programación del temporizador LPC1768

Utilizaremos el archivo de encabezado lpc17xx.h LPC_TIMx , donde x es el módulo del temporizador de 0 a 3. Por lo tanto, para el temporizador 1 usaremos LPC_TIM0 , y así sucesivamente. Se puede acceder a los registros desreferenciando el puntero mediante el operador "->". Por ejemplo, podemos acceder al TCR del bloque temporizador 0 como LPC_TIM0->TCR .Archivo de encabezado para programación. En CMSIS, todos los registros utilizados para programar y usar

temporizadores se definen como miembros de la estructura (puntero).
Veamos ahora algunos de los registros principales relacionados principalmente con el funcionamiento
del temporizador .

1. PR : Registro de preescala (32 bits) : almacena el valor máximo del contador de preescala después del cual se reinicia.

2. PC : Registro de preescala del contador (32 bits) : Este registro se incrementa con cada PCLK (reloj periférico). Controla la resolución del temporizador. Cuando PC alcanza el valor de PR, PC se restablece a 0 y el contador del temporizador se incrementa en 1. Por lo tanto, si PR = 0, el contador del temporizador se incrementa con cada PCLK. Si PR = 9, el contador del temporizador se incrementa con cada décimo ciclo de PCLK. Por lo tanto, al seleccionar un valor de preescala adecuado, podemos controlar la resolución del temporizador.

3. TC : Registro del Contador del Temporizador (32 bits) : Este es el registro principal de conteo. El contador del temporizador se incrementa cuando el PC alcanza su valor máximo, según lo especificado por el PR. Si el temporizador no se reinicia explícitamente (directamente) ni mediante una interrupción, actuará como un contador de ejecución libre que se reinicia a cero cuando alcanza su valor máximo, que es 0xFFFFFFFF.

4. TCR : Registro de Control del Temporizador . Este registro se utiliza para habilitar, deshabilitar y reiniciar el temporizador. Cuando el bit 0 es 1, el temporizador está habilitado y cuando es 0, está deshabilitado. Cuando el bit 1 es 1, el temporizador y el PC se ponen a cero simultáneamente en el siguiente flanco positivo de PCLK. El resto de los bits del TCR están reservados.

5. CTCR : Registro de control de conteo . Se utiliza para seleccionar el modo temporizador/contador. Para nuestro propósito, siempre lo usaremos en el modo temporizador. Cuando el valor de CTCR se establece en 0x0, se selecciona el modo temporizador.

6. MCR : Registro de Control de Coincidencia . Este registro controla qué operaciones se pueden realizar cuando el valor de MR coincide con el de TC. Los bits 0, 1 y 2 corresponden a MR0, los bits 3, 4 y 5 a MR1, y así sucesivamente. A continuación, se muestra una tabla rápida que muestra su uso:

Para MR0:

- Bit 0: Interrupción en MR0, es decir, se activa una interrupción cuando MR0 coincide con TC. Las interrupciones se habilitan cuando se establecen en 1 y se deshabilitan cuando se establecen en 0.
- Bit 1: Reinicio en MR0. Al establecerse en 1, el TC se reiniciará al coincidir con MR0. Se deshabilita al establecerse en 0.
- Bit 2: Detener en MR0. Cuando se establece en 1, TC y PC se detendrán cuando MR0 coincida con TC.

De manera similar, los bits 3-5, 6-8, 9-11 son para MR1, MR2 y MR3 respectivamente.

7. IR : Registro de Interrupciones . Contiene los indicadores de interrupción para 4 interrupciones de coincidencia y 4 de captura. Los bits 0 a 3 corresponden a las interrupciones MR0 a MR3, respectivamente. De forma similar, los 4 siguientes corresponden a las interrupciones CR0-3. Cuando se activa una interrupción, el bit correspondiente en IR se establece en 1 y, en caso contrario, en 0. Escribir un 1 en la ubicación del bit correspondiente restablece la interrupción, lo que se utiliza para confirmar la finalización de la ejecución del ISR correspondiente.

8. EMR : Registro de Coincidencia Externa : Proporciona el estado y el control de los pines de salida de coincidencia externa. Los primeros cuatro bits corresponden a EM0 a EM3. Los siguientes 8 bits corresponden a EMC0 a EMC3 en pares de 2.

- Bit 0 – EM0: Coincidencia externa 0. Cuando se produce una coincidencia entre TC y MR0, dependiendo de los bits [5:4] (es decir, EMC0) de este registro, este bit puede alternar, pasar a nivel BAJO, pasar a nivel ALTO o no hacer nada. Este bit se activa en MATx.0, donde x = número de temporizador.
- Lo mismo ocurre con los bits 1, 2 y 3.
- Bits[5:4] – EMC0: Coincidencia externa 0. Los valores de estos bits seleccionan la funcionalidad de EM0 de la siguiente manera:
  - 0x0 – No hacer nada
  - 0x1 – Borra la salida de coincidencia externa correspondiente a 0 (el pin MATx.m está BAJO).
  - 0x2 – Establece la salida de coincidencia externa correspondiente en 1 (el pin MATx.m es ALTO).
  - 0x3 – Alternar la salida de coincidencia externa correspondiente.
  - De manera similar para Bits[7:6] – EMC1, Bits[9,8] – EMC2, Bits[11:10] – EMC3.

Nota: Los temporizadores 0/1/3 solo tienen dos salidas de coincidencia fijadas, mientras que el temporizador 2 tiene cuatro. Por lo tanto, EM2, EM3, EMC2 y EMC3 no son aplicables al temporizador 0/1/3.

Recomendaría utilizar la siguiente secuencia para configurar temporizadores:

1. Establezca el valor apropiado en LPC_TIMx->CTCR
2. Defina el valor de preescala en LPC_TIMx->PR
3. Establezca valores en los registros de coincidencia si es necesario
4. Establezca el valor apropiado en LPC_TIMx->MCR si utiliza registros de coincidencia/interrupciones
5. Temporizador de reinicio: reinicia PR y TC
6. Establezca LPC_TIMx->TCR en 0x01 para habilitar el temporizador cuando sea necesario
7. Restablezca LPC_TIMx->TCR a 0x00 para deshabilitar el temporizador cuando sea necesario

Configuración del reloj periférico para el temporizador: el reloj de entrada para los temporizadores se puede configurar utilizando los registros de selección de reloj periférico PCLKSEL0 y PCLKSEL1 .

- Para el temporizador 0 se utilizan los bits [3:2] en PCLKSEL0.
- Para el temporizador 1 se utilizan los bits [5:4] en PCLKSEL0.
- Para el temporizador 2 se utilizan los bits [13:12] en PCLKSEL1.
- Para el temporizador 3 se utilizan los bits [15:14] en PCLKSEL1.

Estos bits nos permiten elegir 4 divisores CCLK (SystemCoreClock en el código de inicio) diferentes para obtener el PCLK final de la siguiente manera:

- [00] – PCLK = CCLK/4 (Predeterminado después del reinicio)
- [01] – PCLK = CCLK
- [10] – PCLK = CCLK/2
- [11] – PCLK = CCLK/8

### Drivers

Hay que crear instancias de estructuras, las cuales despues se pasan por referencia (&) a una funcion para configurar.
