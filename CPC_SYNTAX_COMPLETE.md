# Sintaxis CPC Completa para iDSK20

## Resumen
Se ha implementado y documentado completamente la sintaxis CPC para el comando `save`, compatible con el comando SAVE de Amstrad CPC.

## Sintaxis Completa

### Con archivo DSK explícito
```
iDSK20 save <dskfile> "<file>"[,type][,load,exec][,attributes]
```

### Con variable de entorno DISC_DSK_FILE
```
export DISC_DSK_FILE=mydisk.dsk
iDSK20 save "<file>"[,type][,load,exec][,attributes]
```

**¡Importante!** Cuando se usa la variable de entorno `DISC_DSK_FILE`, la sintaxis es **idéntica** al comando SAVE de Amstrad CPC (solo se añade `iDSK20 save` al principio).

## Tipos de Archivo

| Tipo | Descripción | Valor interno | Header AMSDOS |
|------|-------------|---------------|---------------|
| **A** | ASCII/BASIC | 0 | Sí |
| **B** | Binary | 1 | Sí (con direcciones) |
| **R** | Raw | 2 | No |

### Detalles de los Tipos

#### A - ASCII/BASIC (tipo 0)
- Añade header AMSDOS
- Para archivos de texto y BASIC
- Tipo por defecto si no se especifica
- Ejemplo: `"program.bas",A`

#### B - Binary (tipo 1)
- Añade header AMSDOS con direcciones
- Requiere dirección de carga (load)
- Opcionalmente dirección de ejecución (exec)
- Ejemplo: `"loader.bin",B,8000,8010`

#### R - Raw (tipo 2)
- NO añade header AMSDOS
- Archivo guardado tal cual
- Útil para datos binarios puros
- Ejemplo: `"data.dat",R`

## Atributos (Opcionales)

| Atributo | Descripción | Se puede combinar |
|----------|-------------|-------------------|
| **P** | Protected (read-only) | Sí, con cualquier tipo |
| **S** | System file | Sí, con cualquier tipo |

### Combinaciones de Atributos

Los atributos se pueden combinar con los tipos:
- `"file",A,P` - ASCII protegido
- `"file",B,8000,8010,P` - Binary protegido
- `"file",R,S` - Raw system file
- `"file",A,P,S` o `"file",A,PS` - ASCII protegido y system

## Ejemplos Completos

### Uso Recomendado: Con Variable de Entorno

La forma más cómoda y compatible con CPC es usar la variable de entorno:

```bash
# Configurar el disco una vez
export DISC_DSK_FILE=mydisk.dsk

# Ahora la sintaxis es casi idéntica a CPC
iDSK20 save "program.bas"              # ASCII por defecto
iDSK20 save "program.bas",A            # ASCII explícito
iDSK20 save "loader.bin",B,8000,8010   # Binary
iDSK20 save "data.dat",R               # Raw
iDSK20 save "config.cfg",A,P           # ASCII protegido

# Otros comandos también usan la variable
iDSK20 cat                             # Listar contenido
iDSK20 get "program.bas"               # Extraer archivo
iDSK20 era "oldfile.bas"               # Borrar archivo
```

### Archivos ASCII/BASIC

```bash
# ASCII por defecto (sin especificar tipo)
iDSK20 save floppy.dsk "program.bas"

# ASCII explícito
iDSK20 save floppy.dsk "program.bas",A

# ASCII protegido (read-only)
iDSK20 save floppy.dsk "config.cfg",A,P

# ASCII system file
iDSK20 save floppy.dsk "startup.bas",A,S
```

### Archivos Binarios

```bash
# Binary con direcciones
iDSK20 save floppy.dsk "loader.bin",B,8000,8010

# Binary protegido
iDSK20 save floppy.dsk "game.bin",B,4000,4000,P

# Binary con misma dirección de carga y ejecución
iDSK20 save floppy.dsk "code.bin",B,C000,C000
```

### Archivos Raw (sin header)

```bash
# Raw simple
iDSK20 save floppy.dsk "data.dat",R

# Raw protegido
iDSK20 save floppy.dsk "screen.scr",R,P

# Raw system
iDSK20 save floppy.dsk "font.bin",R,S
```

## Comparación con Amstrad CPC

### En Amstrad CPC BASIC

```basic
SAVE "program.bas"              ' ASCII por defecto
SAVE "program.bas",A            ' ASCII explícito
SAVE "loader.bin",B,&8000,&8010 ' Binary con direcciones
SAVE "config.cfg",P             ' Protegido (read-only)
```

### En iDSK20 (con variable de entorno)

