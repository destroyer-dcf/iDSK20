# Tipos de Archivo

[← Volver al índice](README.md)

---

## Introducción

iDSK20 soporta tres tipos de archivo para añadir al disco DSK. Cada tipo tiene un propósito específico y características diferentes.

> ℹ️ **Info:** No es necesario convertir los archivos a formato DOS antes de añadirlos. iDSK20 lo hace automáticamente.

---

## Resumen Rápido

| Tipo | Nombre | Cabecera | Conversión LF→CRLF | Uso Típico |
|------|--------|----------|-------------------|--------------|
| **A** | ASCII/BASIC | ✅ Sí | ✅ Sí | BASIC, texto |
| **B** | Binary | ✅ Sí | ❌ No  | Código máquina |
| **R** | Raw | ❌ No | ❌ No | Datos puros |

---

## Tipo A - ASCII/BASIC (Tipo 0)

### ¿Qué es?

Archivos de texto o programas BASIC con cabecera AMSDOS.

### ¿Cuándo usar?

- Programas BASIC (`.bas`)
- Archivos de texto (`.txt`)
- Archivos ASCII

### Características

- ✅ Convierte saltos de línea Unix (LF) a DOS (CRLF)
- ✅ Se puede ver con `LIST` en el CPC
- ✅ Se puede cargar con `LOAD` en el CPC

### Sintaxis

```bash
# Sintaxis CPC
iDSK20 save "programa.bas",A
iDSK20 save "readme.txt",A
iDSK20 save "datos.txt"      # A es el tipo por defecto

# Sintaxis tradicional
iDSK20 save midisco.dsk programa.bas -t 0
```

### Ejemplo Práctico

```bash
# Crear archivo BASIC
echo "10 PRINT \"HOLA MUNDO\"" > hola.bas

# Añadir al disco (tipo A por defecto)
iDSK20 save midisco.dsk "hola.bas"

# Ver en el CPC:
# LOAD "hola.bas"
# RUN
```

---

## Tipo B - Binary (Tipo 1)

### ¿Qué es?

Archivos binarios (código máquina) con cabecera AMSDOS y direcciones de carga/ejecución.

### ¿Cuándo usar?

- Programas en código máquina (`.bin`)
- Ejecutables compilados
- Código ensamblador compilado

### Características

- ✅ Añade cabecera AMSDOS con direcciones (128 bytes)
- ✅ Requiere especificar dirección de carga
- ✅ Opcionalmente dirección de ejecución
- ✅ Se puede ejecutar con `RUN` en el CPC

<!-- ### Direcciones Comunes

| Dirección | Decimal | Uso Típico |
|-----------|---------|------------|
| `4000` | 16384 | Música, datos |
| `8000` | 32768 | Código general |
| `A000` | 40960 | Código alto |
| `C000` | 49152 | Pantalla |
| `0170` | 368 | BASIC (carga) | -->

### Sintaxis

```bash
# Sintaxis CPC (con direcciones)
iDSK20 save "loader.bin",B,8000,8010  # Carga en 8000, ejecuta en 8010
iDSK20 save "game.bin",B,4000         # Carga y ejecuta en 4000

# Sintaxis tradicional
iDSK20 save midisco.dsk loader.bin -t 1 -c 8000 -e 8010
```

### Ejemplos Prácticos

```bash
# Loader que se ejecuta en 8000
iDSK20 save "loader.bin",B,8000,8000

# Pantalla que se carga en C000
iDSK20 save "screen.bin",B,C000,C000

# Música que se carga en 4000
iDSK20 save "music.bin",B,4000,4000

# Código compilado
iDSK20 save "game.bin",B,8000,8010
```

### Uso en el CPC

```basic
10 LOAD "loader.bin",8000
20 CALL 8000
```

---

## Tipo R - Raw (Tipo 2)

### ¿Qué es?

Archivo sin cabecera AMSDOS, datos puros tal cual.

### ¿Cuándo usar?

- Datos binarios puros
- Pantallas (`.scr`)
- Música
- Gráficos
- Mapas de niveles
- Cualquier dato que no necesite cabecera

### Características

- ✅ NO añade cabecera AMSDOS
- ✅ Archivo guardado exactamente como está
- ✅ Útil para datos que se cargan directamente en memoria
- ✅ Más pequeño (ahorra 128 bytes de cabecera)

### Sintaxis

```bash
# Sintaxis CPC
iDSK20 save "pantalla.scr",R
iDSK20 save "datos.dat",R
iDSK20 save "nivel1.map",R

# Sintaxis tradicional
iDSK20 save midisco.dsk pantalla.scr -t 2
```

### Ejemplos Prácticos

```bash
# Pantalla sin cabecera
iDSK20 save "screen.scr",R

# Datos de nivel
iDSK20 save "level1.dat",R
iDSK20 save "level2.dat",R

# Sprites
iDSK20 save "sprites.bin",R

# Música
iDSK20 save "music.dat",R
```

### Uso en el CPC

```basic
10 OPENIN "datos.dat"
20 ' Leer datos directamente
```


---

## Conversiones Automáticas

### Tipo A - Conversión LF→CRLF

iDSK20 convierte automáticamente los saltos de línea:

```
Unix/Linux (LF):     \n
Windows/DOS (CRLF):  \r\n
```

**Ejemplo:**
```bash
# Archivo Unix con LF
echo -e "10 PRINT \"HOLA\"\n20 END" > programa.bas

# iDSK20 lo convierte automáticamente a CRLF
iDSK20 save "programa.bas",A

# Mensaje: "Converted Unix line endings to DOS format (LF -> CRLF)"
```

---

## Ejemplos Completos

### Proyecto de Juego

```bash
export DISC_DSK_FILE=juego.dsk

# Crear disco
iDSK20 new

# Loader BASIC
iDSK20 save "loader.bas",A

# Código principal
iDSK20 save "main.bin",B,8000,8000

# Niveles (datos puros)
iDSK20 save "level1.dat",R
iDSK20 save "level2.dat",R
iDSK20 save "level3.dat",R

# Recursos
iDSK20 save "sprites.bin",R
iDSK20 save "music.bin",B,4000,4000

# Ver contenido
iDSK20 cat
```

### Proyecto de Demo

```bash
export DISC_DSK_FILE=demo.dsk

# Crear disco
iDSK20 new

# Loader
iDSK20 save "loader.bin",B,8000,8000

# Pantallas
iDSK20 save "screen1.scr",R
iDSK20 save "screen2.scr",R

# Música
iDSK20 save "music.dat",R

# Ver contenido
iDSK20 cat
```

---

[← Volver al índice](README.md) | [Siguiente: Wrappers →](WRAPPERS.md)
