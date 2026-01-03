# Comandos de iDSK20

[← Volver al índice](README.md)

---

## Índice de Comandos

### Comandos Básicos
- [new](#comando-new) - Crear nuevo disco DSK
- [cat](#comando-cat) - Listar contenido del disco
- [save](#comando-save) - Añadir archivo al disco ⭐
- [get](#comando-get) - Extraer archivo del disco
- [era](#comando-era) - Borrar archivo del disco
- [list](#comando-list) - Ver contenido de archivo BASIC
- [hex](#comando-hex) - Ver archivo en hexadecimal
- [disasm](#comando-disasm) - Desensamblar archivo binario
- [dams](#comando-dams) - Ver archivo DAMS
- [run](#comando-run) - Ejecutar en RetroVirtualMachine
- [help](#comando-help) - Mostrar ayuda

---

## Sintaxis General

```bash
iDSK20 <comando> [disco.dsk] <argumentos> [opciones]
```

**Nota:** El parámetro `[disco.dsk]` es opcional si configuras la variable de entorno `DISC_DSK_FILE`.

---

## Comando: `new`

**Descripción:** Crea una nueva imagen de disco DSK vacía.

**Sintaxis:**
```bash
iDSK20 new <disco.dsk>
```

**Parámetros:**
- `<disco.dsk>` - Nombre del archivo DSK a crear

**Características:**
- Crea un disco DSK estándar de 178 KB
- Sobrescribe el archivo si ya existe

**Ejemplos:**
```bash
# Crear un disco nuevo
iDSK20 new midisco.dsk

# Crear disco para un proyecto
iDSK20 new juego.dsk

```

**Notas:**
> ⚠️ **Advertencia:** Si el archivo ya existe, será sobrescrito sin confirmación.

---

## Comando: `cat`

**Descripción:** Lista el contenido del disco DSK mostrando todos los archivos.

**Sintaxis:**
```bash
iDSK20 cat [disco.dsk]
```

**Parámetros:**
- `[disco.dsk]` - Nombre del disco (opcional si `DISC_DSK_FILE` está configurado)

**Información mostrada:**
- Nombre del archivo (formato 8.3)
- Tipo de archivo (ASCII, Binary, Raw)
- Tamaño en KB
- Atributos (Read-only, System)
- Usuario (0-15)
- Direcciones de carga/ejecución (para binarios)
- Espacio libre en el disco

**Ejemplos:**
```bash
# Listar contenido especificando el disco
iDSK20 cat midisco.dsk

# Listar usando variable de entorno
export DISC_DSK_FILE=midisco.dsk
iDSK20 cat

# Guardar listado en archivo
iDSK20 cat midisco.dsk > contenido.txt
```

**Salida típica:**
```
DSK : midisco.dsk
PROGRAMA.BAS    ASCII    2 Ko  User 0
LOADER  .BIN    Binary   4 Ko  User 0  [Load:8000 Exec:8010]
LEVEL1  .DAT    Raw      8 Ko  User 0
MUSIC   .BIN    Binary   12 Ko User 0  [Load:4000 Exec:4000]
---
Free space: 152 Ko
```

---

## Comando: `save`

**Descripción:** Añade un archivo al disco DSK. Soporta sintaxis CPC y sintaxis tradicional.

### Sintaxis CPC (Recomendada) ⭐

```bash
iDSK20 save [disco.dsk] "<archivo>"[,tipo][,carga,ejecución][,atributos]
```

**Parámetros:**
- `[disco.dsk]` - Nombre del disco (opcional si `DISC_DSK_FILE` está configurado)
- `<archivo>` - Archivo local a añadir
- `tipo` - Tipo de archivo: `A` (ASCII), `B` (Binary), `R` (Raw)
- `carga` - Dirección de carga en hexadecimal (solo tipo B)
- `ejecución` - Dirección de ejecución en hexadecimal (solo tipo B)
- `atributos` - `P` (Protected/Read-only), `S` (System)

**Tipos de Archivo:**

| Tipo | Nombre | Descripción | Cuándo Usar |
|------|--------|-------------|-------------|
| **A** | ASCII/BASIC | Archivo de texto o BASIC | Programas BASIC, archivos de texto |
| **B** | Binary | Archivo binario con direcciones | Programas en código máquina |
| **R** | Raw | Archivo sin cabecera AMSDOS | Datos puros, pantallas, música |

Ver más detalles en [Tipos de Archivo](TIPOS.md).

**Atributos Opcionales:**

| Atributo | Nombre | Descripción |
|----------|--------|-------------|
| **P** | Protected | Archivo protegido (solo lectura) |
| **S** | System | Archivo de sistema |

**Ejemplos - Sintaxis CPC:**
```bash
# Configurar disco por defecto (recomendado)
export DISC_DSK_FILE=midisco.dsk

# Archivo BASIC (tipo A - por defecto)
iDSK20 save "programa.bas"
iDSK20 save "programa.bas",A

# Archivo binario con direcciones
iDSK20 save "loader.bin",B,8000,8010

# Archivo raw sin cabecera
iDSK20 save "pantalla.scr",R

# Archivo protegido
iDSK20 save "config.cfg",A,P

# Binario protegido
iDSK20 save "juego.bin",B,4000,4000,P
```

### Sintaxis Tradicional

```bash
iDSK20 save <disco.dsk> <archivo> [opciones]
```

**Opciones:**
- `-t <tipo>` - Tipo: 0=ASCII, 1=Binary, 2=Raw
- `-c <addr>` - Dirección de carga (hex)
- `-e <addr>` - Dirección de ejecución (hex)
- `-f` - Forzar sobrescritura
- `-o` - Marcar como read-only
- `-s` - Marcar como system file
- `-u <num>` - Número de usuario (0-15)

**Ejemplos - Sintaxis Tradicional:**
```bash
# Archivo ASCII
iDSK20 save midisco.dsk programa.bas -t 0

# Archivo binario
iDSK20 save midisco.dsk loader.bin -t 1 -c 8000 -e 8010

# Archivo raw
iDSK20 save midisco.dsk datos.dat -t 2

# Forzar sobrescritura
iDSK20 save midisco.dsk programa.bas -t 0 -f

# Archivo protegido
iDSK20 save midisco.dsk config.cfg -t 0 -o
```

**Conversiones Automáticas:**
- **Tipo A**: Convierte LF a CRLF automáticamente
- **Tipo B**: Añade cabecera AMSDOS con direcciones (128 bytes)
- **Tipo R**: Sin conversiones, datos puros (0 bytes extra)

**Notas:**
> 📝 **Nota:** El tipo A es el predeterminado si no se especifica.
> 
> 📝 **Nota:** No es necesario convertir los archivos a formato DOS antes de añadirlos, iDSK20 lo hace automáticamente.
> 
> ⚠️ **Advertencia:** Si el archivo ya existe en el disco, usa `-f` para sobrescribir.

---

## Comando: `get`

**Descripción:** Extrae un archivo del disco DSK al sistema local.

**Sintaxis:**
```bash
iDSK20 get [disco.dsk] "<archivo>"
```

**Parámetros:**
- `[disco.dsk]` - Nombre del disco (opcional si `DISC_DSK_FILE` está configurado)
- `<archivo>` - Nombre del archivo en el disco (formato CPC)

**Características:**
- Extrae el archivo con su contenido original
- Respeta el tipo de archivo (ASCII, Binary, Raw)
- Preserva las direcciones de carga/ejecución en binarios

**Ejemplos:**
```bash
# Extraer archivo especificando disco
iDSK20 get midisco.dsk "programa.bas"

# Extraer usando variable de entorno
export DISC_DSK_FILE=midisco.dsk
iDSK20 get "programa.bas"

# Extraer todos los archivos (script)
for file in $(iDSK20 cat | grep -v "^DSK" | grep -v "^---" | grep -v "free" | awk '{print $1}'); do
    iDSK20 get "$file"
done
```

**Notas:**
> 📝 **Nota:** El archivo extraído sobrescribirá el archivo local si ya existe.

---

## Comando: `era`

**Descripción:** Borra un archivo del disco DSK.

**Sintaxis:**
```bash
iDSK20 era [disco.dsk] "<archivo>"
```

**Parámetros:**
- `[disco.dsk]` - Nombre del disco (opcional si `DISC_DSK_FILE` está configurado)
- `<archivo>` - Nombre del archivo a borrar (formato CPC)

**Características:**
- Elimina el archivo del directorio del disco
- Libera el espacio ocupado
- No se puede deshacer (sin papelera de reciclaje)

**Ejemplos:**
```bash
# Borrar archivo especificando disco
iDSK20 era midisco.dsk "programa.bas"

# Borrar usando variable de entorno
export DISC_DSK_FILE=midisco.dsk
iDSK20 era "programa.bas"

# Borrar y reemplazar
iDSK20 era "viejo.bas"
iDSK20 save "nuevo.bas",A

# Borrar múltiples archivos (script)
for file in "temp1.dat" "temp2.dat" "temp3.dat"; do
    iDSK20 era "$file"
done
```

**Notas:**
> ⚠️ **Advertencia:** Esta operación no se puede deshacer. Haz backup antes de borrar archivos importantes.

---

## Comando: `list`

**Descripción:** Muestra el contenido de un archivo BASIC del disco.

**Sintaxis:**
```bash
iDSK20 list [disco.dsk] "<archivo>"
```

**Parámetros:**
- `[disco.dsk]` - Nombre del disco (opcional si `DISC_DSK_FILE` está configurado)
- `<archivo>` - Nombre del archivo BASIC a mostrar

**Características:**
- Decodifica tokens BASIC del CPC
- Muestra el programa formateado

**Ejemplos:**
```bash
# Ver programa BASIC especificando disco
iDSK20 list midisco.dsk "programa.bas"

# Ver usando variable de entorno
export DISC_DSK_FILE=midisco.dsk
iDSK20 list "programa.bas"

# Guardar en archivo de texto
iDSK20 list "programa.bas" > programa.txt
```

**Salida típica:**
```basic
10 MODE 1
20 PRINT "HOLA MUNDO"
30 FOR I=1 TO 10
40 PRINT "LINEA ";I
50 NEXT I
60 END
```

**Notas:**
> 📝 **Nota:** Solo funciona con archivos BASIC (tipo A). Para archivos binarios usa `hex` o `disasm`.

---

## Comando: `hex`

**Descripción:** Muestra el contenido de un archivo en formato hexadecimal.

**Sintaxis:**
```bash
iDSK20 hex [disco.dsk] "<archivo>"
```

**Parámetros:**
- `[disco.dsk]` - Nombre del disco (opcional si `DISC_DSK_FILE` está configurado)
- `<archivo>` - Nombre del archivo a mostrar

**Formato de salida:**
```
#0000 3E 01 CD 5A BB C9 00 00 00 00 00 00 00 00 00 00 | >.Z.............
#0010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 | ................
```

**Características:**
- Muestra el contenido completo del archivo
- Formato hexadecimal con representación ASCII
- Offset en hexadecimal (#0000, #0010, etc.)
- 16 bytes por línea

**Ejemplos:**
```bash
# Ver archivo en hexadecimal
iDSK20 hex midisco.dsk "datos.bin"

# Ver usando variable de entorno
export DISC_DSK_FILE=midisco.dsk
iDSK20 hex "datos.bin"

# Guardar en archivo de texto
iDSK20 hex "datos.bin" > dump.txt
```

**Notas:**
> 📝 **Nota:** Útil para inspeccionar archivos binarios, datos, o verificar contenido byte por byte.

---

## Comando: `disasm`

**Descripción:** Desensambla un archivo binario mostrando código Z80.

**Sintaxis:**
```bash
iDSK20 disasm [disco.dsk] "<archivo>"
```

**Parámetros:**
- `[disco.dsk]` - Nombre del disco (opcional si `DISC_DSK_FILE` está configurado)
- `<archivo>` - Nombre del archivo binario a desensamblar

**Formato de salida:**
```
Address  Hex          Mnemonic
-------- ------------ ------------------
8000     3E 01        LD A,01
8002     CD 5A BB     CALL BB5A
8005     C9           RET
```

**Ejemplos:**
```bash
# Desensamblar archivo
iDSK20 disasm midisco.dsk "codigo.bin"

# Desensamblar usando variable de entorno
export DISC_DSK_FILE=midisco.dsk
iDSK20 disasm "codigo.bin"

# Guardar en archivo de texto
iDSK20 disasm "codigo.bin" > codigo.asm
```

**Notas:**
> 📝 **Nota:** El desensamblador es básico. Para análisis avanzado usa herramientas especializadas como Ghidra o IDA.

---

## Comando: `dams`

**Descripción:** Muestra el contenido de un archivo fuente DAMS (ensamblador).

**Sintaxis:**
```bash
iDSK20 dams [disco.dsk] "<archivo>"
```

**Parámetros:**
- `[disco.dsk]` - Nombre del disco (opcional si `DISC_DSK_FILE` está configurado)
- `<archivo>` - Nombre del archivo DAMS

**Características:**
- Decodifica formato DAMS
- Muestra código fuente ensamblador
- Preserva comentarios y etiquetas

**Ejemplos:**
```bash
# Ver archivo DAMS
iDSK20 dams midisco.dsk "fuente.asm"

# Ver usando variable de entorno
export DISC_DSK_FILE=midisco.dsk
iDSK20 dams "fuente.asm"

# Guardar en archivo de texto
iDSK20 dams "fuente.asm" > fuente.txt
```

**Notas:**
> 📝 **Nota:** DAMS es un ensamblador popular para Amstrad CPC.

---

## Comando: `run`

**Descripción:** Ejecuta un programa del disco en RetroVirtualMachine.

**Sintaxis:**
```bash
iDSK20 run [disco.dsk] "<archivo>"
```

**Parámetros:**
- `[disco.dsk]` - Nombre del disco (opcional si `DISC_DSK_FILE` está configurado)
- `<archivo>` - Nombre del archivo a ejecutar

**Requisitos:**
- RetroVirtualMachine 2.0 instalado
- Variable `RVM_PATH` configurada con la ruta al ejecutable

**Variables de entorno opcionales:**
- `CPC_MODEL` - Modelo de CPC: 464, 664, 6128 (por defecto: 6128)

**Ejemplos:**
```bash
# Configurar RetroVirtualMachine
export RVM_PATH="/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"

# Ejecutar programa especificando disco
iDSK20 run midisco.dsk "juego.bas"

# Ejecutar usando variable de entorno
export DISC_DSK_FILE=midisco.dsk
iDSK20 run "juego.bas"

# Ejecutar en CPC 464
export CPC_MODEL=464
iDSK20 run "juego.bas"

# Ejecutar en CPC 6128 (por defecto)
export CPC_MODEL=6128
iDSK20 run "juego.bas"
```

**Notas:**
> 📝 **Nota:** RetroVirtualMachine debe estar instalado y configurado correctamente.
> 
> ⚠️ **Advertencia:** Solo es válida la versión de [RetroVirtualMachine](https://www.retrovirtualmachine.org/blog/future) 2.0 con opciones de desarrollo activadas.
> 
> 💡 **Tip:** Usa la variable `CPC_MODEL` para cambiar el modelo de CPC sin modificar el comando.

---

## Comando: `help`

**Descripción:** Muestra la ayuda general o ayuda específica de un comando.

**Sintaxis:**
```bash
iDSK20 help [comando]
```

**Parámetros:**
- `[comando]` - Comando específico para ver ayuda detallada (opcional)

**Ejemplos:**
```bash
# Ayuda general
iDSK20 help

# Ayuda de comando específico
iDSK20 help save
iDSK20 help cat
iDSK20 help new

# Sin colores
iDSK20 --no-color help
```

**Notas:**
> 📝 **Nota:** La ayuda incluye ejemplos y sintaxis para cada comando.

---

## Resumen de Comandos

| Comando | Descripción | Soporta DISC_DSK_FILE |
|---------|-------------|----------------------|
| `new` | Crear disco DSK | ❌ No |
| `cat` | Listar contenido | ✅ Sí |
| `save` | Añadir archivo | ✅ Sí |
| `get` | Extraer archivo | ✅ Sí |
| `era` | Borrar archivo | ✅ Sí |
| `list` | Ver BASIC | ✅ Sí |
| `hex` | Ver hexadecimal | ✅ Sí |
| `disasm` | Desensamblar | ✅ Sí |
| `dams` | Ver DAMS | ✅ Sí |
| `run` | Ejecutar en RVM | ✅ Sí |
| `help` | Mostrar ayuda | - |

---

[← Volver al índice](README.md) | [Siguiente: Variables de Entorno →](VARIABLES.md)