```bash
export DISC_DSK_FILE=disk.dsk

iDSK20 save "program.bas"              # ASCII por defecto - ¡IDÉNTICO a CPC!
iDSK20 save "program.bas",A            # ASCII explícito - ¡IDÉNTICO a CPC!
iDSK20 save "loader.bin",B,8000,8010   # Binary con direcciones
iDSK20 save "config.cfg",A,P           # ASCII protegido
```

### En iDSK20 (sin variable de entorno)

```bash
iDSK20 save disk.dsk "program.bas"              # ASCII por defecto
iDSK20 save disk.dsk "program.bas",A            # ASCII explícito
iDSK20 save disk.dsk "loader.bin",B,8000,8010   # Binary con direcciones
iDSK20 save disk.dsk "config.cfg",A,P           # ASCII protegido
```

**Diferencias:**
1. **Con DISC_DSK_FILE**: Sintaxis prácticamente idéntica a CPC (solo añadir `iDSK20 save`)
2. **Sin DISC_DSK_FILE**: Hay que especificar el archivo DSK como primer parámetro
3. iDSK20 usa direcciones hexadecimales sin el prefijo `&`
4. En CPC, `P` solo marca como protegido (tipo ASCII por defecto)
5. En iDSK20, `P` es un atributo que se combina con el tipo

## Sintaxis Tradicional Equivalente

Cada sintaxis CPC tiene su equivalente con flags tradicionales:

| Sintaxis CPC | Sintaxis Tradicional |
|--------------|---------------------|
| `"file",A` | `file -t 0` |
| `"file",B,8000,8010` | `file -t 1 -c 8000 -e 8010` |
| `"file",R` | `file -t 2` |
| `"file",A,P` | `file -t 0 -o` |
| `"file",B,8000,8010,P` | `file -t 1 -c 8000 -e 8010 -o` |
| `"file",A,S` | `file -t 0 -s` |

## Implementación

### Función de Parsing

La función `parseCPCStyle()` en `Main.cpp` procesa la sintaxis:

```cpp
bool parseCPCStyle(const string& arg, 
                   string& filename, 
                   int& type, 
                   int& loadAddr, 
                   int& execAddr, 
                   bool& readOnly, 
                   bool& systemFile)
```

### Lógica de Parsing

1. **Divide por comas** - Separa los parámetros
2. **Extrae filename** - Primer parámetro (quita comillas)
3. **Procesa tipos** - A, B, R (establece el tipo de archivo)
4. **Procesa atributos** - P, S (establece flags)
5. **Procesa direcciones** - Valores hexadecimales (load y exec)

### Orden de Parámetros

El orden es flexible, pero se recomienda:
```
"filename",tipo,load,exec,atributos
```

Ejemplos válidos:
- `"file",B,8000,8010,P` ✅
- `"file",B,P,8000,8010` ✅ (también funciona)
- `"file",P,B,8000,8010` ✅ (también funciona)

## Casos Especiales

### Solo Filename
```bash
iDSK20 save disk.dsk "program.bas"
# Tipo: 0 (ASCII)
# Atributos: ninguno
```

### Solo Tipo
```bash
iDSK20 save disk.dsk "data.dat",R
# Tipo: 2 (Raw)
# Atributos: ninguno
```

### Solo Atributo
```bash
iDSK20 save disk.dsk "config.cfg",P
# Tipo: 0 (ASCII por defecto)
# Atributos: read-only
```

### Binary sin Exec
```bash
iDSK20 save disk.dsk "code.bin",B,8000
# Load: 8000
# Exec: 8000 (mismo que load por defecto)
```

## Validación

### Pruebas Realizadas

```bash
# Crear disco de prueba
./build/iDSK20 new test.dsk

# Probar diferentes sintaxis
./build/iDSK20 save test.dsk "test1.bas",A
./build/iDSK20 save test.dsk "test2.bin",B,4000,4000
./build/iDSK20 save test.dsk "test3.dat",R
./build/iDSK20 save test.dsk "test4.cfg",A,P
./build/iDSK20 save test.dsk "test5.bin",B,8000,8010,P

# Verificar
./build/iDSK20 cat test.dsk
```

## Beneficios

1. **Compatibilidad** - Sintaxis familiar para usuarios de CPC
2. **Flexibilidad** - Soporta todas las combinaciones
3. **Claridad** - Documentación completa en la ayuda
4. **Potencia** - Combina tipos y atributos libremente

## Documentación en la Ayuda

La ayuda del comando muestra:
- Sintaxis completa con parámetros opcionales
- Tabla de tipos de archivo
- Tabla de atributos
- 6 ejemplos prácticos
- Explicación de cada tipo y atributo

Para ver la ayuda:
```bash
./build/iDSK20 help
```

---

**Fecha:** 2 de enero de 2025  
**Autor:** Destroyer  
**Estado:** ✅ Completado y documentado  
**Versión:** iDSK20 2.0.0
