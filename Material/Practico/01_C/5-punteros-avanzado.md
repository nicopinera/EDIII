# Punteros y arreglos

En C, los arreglos y los punteros están estrechamente relacionados.
En muchas expresiones, **el nombre de un arreglo se convierte automáticamente** (o *"decay"*) **en un puntero a su primer elemento**.

Por ejemplo, si tienes:
```c
int arr[10];
```
entonces, en la mayoría de los contextos, `arr` puede usarse como si fuera de tipo `int*` apuntando a `arr[0]`.
De hecho, `arr` y `&arr[0]` devuelven la **misma dirección**.
Podrías hacer:
```c
int *p = arr;
```
y entonces `p` apuntará al inicio del arreglo. Después de esa asignación:

- `p[0]` es igual a `arr[0]`
- `*(p + 1)` es igual a `arr[1]`
- y así sucesivamente.

Esto es muy útil para **iterar sobre arreglos** con aritmética de punteros o **pasar arreglos a funciones** (ya que los parámetros de tipo arreglo en realidad reciben punteros).

> **IMPORTANTE**
>
> Aunque `arr` (en una expresión) actúa como un puntero al primer elemento, hay una distinción clave:

- `arr` **no** es un *lvalue* modificable → no puedes hacer `arr = otroArray;`
  (en su declaración, `arr` es una referencia constante a un bloque fijo de memoria).
- En cambio, un puntero como `p` **sí** puede reasignarse para apuntar a otro lado.

En resumen: los arreglos “se comportan como” punteros en expresiones, pero **no son completamente intercambiables**.
Aun así, entender que `arr` puede tratarse como un puntero a su primer elemento es fundamental.

---

### Ejemplo:
```c
int numbers[3] = {5, 10, 15};
int *ptr = numbers;            // ptr apunta a numbers[0]

printf("%d\n", ptr[2]);       // imprime 15 (ptr[2] == *(ptr + 2))
ptr[1] = 20;                   // modifica numbers[1] a 20
printf("%d\n", numbers[1]);   // imprime 20, reflejando el cambio
printf("%d\n", *arr);        // imprime 5 (arr[0]) , desreferencia de arr

```

Aquí, `ptr[2]` y `numbers[2]` se refieren al **mismo elemento**.
El estándar de C define que `ptr[i]` es equivalente a `*(ptr + i)`.

---
Incluso se puede hacer algo como `2[numbers]` porque es igual a `*(2 + numbers)` → mismo que `numbers[2]`.
Pero **no lo hagas nunca en código real**; solo sirve para ilustrar que el índice `x[y]` se interpreta como `*(x + y)`.

---

### Patrón común de iteración con punteros:
```c
int arr[N];
int *end = arr + N;   // apunta a una posición más allá del último elemento

for (int *p = arr; p < end; ++p) {
    printf("%d\n", *p);
}
```
Este bucle recorre el arreglo desde el inicio (`arr`) hasta `end` (sin incluirlo), desreferenciando en cada paso para imprimir el valor.
Evita usar una variable de índice y puede ser muy eficiente.

---

# Punteros y cadenas

En C, las **cadenas** se representan típicamente como **arreglos de `char` terminados en el carácter nulo** (`'\0'`).
Dado que los arreglos y punteros están relacionados, una cadena puede manipularse fácilmente con un `char*`.

Ejemplo:
```c
char str[] = "Hello";  // arreglo de chars (H, e, l, l, o, '\0')
char *p = str;         // p apunta a 'H'

while (*p) {           // recorre hasta encontrar '\0'
    printf("%c\n", *p);
    p++;
}
```

Muchas funciones estándar (como `strlen`, `strcpy`, etc.) usan `char*` para procesar cadenas.
Por ejemplo:
```c
int strlen(const char *s);
```
recibe un `const char*` apuntando a la cadena y recorre la memoria hasta encontrar `'\0'`.

---

### Cuidado con los literales de cadena:
Si haces:
```c
char *msg = "Hello";
```
`msg` apuntará a un **literal de cadena**, que normalmente está en una zona de memoria **de solo lectura**. No se está copiando a la RAM, sino que apunta a la flash.
Intentar modificarlo (ej., `msg[0] = 'J';`) es **comportamiento indefinido** .
> `msg` si se guarda en la RAM, pero apuntará a la flash (zona .rodata).




Si necesitas modificar la cadena, se copia primero a un arreglo (esto sí genera una copia en la RAM):
```c
char msg[] = "Hello";  // ahora se puede modificar
msg[0] = 'J';          // válido
```


