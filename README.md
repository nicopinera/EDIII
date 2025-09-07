# EDIII

> Material y recursos de la materia **Electrónica Digital III**
> Facultad de Ciencias Exactas, Físicas y Naturales (FCEFyN) - Universidad Nacional de Córdoba (UNC)

---

## Descripción general

Este repositorio contiene material teórico, práctico y proyectos de laboratorio para la cursada de Electrónica Digital 3. Está organizado para facilitar el acceso a apuntes, ejercicios, documentación técnica y ejemplos de código en C para microcontroladores (principalmente la familia LPC176x/5x de NXP).

---

## Estructura del repositorio

```text
EDIII/
│
├── Material/
│   ├── apunte.md
│   ├── ED3 - Ejercicios propuestos (taller).pdf
│   ├── LPCXpresso-LPC1769-CMSIS-DAP.pdf
│   ├── Opcional, para imprimir.pdf
│   ├── clases migliore/
│   │   └── (Clases teóricas en PDF)
│   ├── Practico/
│   │   ├── 01_C/ (Apuntes y ejercicios de C)
│   │   ├── 02_GPIO/ (Guías sobre GPIO)
│   │   ├── 03_SysTick/ (Material sobre temporizador SysTick)
│   │   └── 04_Interrupciones/ (Material sobre interrupciones)
│   └── Teorico/
│       └── (Manuales, datasheets y presentaciones)
│
├── Proyectos_MCXpresso/
│   ├── 01_Ejercicios_David/
│   ├── 02_Ayarde/
│   ├── 03_Gallardo/
│   └── 04_Migliore/
│       └── (Proyectos de laboratorio y ejemplos en C)
│
├── C_embebido/
│   └── 01embebido.c
│
├── script.sh
├── README.md
└── ...
```

---

## Contenido detallado

### Material/

- **apunte.md**: Apunte general de la materia.
- **ED3 - Ejercicios propuestos (taller).pdf**: Ejercicios prácticos para resolver en clase o como tarea.
- **LPCXpresso-LPC1769-CMSIS-DAP.pdf**: Manual de la placa y entorno de desarrollo LPCXpresso.
- **Opcional, para imprimir.pdf**: Material adicional para impresión.
- **clases migliore/**: Clases teóricas en PDF dictadas por el profesor Migliore.
- **Practico/**: Apuntes y ejercicios prácticos organizados por temas:
  - `01_C/`: Introducción al lenguaje C, operadores, preprocesador, punteros, memoria dinámica, etc.
  - `02_GPIO/`: Configuración y uso de pines GPIO en microcontroladores.
  - `03_SysTick/`: Uso del temporizador SysTick.
  - `04_Interrupciones/`: Manejo de interrupciones y controladores NVIC.
- **Teorico/**: Material teórico, datasheets, manuales de usuario y presentaciones.

### Proyectos_MCXpresso/

Proyectos de laboratorio y ejemplos de código en C para microcontroladores LPC176x/5x, organizados por docente o grupo:

- **01_Ejercicios_David/**: Ejercicios y ejemplos prácticos.
- **02_Ayarde/**: Clases prácticas y ejemplos.
- **03_Gallardo/**: Ejercicios, funciones y ejemplos de GPIO, interrupciones y SysTick.
- **04_Migliore/**: Ejercicios y ejemplos de laboratorio.

---

## ¿Cómo usar este repositorio?

1. Explora la carpeta `Material/` para acceder a apuntes, guías y ejercicios.
2. Revisa los proyectos en `Proyectos_MCXpresso/` para ejemplos de código y prácticas de laboratorio.
3. Utiliza los archivos de teoría y presentaciones para complementar el estudio.
4. Si tienes una placa LPCXpresso, puedes compilar y probar los ejemplos en C.

---

## Créditos y agradecimientos

- Docentes y ayudantes de Electrónica Digital 3 (FCEFyN-UNC).
- Contribuidores de material y ejemplos.

---

## Licencia

Este repositorio es solo para uso educativo. El material puede contener derechos de autor de terceros (manuales, datasheets, presentaciones, etc.).
