# 🔍 Solución de Problemas

[← Volver al índice](README.md)

---

## Introducción

Esta sección contiene soluciones a problemas comunes que puedes encontrar al usar iDSK20.

---

## 📋 Índice de Problemas

- [Problemas de Instalación](#problemas-de-instalación)
- [Problemas de Ejecución](#problemas-de-ejecución)
- [Problemas con Archivos](#problemas-con-archivos)
- [Problemas con Variables de Entorno](#problemas-con-variables-de-entorno)
- [Problemas con RetroVirtualMachine](#problemas-con-retrovirtualmachine)
- [Problemas de Sintaxis](#problemas-de-sintaxis)
- [Problemas de Compilación](#problemas-de-compilación)

---

## Problemas de Instalación

### Problema: "cmake: command not found"

**Causa:** CMake no está instalado en tu sistema.

**Solución:**

```bash
# macOS
brew install cmake

# Ubuntu/Debian
sudo apt-get update
sudo apt-get install cmake build-essential

# Fedora/RHEL
sudo dnf install cmake gcc-c++

# Windows
# Descargar desde: https://cmake.org/download/
```

### Problema: "No C++ compiler found"

**Causa:** No tienes un compilador C++ instalado.

**Solución:**

```bash
# macOS
xcode-select --install

# Ubuntu/Debian
sudo apt-get install build-essential

# Fedora/RHEL
sudo dnf install gcc-c++

# Windows
# Instalar Visual Studio o MinGW
```

### Problema: "Permission denied: ./build.sh"

**Causa:** El script no tiene permisos de ejecución.

**Solución:**

```bash
# Dar permisos de ejecución
chmod +x build.sh

# Ejecutar
./build.sh
```

### Problema: Compilación falla con errores de C++11

**Causa:** El compilador no soporta C++11 o no está configurado.

**Solución:**

Ya está configurado en `CMakeLists.txt`, pero si persiste:

```bash
# Limpiar y recompilar
rm -rf build
./build.sh

# O manualmente
mkdir build
cd build
cmake .. -DCMAKE_CXX_STANDARD=11
make
```

---

## Problemas de Ejecución

### Problema: "Command not found: iDSK20"

**Causa:** El programa no está en el PATH o no se ha compilado.

**Solución:**

```bash
# Opción 1: Usar ruta completa
./build/iDSK20 help

# Opción 2: Añadir al PATH (temporal)
export PATH=$PATH:$(pwd)/build
iDSK20 help

# Opción 3: Añadir al PATH (permanente)
# En ~/.bashrc o ~/.zshrc
echo 'export PATH=$PATH:/ruta/completa/a/tools/disc/build' >> ~/.bashrc
source ~/.bashrc

# Opción 4: Instalar globalmente
sudo cp build/iDSK20 /usr/local/bin/
```

### Problema: "Segmentation fault"

**Causa:** Error interno del programa o archivo corrupto.

**Solución:**

```bash
# 1. Verificar que el archivo DSK existe y es válido
ls -lh disco.dsk

# 2. Intentar crear un disco nuevo
iDSK20 new test.dsk

# 3. Si persiste, recompilar
rm -rf build
./build.sh

# 4. Reportar el bug en GitHub con:
# - Comando exacto que usaste
# - Mensaje de error completo
# - Sistema operativo y versión
```

### Problema: Los colores no se ven bien

**Causa:** Tu terminal no soporta colores ANSI o tiene configuración incorrecta.

**Solución:**

```bash
# Opción 1: Desactivar colores con variable de entorno
export NO_COLOR=1
iDSK20 help

# Opción 2: Desactivar colores con flag
iDSK20 --no-color help

# Opción 3: Cambiar terminal
# Usar una terminal moderna como:
# - iTerm2 (macOS)
# - Windows Terminal (Windows)
# - GNOME Terminal (Linux)
# - Konsole (Linux)
```

### Problema: "Invalid command"

**Causa:** Comando mal escrito o no existe.

**Solución:**

```bash
# Ver lista de comandos disponibles
iDSK20 help

# Comandos válidos:
# new, cat, save, get, era, list, hex, disasm, dams, run, help

# Verificar ortografía
iDSK20 cat disco.dsk    # ✅ Correcto
iDSK20 list disco.dsk   # ❌ Incorrecto (falta nombre de archivo)
```

---

## Problemas con Archivos

### Problema: "Error reading file"

**Causa:** El archivo no existe o la ruta es incorrecta.

**Solución:**

```bash
# Verificar que el archivo existe
ls -la archivo.bas

# Usar ruta completa
iDSK20 save disco.dsk "/ruta/completa/archivo.bas",A

# Verificar permisos
chmod 644 archivo.bas

# Verificar que no hay caracteres especiales en el nombre
# Evitar: ñ, á, é, í, ó, ú, espacios sin comillas
```

### Problema: "Disk image not found"

**Causa:** El disco DSK no existe o DISC_DSK_FILE no está configurado.

**Solución:**

```bash
# Opción 1: Crear el disco primero
iDSK20 new midisco.dsk

# Opción 2: Verificar que existe
ls -la midisco.dsk

# Opción 3: Configurar variable de entorno
export DISC_DSK_FILE=midisco.dsk
iDSK20 cat

# Opción 4: Usar ruta completa
iDSK20 cat /ruta/completa/midisco.dsk
```

### Problema: "File already exists"

**Causa:** Ya existe un archivo con ese nombre en el disco.

**Solución:**

```bash
# Opción 1: Borrar el archivo primero
iDSK20 era disco.dsk "archivo.bas"
iDSK20 save disco.dsk "archivo.bas",A

# Opción 2: Usar flag -f (force) con sintaxis tradicional
iDSK20 save disco.dsk archivo.bas -t 0 -f

# Opción 3: Usar otro nombre
iDSK20 save disco.dsk "archivo2.bas",A
```

### Problema: "Disk full"

**Causa:** No hay espacio suficiente en el disco.

**Solución:**

```bash
# Ver espacio disponible
iDSK20 cat disco.dsk | grep "free"

# Opciones:
# 1. Borrar archivos innecesarios
iDSK20 era disco.dsk "archivo-viejo.bas"

# 2. Crear un nuevo disco
iDSK20 new disco2.dsk

# 3. Comprimir archivos antes de añadir
# (usar herramientas externas de compresión)
```

### Problema: El archivo no se ejecuta en el CPC

**Causa:** Tipo de archivo incorrecto o direcciones erróneas.

**Solución:**

```bash
# Para BASIC: usar tipo A
iDSK20 save "programa.bas",A

# Para binarios: verificar direcciones
# Direcciones comunes:
# - 8000 (32768) - Código general
# - 4000 (16384) - Música/datos
# - C000 (49152) - Pantalla

iDSK20 save "codigo.bin",B,8000,8000

# Verificar en el CPC:
# LOAD "programa.bas"
# RUN

# Para binarios:
# LOAD "codigo.bin",8000
# CALL 8000
```

### Problema: "Invalid file type"

**Causa:** Tipo de archivo no válido o mal especificado.

**Solución:**

```bash
# Tipos válidos:
# - A (ASCII/BASIC)
# - B (Binary)
# - R (Raw)

# ✅ Correcto
iDSK20 save "archivo.bas",A
iDSK20 save "codigo.bin",B,8000,8010
iDSK20 save "datos.dat",R

# ❌ Incorrecto
iDSK20 save "archivo.bas",T  # T no existe
iDSK20 save "archivo.bas",0  # Usar A, no 0 (sintaxis CPC)
```

---

## Problemas con Variables de Entorno

### Problema: Variable DISC_DSK_FILE no funciona

**Causa:** Variable no configurada correctamente o no exportada.

**Solución:**

```bash
# ❌ Incorrecto (sin export)
DISC_DSK_FILE=disco.dsk
iDSK20 cat  # No funciona

# ✅ Correcto (con export)
export DISC_DSK_FILE=disco.dsk
iDSK20 cat  # Funciona

# Verificar que está configurada
echo $DISC_DSK_FILE

# Ver todas las variables
env | grep DISC
```

### Problema: Variable no persiste entre sesiones

**Causa:** Variable no añadida al archivo de configuración del shell.

**Solución:**

```bash
# Bash
echo 'export DISC_DSK_FILE=~/cpc/midisco.dsk' >> ~/.bashrc
source ~/.bashrc

# Zsh
echo 'export DISC_DSK_FILE=~/cpc/midisco.dsk' >> ~/.zshrc
source ~/.zshrc

# Fish
echo 'set -x DISC_DSK_FILE ~/cpc/midisco.dsk' >> ~/.config/fish/config.fish
source ~/.config/fish/config.fish
```

### Problema: RVM_PATH no funciona

**Causa:** Ruta incorrecta o RetroVirtualMachine no instalado.

**Solución:**

```bash
# Verificar que RetroVirtualMachine está instalado
# macOS
ls -la "/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"

# Linux
which retrovirtualmachine

# Configurar correctamente
# macOS
export RVM_PATH="/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"

# Linux
export RVM_PATH="/usr/bin/retrovirtualmachine"

# Verificar
echo $RVM_PATH
"$RVM_PATH" --version
```

---

## Problemas con RetroVirtualMachine

### Problema: "RetroVirtualMachine not found"

**Causa:** RVM_PATH no configurado o RetroVirtualMachine no instalado.

**Solución:**

```bash
# 1. Descargar RetroVirtualMachine
# https://www.retrovirtualmachine.org/blog/future

# 2. Instalar

# 3. Configurar RVM_PATH
# macOS
export RVM_PATH="/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"

# Linux
export RVM_PATH="/usr/bin/retrovirtualmachine"

# 4. Verificar
"$RVM_PATH" --version
```

### Problema: "RVM version not supported"

**Causa:** Versión de RetroVirtualMachine no compatible.

**Solución:**

```bash
# Descargar versión 2.0 o superior
# https://www.retrovirtualmachine.org/blog/future

# Verificar versión
"$RVM_PATH" --version

# Debe ser 2.0 o superior con opciones de desarrollo activadas
```

### Problema: El programa no se ejecuta en RVM

**Causa:** Archivo no compatible o modelo de CPC incorrecto.

**Solución:**

```bash
# Verificar que el archivo existe en el disco
export DISC_DSK_FILE=disco.dsk
iDSK20 cat

# Probar con diferentes modelos
export CPC_MODEL=6128
iDSK20 run "programa.bas"

export CPC_MODEL=464
iDSK20 run "programa.bas"

# Verificar tipo de archivo
# BASIC debe ser tipo A
# Binarios deben ser tipo B con direcciones correctas
```

---

## Problemas de Sintaxis

### Problema: "Invalid CPC syntax"

**Causa:** Error en la sintaxis CPC del comando save.

**Solución:**

```bash
# ❌ Incorrecto (sin comillas)
iDSK20 save archivo.bas,A

# ✅ Correcto (con comillas)
iDSK20 save "archivo.bas",A

# ❌ Incorrecto (falta tipo antes de direcciones)
iDSK20 save "loader.bin",8000,8010

# ✅ Correcto (tipo B para binarios)
iDSK20 save "loader.bin",B,8000,8010

# ❌ Incorrecto (espacios sin comillas)
iDSK20 save mi archivo.bas,A

# ✅ Correcto (con comillas)
iDSK20 save "mi archivo.bas",A
```

### Problema: "Invalid address format"

**Causa:** Formato de dirección hexadecimal incorrecto.

**Solución:**

```bash
# ❌ Incorrecto (con prefijo &)
iDSK20 save "code.bin",B,&8000,&8010

# ✅ Correcto (sin prefijo)
iDSK20 save "code.bin",B,8000,8010

# ❌ Incorrecto (decimal)
iDSK20 save "code.bin",B,32768,32768

# ✅ Correcto (hexadecimal)
iDSK20 save "code.bin",B,8000,8000

# Conversión decimal a hexadecimal:
# 32768 = 8000
# 16384 = 4000
# 49152 = C000
```

### Problema: "Missing required parameter"

**Causa:** Falta un parámetro obligatorio.

**Solución:**

```bash
# ❌ Incorrecto (falta nombre de archivo)
iDSK20 save disco.dsk

# ✅ Correcto
iDSK20 save disco.dsk "archivo.bas",A

# ❌ Incorrecto (tipo B sin direcciones)
iDSK20 save "code.bin",B

# ✅ Correcto (con dirección de carga)
iDSK20 save "code.bin",B,8000

# ✅ Correcto (con carga y ejecución)
iDSK20 save "code.bin",B,8000,8010
```

---

## Problemas de Compilación

### Problema: "fatal error: iostream: No such file or directory"

**Causa:** Compilador C++ no instalado correctamente.

**Solución:**

```bash
# macOS
xcode-select --install

# Ubuntu/Debian
sudo apt-get install g++

# Fedora/RHEL
sudo dnf install gcc-c++
```

### Problema: "undefined reference to..."

**Causa:** Problema de enlazado o biblioteca faltante.

**Solución:**

```bash
# Limpiar y recompilar
rm -rf build
./build.sh

# Si persiste, verificar CMakeLists.txt
cat CMakeLists.txt
```

### Problema: Warnings sobre sprintf deprecated

**Causa:** Warnings normales en macOS/Linux modernos.

**Solución:**

```bash
# Los warnings son normales y no afectan la funcionalidad
# El programa se compila correctamente

# Si quieres suprimirlos (no recomendado):
# Editar CMakeLists.txt y añadir:
# add_compile_options(-Wno-deprecated-declarations)
```

---

## Problemas Específicos por Plataforma

### macOS

#### Problema: "xcrun: error: invalid active developer path"

**Solución:**

```bash
xcode-select --install
```

#### Problema: "Operation not permitted"

**Solución:**

```bash
# Dar permisos en System Preferences > Security & Privacy
# O usar sudo (no recomendado)
```

### Linux

#### Problema: "Permission denied" al ejecutar

**Solución:**

```bash
chmod +x build/iDSK20
```

#### Problema: Falta libstdc++

**Solución:**

```bash
# Ubuntu/Debian
sudo apt-get install libstdc++6

# Fedora/RHEL
sudo dnf install libstdc++
```

### Windows

#### Problema: "MSVCR120.dll not found"

**Solución:**

```bash
# Instalar Visual C++ Redistributable
# https://www.microsoft.com/en-us/download/details.aspx?id=40784
```

#### Problema: Compilación falla con MinGW

**Solución:**

```bash
# Usar Visual Studio en su lugar
# O instalar MinGW-w64 completo
```

---

## Debugging Avanzado

### Habilitar Modo Verbose

```bash
# Añadir flag de debug (si está implementado)
iDSK20 --verbose cat disco.dsk

# O usar strace/ltrace en Linux
strace iDSK20 cat disco.dsk

# O dtruss en macOS
sudo dtruss iDSK20 cat disco.dsk
```

### Verificar Integridad del Disco

```bash
# Listar contenido
iDSK20 cat disco.dsk

# Ver en hexadecimal
hexdump -C disco.dsk | head -n 20

# Verificar tamaño
ls -lh disco.dsk
# Debe ser ~178 KB para disco estándar
```

### Reportar Bugs

Al reportar un bug, incluye:

1. **Versión de iDSK20**
   ```bash
   iDSK20 help | head -n 1
   ```

2. **Sistema operativo**
   ```bash
   uname -a
   ```

3. **Comando exacto**
   ```bash
   iDSK20 save disco.dsk "archivo.bas",A
   ```

4. **Mensaje de error completo**
   ```
   Error: ...
   ```

5. **Pasos para reproducir**
   - Paso 1: ...
   - Paso 2: ...
   - Paso 3: ...

---

## Preguntas Frecuentes (FAQ)

### ¿Por qué mi archivo BASIC no se ve bien en el CPC?

Asegúrate de usar tipo A para archivos BASIC:
```bash
iDSK20 save "programa.bas",A
```

### ¿Cómo sé qué dirección usar para un binario?

Direcciones comunes:
- `8000` - Código general
- `4000` - Música/datos
- `C000` - Pantalla
- `0170` - BASIC

### ¿Puedo usar iDSK20 en scripts?

Sí, iDSK20 está diseñado para automatización. Ver [Ejemplos Prácticos](EJEMPLOS.md).

### ¿Cómo convierto archivos de texto a formato CPC?

iDSK20 lo hace automáticamente al usar tipo A:
```bash
iDSK20 save "texto.txt",A
```

### ¿Funciona con discos de 80 pistas?

Actualmente solo soporta discos estándar de 40 pistas.

---

## Recursos Adicionales

- **[Comandos](COMANDOS.md)** - Referencia completa
- **[Ejemplos Prácticos](EJEMPLOS.md)** - Scripts y casos de uso
- **[Variables de Entorno](VARIABLES.md)** - Configuración
- **GitHub Issues** - https://github.com/destroyer.dcf/iDSK20/issues

---

## ¿No Encuentras tu Problema?

Si tu problema no está listado aquí:

1. Busca en GitHub Issues
2. Consulta la documentación completa
3. Crea un nuevo Issue con toda la información

---

[← Volver al índice](README.md)