**Ventaja de usar punteros con cadenas:**
Permite manejo dinámico, paso eficiente de parámetros a funciones y manipulación directa de la memoria subyacente.
La iteración puede hacerse con índices (`str[i]`) o aritmética de punteros (`*(str + i)`), según la preferencia y el caso de uso.

## Punteros y estructuras

Los punteros también pueden apuntar a tipos estructurados (`struct` o `union`).
Un puntero a una estructura permite:

- **Pasar estructuras grandes de forma eficiente** (se pasa solo la dirección en lugar de copiar toda la estructura).
- **Crear estructuras enlazadas** (como listas, árboles, etc., donde cada nodo contiene un puntero al siguiente).

Ejemplo de declaración:
```c
struct Point {
    int x;
    int y;
};

struct Point p1;            // declaración de una estructura tipo Point


struct Point *pPtr = &p1;   // declaración de un puntero a la estructura p1
````

Aquí, `pPtr` es un puntero a `struct Point`.

---

### Acceso a miembros de estructura con punteros

En C, para acceder a los miembros de una estructura a través de un puntero se usa el **operador flecha `->`**.

* `pPtr->x` accede al campo `x` de la estructura a la que `pPtr` apunta.
* `pPtr->x` es equivalente a `(*pPtr).x` (se desreferencia el puntero y luego se accede al campo).

El operador `->` hace el código más legible.

Ejemplo:

```c
struct Point p1 = {2, 3};
p1.x = 10; // se puede modificar el valor de x e y directamente
p1.y = 20;


struct Point *pPtr = &p1;

printf("%d, %d\n", pPtr->x, pPtr->y);  // imprime "10, 20"

pPtr->x = 11;                          // tambien se puede modificar el valor de x e y mediante el puntero

printf("%d\n", p1.x);                  // imprime "11", reflejando el cambio
```


> El operador `->` funciona tanto para punteros a estructuras como a *unions*.

---

### Uso común en estructuras enlazadas

Los punteros a estructuras se usan intensamente en estructuras de datos dinámicas:
por ejemplo, en una lista enlazada cada nodo contiene un puntero al siguiente.

También son útiles al pasar estructuras a funciones:
en lugar de pasar la estructura por valor (lo que copia todo su contenido), se pasa un puntero para ganar eficiencia.

---

## Punteros a punteros (multinivel)

Así como podemos tener un puntero a un `int` o a un `char`, podemos tener un puntero **a otro puntero**.

Un puntero a puntero (o doble puntero) se declara con un `*` adicional:

```c
int **pp;   // pp es un puntero a un int*
```

---

### Desglose con ejemplo:

```c
int x = 5;        // x es un int
int *p = &x;      // p es un puntero a int (contiene la dirección de x)
int **pp = &p;    // pp es un puntero a puntero a int (contiene la dirección de p)
```

En este escenario:

* `*pp` es de tipo `int*` → es el puntero `p`.
* `**pp` es de tipo `int` → es el valor al que apunta `p` (o sea, `x`).

```c
printf("%d\n", **pp); // imprime 5
```

---

###  Para que se usan los punteros a punteros?

1. **Arreglos 2D dinámicos:**
   Para reservar memoria manualmente para una matriz bidimensional:

   ```c
   int **matrix;
   matrix = malloc(rows * sizeof(int*));
   for (int i = 0; i < rows; i++) {
       matrix[i] = malloc(cols * sizeof(int));
   }
   ```

2. **Pasar punteros a funciones para modificarlos:**
   Si quieres que una función asigne memoria y devuelva el puntero a través de un parámetro:

   ```c
   void allocateArray(int **p, int size) {
       *p = malloc(size * sizeof(int));
   }

   int *arr;
   allocateArray(&arr, 10); // arr queda inicializado en main
   ```

3. **Argumentos de línea de comandos (`argv`):**
   En `main(int argc, char **argv)`, `argv` es un puntero a puntero a `char` (arreglo de cadenas).

---

### Ejemplo práctico:

```c
int a = 100;
int *p = &a;
int **pp = &p;

printf("%d\n", **pp);  // imprime 100

*pp = NULL;            // cambia p a NULL
```

Aquí, `*pp = NULL;` modifica el puntero `p` (ya que `pp` apunta a `p`).
Después de esa línea, `p` es `NULL`.

Esto demuestra que un puntero a puntero permite **manipular el puntero original** (no solo el valor al que apunta) desde otra función o contexto.

---

📌 Podes tener más niveles (`***` para triple puntero, etc.), pero rara vez se necesitan a menos que trabajes con datos muy complejos o arreglos multidimensionales.
El caso más común en C es el **doble puntero**.
