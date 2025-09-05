# Apuntes clases Practicas

## Clases Gallardo

### Clase 1 (Grabada)

Estructura para configurar pines.

```cpp
LPC_PINCON->PINSEL; // SELECCIONAR FUNCION
LPC_PINCON->PINMODE; //SELECCIONAR FUNCIONAMIENTO DE RESISTENCIAS
```

Estructura para trabajar con modo GPIO el puerto x:

```cpp
LPC_GPIOx->FIODIR; // SETEAR DIRECCION
LPC_GPIOx->FIOSET; // SETEAR EN 1
LPC_GPIOx->FIOCLR; // SETEAR EN 0
LPC_GPIOx->FIOPIN; // DEVUELVE EL VALOR DE LOS PINES ACTUALES
```

### Clase 2 (Grabada)

Excepciones: Es una condicion que cambia el flujo normal de control de un programa. Son cambios de flujo producidas por hardware dentro del core.

Los perifericos generan interrupciones, por ejemplo el ADC, GPIO, etc.

Se caracterizan por:

- Numero de excepcion
- IRQ number
- Prioridad configurable
- Sincronas o asincronas

El bloque de hardware que controla las interrupciones es el NVIC, mejorando la latencia de atencion de las interrupciones o excepciones.

Registros del NVIC:

```cpp
NVIC->ISER[0-1]; // HABILITA INTERRUPCINES
NVIC->ICER[0-1]; // LIMPIA INTERRUPCIONES
NVIC->ISPR[0-1]; // NOS DICE SOBRE INTERRUPCIONES PENDIENTES
NVIC->ICPR[0-1]; // NOS LIMPIA LAS INTERRUPCIONES PENDIENTES
NVIC->IPR[0-8]; // NOS PERMITE SETTEAR LA PRIORIDAD
```

Primero se deshabilitan, se habilitan la interrupcion del periferico, se limpia la bandera de la interrupcion, se establece la prioridad y se vuelve a habilitar. Funciones del CMSIS para trabajar con interrupciones:

```cpp
void NVIC_EnableIRQ(IRQn_Type IRQn); // HABILITAR INTERRUPCION
void NVIC_DisableIRQ(IRQn_Type IRQn); // DESHABILITAR INTERRUPCION
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
```

### Clase 4/9

- Se puede activar la interrupcion por ambos flancos cuando la interrupcion es por GPIO

Ejercicio 2: Hay 4 llaves que te forman un numero binario, hay que sacar ese numero por un pin, sin que el numero se corte cuando entra un numero nuevo, almacenando los datos en un buffer cirtuclar

---
