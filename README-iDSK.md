# 📀 iDSK20 - Editor de Imágenes DSK para Amstrad CPC

**Versión 2.0.0** | Editor moderno de archivos DSK (imágenes de disco) para Amstrad CPC

---

## 📋 Tabla de Contenidos

1. [¿Qué es iDSK20?](#-qué-es-idsk20)
2. [Instalación](#-instalación)
3. [Primeros Pasos](#-primeros-pasos)
4. [Comandos Disponibles](#-comandos-disponibles)
5. [Sintaxis CPC (Recomendada)](#-sintaxis-cpc-recomendada)
6. [Variables de Entorno](#-variables-de-entorno)
7. [Ejemplos Prácticos](#-ejemplos-prácticos)
8. [Tipos de Archivo](#-tipos-de-archivo)
9. [Solución de Problemas](#-solución-de-problemas)
10. [Preguntas Frecuentes](#-preguntas-frecuentes)

---

## 🎯 ¿Qué es iDSK20?

iDSK20 es una herramienta de línea de comandos que te permite trabajar con imágenes de disco DSK del Amstrad CPC. Con iDSK20 puedes:

- ✅ **Crear** nuevas imágenes de disco DSK
- ✅ **Añadir** archivos a un disco DSK
- ✅ **Extraer** archivos de un disco DSK
- ✅ **Listar** el contenido de un disco DSK
- ✅ **Borrar** archivos de un disco DSK
- ✅ **Ver** el contenido de archivos BASIC, binarios, etc.
- ✅ **Ejecutar** programas en RetroVirtualMachine

**¿Por qué usar iDSK20?**
- 🚀 Sintaxis compatible con Amstrad CPC (comando SAVE)
- 🎨 Interfaz con colores para mejor legibilidad
- 🔧 Fácil de usar con variables de entorno
- 🌍 Multiplataforma (Windows, macOS, Linux)
- 📚 Documentación completa en español

---

## 💻 Instalación

### Requisitos Previos

Necesitas tener instalado:
- **CMake** (versión 3.10 o superior)
- **Compilador C++** (GCC, Clang, o MSVC)
- **Git** (para clonar el repositorio)


### Paso 1: Descargar el Código

```bash
# Clonar el repositorio
git clone https://github.com/CPCReady/tools.git
cd tools/disc
```

### Paso 2: Compilar

#### En macOS y Linux

```bash
# Dar permisos de ejecución al script
chmod +x build.sh

# Compilar
./build.sh
```

#### En Windows

```bash
# Abrir PowerShell o CMD en la carpeta disc
.\build.sh
```

### Paso 3: Verificar la Instalación

```bash
# Probar que funciona
./build/iDSK20 help
```

Si ves el menú de ayuda con colores, ¡la instalación fue exitosa! 🎉

### Instalación Opcional: Añadir al PATH

Para poder usar `iDSK20` desde cualquier carpeta:

#### En macOS/Linux

```bash
# Añadir al PATH (temporal, solo para esta sesión)
export PATH=$PATH:$(pwd)/build

# O añadir permanentemente al ~/.bashrc o ~/.zshrc
echo 'export PATH=$PATH:/ruta/completa/a/tools/disc/build' >> ~/.bashrc
source ~/.bashrc
```

#### En Windows

```powershell
# Añadir al PATH del sistema
# Panel de Control > Sistema > Configuración avanzada > Variables de entorno
# Añadir la ruta: C:\ruta\a\tools\disc\build
```

---


## 🚀 Primeros Pasos

### Tu Primer Disco DSK

Vamos a crear tu primer disco DSK y añadir un archivo:

```bash
# 1. Crear un nuevo disco DSK
./build/iDSK20 new midisco.dsk

# 2. Crear un archivo de prueba
echo "10 PRINT \"HOLA MUNDO\"" > hola.bas

# 3. Añadir el archivo al disco
./build/iDSK20 save midisco.dsk "hola.bas",A

# 4. Ver el contenido del disco
./build/iDSK20 cat midisco.dsk
```

¡Felicidades! Has creado tu primer disco DSK con iDSK20 🎉

### Usando Variables de Entorno (Recomendado)

Para no tener que escribir el nombre del disco cada vez:

```bash
# Configurar el disco por defecto
export DISC_DSK_FILE=midisco.dsk

# Ahora puedes omitir el nombre del disco
iDSK20 save "hola.bas",A
iDSK20 cat
iDSK20 get "hola.bas"
```

---

## 📚 Comandos Disponibles

### Comandos Básicos

| Comando | Descripción | Ejemplo |
|---------|-------------|---------|
| `new` | Crear nuevo disco DSK | `iDSK20 new disco.dsk` |
| `cat` | Listar contenido del disco | `iDSK20 cat disco.dsk` |
| `save` | Añadir archivo al disco | `iDSK20 save disco.dsk archivo.bas` |
| `get` | Extraer archivo del disco | `iDSK20 get disco.dsk archivo.bas` |
| `era` | Borrar archivo del disco | `iDSK20 era disco.dsk archivo.bas` |
| `help` | Mostrar ayuda | `iDSK20 help` |

### Comandos Avanzados

| Comando | Descripción | Ejemplo |
|---------|-------------|---------|
| `list` | Ver contenido de un archivo | `iDSK20 list disco.dsk programa.bas` |
| `hex` | Ver archivo en hexadecimal | `iDSK20 hex disco.dsk datos.bin` |
| `disasm` | Desensamblar archivo binario | `iDSK20 disasm disco.dsk codigo.bin` |
| `dams` | Ver archivo DAMS | `iDSK20 dams disco.dsk fuente.asm` |
| `run` | Ejecutar en RetroVirtualMachine | `iDSK20 run disco.dsk juego.bas` |

---


## 🎮 Sintaxis CPC (Recomendada)

iDSK20 soporta una sintaxis **idéntica** al comando SAVE de Amstrad CPC. ¡Es la forma más fácil de usar!

### Sintaxis Básica

```
iDSK20 save "<archivo>"[,tipo][,carga,ejecución][,atributos]
```

### Tipos de Archivo

| Tipo | Nombre | Descripción | Cuándo Usar |
|------|--------|-------------|-------------|
| **A** | ASCII/BASIC | Archivo de texto o BASIC | Programas BASIC, archivos de texto |
| **B** | Binary | Archivo binario con direcciones | Programas en código máquina |
| **R** | Raw | Archivo sin cabecera AMSDOS | Datos puros, pantallas, música |

### Atributos Opcionales

| Atributo | Nombre | Descripción |
|----------|--------|-------------|
| **P** | Protected | Archivo protegido (solo lectura) |
| **S** | System | Archivo de sistema |

### Ejemplos Rápidos

```bash
# Configurar disco por defecto (recomendado)
export DISC_DSK_FILE=midisco.dsk

# Archivo BASIC (tipo A - por defecto)
iDSK20 save "programa.bas"
iDSK20 save "programa.bas",A

# Archivo binario (tipo B - con direcciones)
iDSK20 save "loader.bin",B,8000,8010

# Archivo raw sin cabecera (tipo R)
iDSK20 save "pantalla.scr",R

# Archivo protegido (atributo P)
iDSK20 save "config.cfg",A,P

# Binario protegido
iDSK20 save "juego.bin",B,4000,4000,P
```

### Comparación con Amstrad CPC

| En Amstrad CPC | En iDSK20 (con DISC_DSK_FILE) |
|----------------|-------------------------------|
| `SAVE "programa.bas"` | `iDSK20 save "programa.bas"` |
| `SAVE "programa.bas",A` | `iDSK20 save "programa.bas",A` |
| `SAVE "loader.bin",B,&8000,&8010` | `iDSK20 save "loader.bin",B,8000,8010` |
| `SAVE "config.cfg",P` | `iDSK20 save "config.cfg",A,P` |

**¡Casi idéntico!** Solo añades `iDSK20 save` al principio 🎯

---


## 🔧 Variables de Entorno

Las variables de entorno te permiten configurar iDSK20 una vez y olvidarte de repetir parámetros.

### DISC_DSK_FILE (Muy Recomendada)

**¿Qué hace?** Define el disco DSK por defecto para todos los comandos.

**¿Por qué usarla?** No tienes que escribir el nombre del disco cada vez.

#### Configuración

```bash
# En macOS/Linux
export DISC_DSK_FILE=midisco.dsk

# En Windows (PowerShell)
$env:DISC_DSK_FILE="midisco.dsk"

# En Windows (CMD)
set DISC_DSK_FILE=midisco.dsk
```

#### Uso

```bash
# Sin variable de entorno (tedioso)
iDSK20 save midisco.dsk "archivo1.bas",A
iDSK20 save midisco.dsk "archivo2.bin",B,8000,8010
iDSK20 cat midisco.dsk

# Con variable de entorno (¡mucho mejor!)
export DISC_DSK_FILE=midisco.dsk
iDSK20 save "archivo1.bas",A
iDSK20 save "archivo2.bin",B,8000,8010
iDSK20 cat
```

### RVM_PATH (Para Ejecutar Imagenes de Disco)

**¿Qué hace?** Define la ruta a RetroVirtualMachine para ejecutar programas.

**¿Cuándo usarla?** Si quieres probar tus programas directamente desde iDSK20.

#### Configuración

```bash
# En macOS
export RVM_PATH="/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"

# En Linux
export RVM_PATH="/usr/bin/retrovirtualmachine"

# En Windows
set RVM_PATH="C:\Program Files\RetroVirtualMachine\RetroVirtualMachine.exe"
```

#### Uso

```bash
# Ejecutar un programa
iDSK20 run midisco.dsk "juego.bas"
```

### CPC_MODEL (Modelo de CPC)

**¿Qué hace?** Define el modelo de CPC para ejecutar programas.

**Valores posibles:** `464`, `664`, `6128` (por defecto: `6128`)

```bash
export CPC_MODEL=464
iDSK20 run midisco.dsk "programa.bas"
```

### NO_COLOR / DISC_NO_COLOR (Desactivar Colores)

**¿Qué hace?** Desactiva los colores ANSI en la salida.

**¿Cuándo usarla?** Si tu terminal no soporta colores o prefieres texto plano.

```bash
export NO_COLOR=1
iDSK20 help
```

### Configuración Permanente

Para no tener que configurar las variables cada vez:

#### En macOS/Linux

```bash
# Editar ~/.bashrc o ~/.zshrc
nano ~/.bashrc

# Añadir al final:
export DISC_DSK_FILE=~/cpc/midisco.dsk
export RVM_PATH="/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"

# Recargar configuración
source ~/.bashrc
```

#### En Windows

1. Buscar "Variables de entorno" en el menú inicio
2. Clic en "Variables de entorno"
3. En "Variables de usuario", clic en "Nueva"
4. Nombre: `DISC_DSK_FILE`
5. Valor: `C:\cpc\midisco.dsk`
6. Aceptar

---


## 💡 Ejemplos Prácticos

### Ejemplo 1: Crear un Disco de Juego

```bash
# 1. Configurar entorno
export DISC_DSK_FILE=juego.dsk

# 2. Crear disco
iDSK20 new

# 3. Añadir archivos
iDSK20 save "loader.bas",A           # Cargador BASIC
iDSK20 save "main.bin",B,8000,8000   # Programa principal
iDSK20 save "level1.dat",R           # Datos del nivel 1
iDSK20 save "level2.dat",R           # Datos del nivel 2
iDSK20 save "music.bin",B,4000,4000  # Música

# 4. Ver contenido
iDSK20 cat

# 5. Probar en emulador
iDSK20 run "loader.bas"
```

### Ejemplo 2: Extraer Archivos de un Disco

```bash
# 1. Ver qué hay en el disco
iDSK20 cat juego.dsk

# 2. Extraer un archivo específico
iDSK20 get juego.dsk "loader.bas"

# 3. Extraer todos los archivos (usando script)
for file in $(iDSK20 cat juego.dsk | grep -v "^$" | awk '{print $1}'); do
    iDSK20 get juego.dsk "$file"
done
```

### Ejemplo 3: Desarrollo con Makefile

Crea un archivo `Makefile`:

```makefile
# Configuración
DISK = game.dsk
COMPILER = z88dk-zcc +cpc

# Compilar y crear disco
all: compile disk

# Compilar código
compile:
	$(COMPILER) -o game.bin game.c

# Crear disco y añadir archivos
disk:
	export DISC_DSK_FILE=$(DISK) && \
	iDSK20 new && \
	iDSK20 save "loader.bas",A && \
	iDSK20 save "game.bin",B,8000,8000 && \
	iDSK20 cat

# Probar en emulador
test:
	export DISC_DSK_FILE=$(DISK) && \
	iDSK20 run "loader.bas"

# Limpiar
clean:
	rm -f game.bin $(DISK)
```

Uso:
```bash
make all    # Compilar y crear disco
make test   # Probar en emulador
make clean  # Limpiar
```

### Ejemplo 4: Convertir Archivos de Texto

```bash
# Convertir archivo de texto a formato CPC
export DISC_DSK_FILE=textos.dsk
iDSK20 new

# Añadir archivos de texto
iDSK20 save "readme.txt",A
iDSK20 save "manual.txt",A
iDSK20 save "creditos.txt",A

# Ver contenido de un archivo
iDSK20 list "readme.txt"
```

### Ejemplo 5: Gestionar Múltiples Discos

```bash
# Crear varios discos
iDSK20 new disco1.dsk
iDSK20 new disco2.dsk
iDSK20 new disco3.dsk

# Trabajar con disco 1
export DISC_DSK_FILE=disco1.dsk
iDSK20 save "programa1.bas",A
iDSK20 cat

# Cambiar a disco 2
export DISC_DSK_FILE=disco2.dsk
iDSK20 save "programa2.bas",A
iDSK20 cat

# Cambiar a disco 3
export DISC_DSK_FILE=disco3.dsk
iDSK20 save "programa3.bas",A
iDSK20 cat
```

---


## 📖 Tipos de Archivo

### Tipo A - ASCII/BASIC (Tipo 0)

**¿Qué es?** Archivos de texto o programas BASIC con cabecera AMSDOS.

**¿Cuándo usar?**
- Programas BASIC (`.bas`)
- Archivos de texto (`.txt`)
- Archivos ASCII

**Características:**
- ✅ Añade cabecera AMSDOS automáticamente
- ✅ Convierte saltos de línea Unix (LF) a DOS (CRLF)
- ✅ Se puede ver con `LIST` en el CPC
- ✅ Se puede cargar con `LOAD` en el CPC

**Ejemplos:**
```bash
iDSK20 save "programa.bas",A
iDSK20 save "readme.txt",A
iDSK20 save "datos.txt"      # A es el tipo por defecto
```

### Tipo B - Binary (Tipo 1)

**¿Qué es?** Archivos binarios (código máquina) con cabecera AMSDOS y direcciones.

**¿Cuándo usar?**
- Programas en código máquina (`.bin`)
- Ejecutables compilados
- Código ensamblador compilado

**Características:**
- ✅ Añade cabecera AMSDOS con direcciones de carga y ejecución
- ✅ Requiere especificar dirección de carga
- ✅ Opcionalmente dirección de ejecución
- ✅ Se puede ejecutar con `RUN` en el CPC

**Formato de direcciones:**
- Hexadecimal sin prefijo: `8000`, `4000`, `C000`
- Si solo se especifica carga, ejecución = carga

**Ejemplos:**
```bash
# Con dirección de carga y ejecución
iDSK20 save "loader.bin",B,8000,8010

# Solo dirección de carga (ejecución = carga)
iDSK20 save "game.bin",B,4000

# Direcciones comunes en CPC
iDSK20 save "screen.bin",B,C000,C000  # Pantalla
iDSK20 save "music.bin",B,4000,4000   # Música
iDSK20 save "code.bin",B,8000,8000    # Código
```

### Tipo R - Raw (Tipo 2)

**¿Qué es?** Archivo sin cabecera AMSDOS, datos puros.

**¿Cuándo usar?**
- Datos binarios puros
- Pantallas (`.scr`)
- Música
- Gráficos
- Cualquier dato que no necesite cabecera

**Características:**
- ✅ NO añade cabecera AMSDOS
- ✅ Archivo guardado exactamente como está
- ✅ Útil para datos que se cargan directamente en memoria
- ✅ Más pequeño (ahorra 128 bytes de cabecera)

**Ejemplos:**
```bash
iDSK20 save "pantalla.scr",R
iDSK20 save "datos.dat",R
iDSK20 save "nivel1.map",R
iDSK20 save "sprites.bin",R
```

### Tabla Comparativa

| Característica | Tipo A (ASCII) | Tipo B (Binary) | Tipo R (Raw) |
|----------------|----------------|-----------------|--------------|
| Cabecera AMSDOS | ✅ Sí | ✅ Sí | ❌ No |
| Direcciones | ❌ No | ✅ Sí | ❌ No |
| Conversión LF→CRLF | ✅ Sí | ❌ No | ❌ No |
| Tamaño extra | +128 bytes | +128 bytes | 0 bytes |
| Uso típico | BASIC, texto | Código máquina | Datos puros |
| Comando CPC | `LOAD` | `LOAD` + `CALL` | Carga directa |

### ¿Qué Tipo Usar?

```
┌─────────────────────────────────────┐
│ ¿Es un programa BASIC?              │
│ ├─ Sí → Tipo A                      │
│ └─ No ↓                             │
│                                     │
│ ¿Es código máquina ejecutable?     │
│ ├─ Sí → Tipo B (con direcciones)   │
│ └─ No ↓                             │
│                                     │
│ ¿Son datos puros sin cabecera?     │
│ └─ Sí → Tipo R                      │
└─────────────────────────────────────┘
```

---


## 🔍 Solución de Problemas

### Problema: "Command not found: iDSK20"

**Causa:** El programa no está en el PATH o no se ha compilado.

**Solución:**
```bash
# Opción 1: Usar ruta completa
./build/iDSK20 help

# Opción 2: Añadir al PATH
export PATH=$PATH:$(pwd)/build
iDSK20 help

# Opción 3: Verificar que se compiló
ls -la build/iDSK20
```

### Problema: "Error reading file"

**Causa:** El archivo no existe o la ruta es incorrecta.

**Solución:**
```bash
# Verificar que el archivo existe
ls -la archivo.bas

# Usar ruta completa si es necesario
iDSK20 save disco.dsk "/ruta/completa/archivo.bas",A

# Verificar permisos
chmod 644 archivo.bas
```

### Problema: "Disk image not found"

**Causa:** El disco DSK no existe o DISC_DSK_FILE no está configurado.

**Solución:**
```bash
# Crear el disco primero
iDSK20 new midisco.dsk

# O configurar la variable de entorno
export DISC_DSK_FILE=midisco.dsk

# Verificar que existe
ls -la midisco.dsk
```

### Problema: "File already exists"

**Causa:** Ya existe un archivo con ese nombre en el disco.

**Solución:**
```bash
# Opción 1: Borrar el archivo primero
iDSK20 era disco.dsk "archivo.bas"
iDSK20 save disco.dsk "archivo.bas",A

# Opción 2: Usar flag -f (force)
iDSK20 save disco.dsk archivo.bas -t 0 -f
```

### Problema: Los colores no se ven bien

**Causa:** Tu terminal no soporta colores ANSI.

**Solución:**
```bash
# Desactivar colores
export NO_COLOR=1
iDSK20 help

# O usar flag --no-color
iDSK20 --no-color help
```

### Problema: "Invalid CPC syntax"

**Causa:** Error en la sintaxis CPC.

**Solución:**
```bash
# ❌ Incorrecto
iDSK20 save archivo.bas,A

# ✅ Correcto (con comillas)
iDSK20 save "archivo.bas",A

# ❌ Incorrecto (falta tipo antes de direcciones)
iDSK20 save "loader.bin",8000,8010

# ✅ Correcto (tipo B para binarios)
iDSK20 save "loader.bin",B,8000,8010
```

### Problema: El archivo no se ejecuta en el CPC

**Causa:** Tipo de archivo incorrecto o direcciones erróneas.

**Solución:**
```bash
# Para BASIC: usar tipo A
iDSK20 save "programa.bas",A

# Para binarios: verificar direcciones
# Direcciones comunes:
# - &8000 (32768) - Código general
# - &4000 (16384) - Música/datos
# - &C000 (49152) - Pantalla

iDSK20 save "codigo.bin",B,8000,8000
```

### Problema: Compilación falla

**Causa:** Faltan dependencias o CMake no está instalado.

**Solución:**
```bash
# En macOS
brew install cmake

# En Ubuntu/Debian
sudo apt-get install cmake build-essential

# En Windows
# Descargar CMake de: https://cmake.org/download/

# Limpiar y recompilar
rm -rf build
./build.sh
```

---


## ❓ Preguntas Frecuentes

### ¿Qué diferencia hay entre iDSK20 y el iDSK original?

iDSK20 es una versión modernizada con:
- ✅ Sintaxis compatible con Amstrad CPC
- ✅ Interfaz con colores
- ✅ Mejor manejo de variables de entorno
- ✅ Documentación completa en español
- ✅ Más opciones y comandos

### ¿Puedo usar iDSK20 con emuladores?

Sí, iDSK20 genera archivos DSK estándar compatibles con:
- RetroVirtualMachine
- WinAPE
- Arnold
- JavaCPC
- Cualquier emulador que soporte formato DSK

### ¿Cómo convierto archivos de texto a formato CPC?

iDSK20 lo hace automáticamente:
```bash
# Convierte LF a CRLF automáticamente
iDSK20 save "texto.txt",A
```

### ¿Puedo añadir archivos grandes?

Sí, pero ten en cuenta:
- Disco DSK estándar: 178 KB libres
- Archivo máximo: ~178 KB
- Si necesitas más espacio, usa múltiples discos

### ¿Cómo protejo un archivo contra escritura?

Usa el atributo `P`:
```bash
iDSK20 save "importante.bas",A,P
```

### ¿Puedo usar iDSK20 en scripts?

Sí, iDSK20 está diseñado para automatización:
```bash
#!/bin/bash
export DISC_DSK_FILE=auto.dsk
iDSK20 new
for file in *.bas; do
    iDSK20 save "$file",A
done
iDSK20 cat
```

### ¿Cómo veo el contenido de un archivo BASIC?

```bash
# Ver contenido
iDSK20 list disco.dsk "programa.bas"

# Ver con paginación
iDSK20 list disco.dsk "programa.bas" -p
```

### ¿Qué formato de direcciones debo usar?

Hexadecimal sin prefijo:
```bash
# ✅ Correcto
iDSK20 save "code.bin",B,8000,8010
iDSK20 save "screen.bin",B,C000,C000

# ❌ Incorrecto (no usar &)
iDSK20 save "code.bin",B,&8000,&8010
```

### ¿Puedo renombrar archivos en el disco?

No directamente, pero puedes:
```bash
# 1. Extraer el archivo
iDSK20 get disco.dsk "viejo.bas"

# 2. Renombrar localmente
mv viejo.bas nuevo.bas

# 3. Borrar el viejo
iDSK20 era disco.dsk "viejo.bas"

# 4. Añadir el nuevo
iDSK20 save disco.dsk "nuevo.bas",A
```

### ¿Cómo hago backup de un disco?

```bash
# Simplemente copia el archivo DSK
cp midisco.dsk midisco-backup.dsk

# O con fecha
cp midisco.dsk midisco-$(date +%Y%m%d).dsk
```

### ¿Funciona con discos de 80 pistas?

Actualmente iDSK20 soporta discos estándar de 40 pistas. El soporte para 80 pistas está en desarrollo.

### ¿Puedo contribuir al proyecto?

¡Sí! El proyecto es open source:
- GitHub: https://github.com/CPCReady/tools
- Reporta bugs
- Sugiere mejoras
- Envía pull requests

---


## 📝 Referencia Rápida de Comandos

### Sintaxis General

```bash
iDSK20 <comando> [disco.dsk] <argumentos> [opciones]
```

### Crear y Gestionar Discos

```bash
# Crear nuevo disco
iDSK20 new disco.dsk

# Listar contenido
iDSK20 cat disco.dsk

# Con variable de entorno
export DISC_DSK_FILE=disco.dsk
iDSK20 cat
```

### Añadir Archivos (Sintaxis CPC)

```bash
# ASCII/BASIC (por defecto)
iDSK20 save "archivo.bas"
iDSK20 save "archivo.bas",A

# Binary con direcciones
iDSK20 save "codigo.bin",B,8000,8010

# Raw sin cabecera
iDSK20 save "datos.dat",R

# Con atributos
iDSK20 save "config.cfg",A,P      # Protegido
iDSK20 save "system.bin",B,4000,4000,S  # Sistema
```

### Añadir Archivos (Sintaxis Tradicional)

```bash
# Con flags
iDSK20 save disco.dsk archivo.bas -t 0
iDSK20 save disco.dsk codigo.bin -t 1 -c 8000 -e 8010
iDSK20 save disco.dsk datos.dat -t 2

# Con atributos
iDSK20 save disco.dsk archivo.bas -t 0 -o  # Read-only
iDSK20 save disco.dsk archivo.bas -t 0 -s  # System
iDSK20 save disco.dsk archivo.bas -t 0 -f  # Force overwrite
```

### Extraer y Borrar

```bash
# Extraer archivo
iDSK20 get disco.dsk "archivo.bas"

# Borrar archivo
iDSK20 era disco.dsk "archivo.bas"
```

### Ver Contenido

```bash
# Ver archivo BASIC
iDSK20 list disco.dsk "programa.bas"

# Ver en hexadecimal
iDSK20 hex disco.dsk "datos.bin"

# Desensamblar
iDSK20 disasm disco.dsk "codigo.bin"
```

### Ejecutar

```bash
# Configurar RetroVirtualMachine
export RVM_PATH="/ruta/a/RetroVirtualMachine"

# Ejecutar programa
iDSK20 run disco.dsk "juego.bas"
```

---

## 🎨 Opciones de Formato

### Flags para el Comando save

| Flag | Descripción | Ejemplo |
|------|-------------|---------|
| `-t <tipo>` | Tipo de archivo (0=ASCII, 1=BINARY, 2=raw) | `-t 1` |
| `-c <addr>` | Dirección de carga (hex) | `-c 8000` |
| `-e <addr>` | Dirección de ejecución (hex) | `-e 8010` |
| `-f` | Forzar sobrescritura | `-f` |
| `-o` | Marcar como read-only | `-o` |
| `-s` | Marcar como system file | `-s` |
| `-u <num>` | Número de usuario (0-15) | `-u 1` |

### Direcciones Hexadecimales Comunes

| Dirección | Decimal | Uso Típico |
|-----------|---------|------------|
| `4000` | 16384 | Música, datos |
| `8000` | 32768 | Código general |
| `A000` | 40960 | Código alto |
| `C000` | 49152 | Pantalla |
| `0170` | 368 | BASIC (carga) |

---


## 🚀 Casos de Uso Avanzados

### Automatización con Scripts

#### Script para Crear Disco de Juego Completo

```bash
#!/bin/bash
# create-game-disk.sh

DISK="mygame.dsk"
export DISC_DSK_FILE=$DISK

echo "🎮 Creando disco de juego..."

# Crear disco
iDSK20 new

# Añadir archivos
echo "📝 Añadiendo archivos..."
iDSK20 save "loader.bas",A
iDSK20 save "main.bin",B,8000,8000
iDSK20 save "level1.dat",R
iDSK20 save "level2.dat",R
iDSK20 save "level3.dat",R
iDSK20 save "music.bin",B,4000,4000
iDSK20 save "sprites.bin",R

# Mostrar contenido
echo "📀 Contenido del disco:"
iDSK20 cat

echo "✅ Disco creado: $DISK"
```

Uso:
```bash
chmod +x create-game-disk.sh
./create-game-disk.sh
```

#### Script para Backup Automático

```bash
#!/bin/bash
# backup-disk.sh

DISK=$1
BACKUP_DIR="backups"
DATE=$(date +%Y%m%d-%H%M%S)

if [ -z "$DISK" ]; then
    echo "Uso: $0 <disco.dsk>"
    exit 1
fi

mkdir -p $BACKUP_DIR
cp "$DISK" "$BACKUP_DIR/${DISK%.dsk}-$DATE.dsk"
echo "✅ Backup creado: $BACKUP_DIR/${DISK%.dsk}-$DATE.dsk"
```

Uso:
```bash
./backup-disk.sh midisco.dsk
```

### Integración con Compiladores

#### Con Z88DK

```bash
#!/bin/bash
# compile-and-disk.sh

SOURCE="game.c"
OUTPUT="game.bin"
DISK="game.dsk"

echo "🔨 Compilando..."
z88dk-zcc +cpc -o $OUTPUT $SOURCE

if [ $? -eq 0 ]; then
    echo "📀 Creando disco..."
    export DISC_DSK_FILE=$DISK
    iDSK20 new
    iDSK20 save "loader.bas",A
    iDSK20 save "$OUTPUT",B,8000,8000
    echo "✅ Listo!"
else
    echo "❌ Error en compilación"
    exit 1
fi
```

#### Con SDCC

```bash
#!/bin/bash
# sdcc-to-disk.sh

sdcc -mz80 --code-loc 0x8000 --data-loc 0x9000 game.c
hex2bin game.ihx
export DISC_DSK_FILE=game.dsk
iDSK20 new
iDSK20 save "game.bin",B,8000,8000
```

### Conversión Masiva de Archivos

```bash
#!/bin/bash
# convert-all-bas.sh

DISK="programs.dsk"
export DISC_DSK_FILE=$DISK

echo "📀 Creando disco..."
iDSK20 new

echo "📝 Convirtiendo archivos BASIC..."
for file in *.bas; do
    if [ -f "$file" ]; then
        echo "  → $file"
        iDSK20 save "$file",A
    fi
done

echo "✅ Conversión completada"
iDSK20 cat
```

### Validación de Discos

```bash
#!/bin/bash
# validate-disk.sh

DISK=$1

if [ ! -f "$DISK" ]; then
    echo "❌ Disco no encontrado: $DISK"
    exit 1
fi

echo "🔍 Validando disco: $DISK"
export DISC_DSK_FILE=$DISK

# Verificar que se puede leer
if iDSK20 cat > /dev/null 2>&1; then
    echo "✅ Disco válido"
    
    # Mostrar estadísticas
    TOTAL=$(iDSK20 cat | grep -c "^[A-Z]")
    echo "📊 Archivos en disco: $TOTAL"
    
    # Mostrar espacio libre
    iDSK20 cat | grep "free"
else
    echo "❌ Disco corrupto o inválido"
    exit 1
fi
```

---


## 🎓 Tutorial Paso a Paso

### Tutorial 1: Mi Primer Programa en CPC

**Objetivo:** Crear un programa BASIC y ejecutarlo en el emulador.

#### Paso 1: Crear el programa BASIC

```bash
# Crear archivo con tu editor favorito
nano hola.bas
```

Contenido:
```basic
10 MODE 1
20 PRINT "HOLA MUNDO"
30 PRINT "MI PRIMER PROGRAMA"
40 FOR I=1 TO 10
50 PRINT "LINEA ";I
60 NEXT I
70 PRINT "FIN"
```

#### Paso 2: Crear el disco

```bash
# Configurar disco por defecto
export DISC_DSK_FILE=tutorial.dsk

# Crear disco nuevo
iDSK20 new
```

#### Paso 3: Añadir el programa

```bash
# Añadir con sintaxis CPC
iDSK20 save "hola.bas",A
```

#### Paso 4: Verificar

```bash
# Ver contenido del disco
iDSK20 cat

# Ver contenido del programa
iDSK20 list "hola.bas"
```

#### Paso 5: Ejecutar (opcional)

```bash
# Si tienes RetroVirtualMachine
export RVM_PATH="/ruta/a/RetroVirtualMachine"
iDSK20 run "hola.bas"
```

### Tutorial 2: Crear un Juego Simple

**Objetivo:** Crear un disco con loader y código binario.

#### Paso 1: Crear el loader BASIC

```bash
nano loader.bas
```

Contenido:
```basic
10 MODE 0
20 PRINT "CARGANDO JUEGO..."
30 LOAD "GAME.BIN",8000
40 CALL 8000
```

#### Paso 2: Crear código de ejemplo (o compilar el tuyo)

```bash
# Crear archivo binario de ejemplo
echo "Código del juego" > game.bin
```

#### Paso 3: Crear disco y añadir archivos

```bash
export DISC_DSK_FILE=juego.dsk
iDSK20 new
iDSK20 save "loader.bas",A
iDSK20 save "game.bin",B,8000,8000
```

#### Paso 4: Verificar

```bash
iDSK20 cat
```

### Tutorial 3: Gestionar Múltiples Niveles

**Objetivo:** Crear un disco con varios niveles de juego.

```bash
# Configurar disco
export DISC_DSK_FILE=adventure.dsk

# Crear disco
iDSK20 new

# Añadir loader
iDSK20 save "loader.bas",A

# Añadir código principal
iDSK20 save "main.bin",B,8000,8000

# Añadir niveles como datos raw
iDSK20 save "level1.dat",R
iDSK20 save "level2.dat",R
iDSK20 save "level3.dat",R
iDSK20 save "level4.dat",R

# Añadir recursos
iDSK20 save "sprites.bin",R
iDSK20 save "music.bin",B,4000,4000

# Ver resultado
iDSK20 cat
```

---


## 🔗 Recursos Adicionales

### Documentación

- **Manual completo**: `doc/README.md`
- **Documentación en inglés**: `doc/README_EN.md`
- **Guía de compilación**: `doc/BUILD.md`
- **Cross-compilation**: `doc/CROSS-COMPILATION.md`

### Comunidad

- **GitHub**: https://github.com/CPCReady/tools
- **Issues**: Reporta bugs y sugiere mejoras
- **Pull Requests**: Contribuye al proyecto

### Herramientas Relacionadas

- **RetroVirtualMachine**: Emulador moderno de CPC
- **WinAPE**: Emulador clásico para Windows
- **Z88DK**: Compilador C para Z80/CPC
- **SDCC**: Small Device C Compiler

### Aprender Más sobre CPC

- **CPCWiki**: https://www.cpcwiki.eu/
- **Amstrad CPC**: Documentación técnica
- **AMSDOS**: Sistema de archivos del CPC

---

## 📄 Licencia

iDSK20 es software libre bajo licencia MIT.

```
Copyright (c) 2025 Destroyer
Basado en iDSK por Sid, Colin Pitrat, y otros

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 👥 Créditos

### iDSK20 (2025)
- **Destroyer** - Modernización, sintaxis CPC, documentación

### iDSK Original
- **Jérôme Le Saux (Sid)** - Autor original
- **Colin Pitrat** - Contribuciones
- **Thomas Bernard (miniupnp)** - Contribuciones
- **Romain Giot (Krusty)** - Contribuciones
- **Adrien Destugues (PulkoMandy)** - Contribuciones

### Agradecimientos
- Comunidad de Amstrad CPC
- Desarrolladores de RetroVirtualMachine
- Todos los contribuidores del proyecto

---

## 📞 Soporte

### ¿Necesitas Ayuda?

1. **Lee este README** - La mayoría de preguntas están respondidas aquí
2. **Consulta la ayuda integrada** - `iDSK20 help`
3. **Revisa los ejemplos** - Sección de ejemplos prácticos
4. **Busca en Issues** - Puede que alguien ya haya tenido tu problema
5. **Crea un Issue** - Si no encuentras solución

### Reportar Bugs

Al reportar un bug, incluye:
- Versión de iDSK20 (`iDSK20 help` muestra la versión)
- Sistema operativo y versión
- Comando exacto que usaste
- Mensaje de error completo
- Pasos para reproducir el problema

### Sugerir Mejoras

¡Las sugerencias son bienvenidas! Abre un Issue con:
- Descripción clara de la mejora
- Caso de uso (¿para qué la necesitas?)
- Ejemplos de cómo funcionaría

---

## 🎉 ¡Gracias por Usar iDSK20!

Esperamos que iDSK20 te ayude en tus proyectos de Amstrad CPC. Si te gusta el proyecto:

- ⭐ Dale una estrella en GitHub
- 🐛 Reporta bugs que encuentres
- 💡 Sugiere mejoras
- 🤝 Contribuye con código
- 📢 Comparte con otros desarrolladores de CPC

**¡Feliz desarrollo para Amstrad CPC!** 🎮

---

**Versión del documento:** 1.0.0  
**Fecha:** 2 de enero de 2025  
**Autor:** Destroyer  
**Proyecto:** iDSK20 - Modern DSK Image Editor
