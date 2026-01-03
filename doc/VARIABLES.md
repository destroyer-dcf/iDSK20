# Variables de Entorno

[← Volver al índice](README.md)

---

## ¿Qué son las Variables de Entorno?

Las variables de entorno te permiten configurar iDSK20 una vez y olvidarte de repetir parámetros en cada comando.

**Ventajas:**
- ✅ Evita escribir el nombre del disco en cada comando
- ✅ Simplifica scripts y automatización
- ✅ Reduce errores de tipeo
- ✅ Workflow más limpio y eficiente

---

## Variables Disponibles

| Variable | Descripción | Requerida | Por Defecto |
|----------|-------------|-----------|-------------|
| `DISC_DSK_FILE` | Disco DSK por defecto | No | - |
| `RVM_PATH` | Ruta a RetroVirtualMachine | Solo para `run` | - |
| `CPC_MODEL` | Modelo de CPC (464/664/6128) | No | 6128 |
| `NO_COLOR` | Desactivar colores ANSI | No | - |

---

## `DISC_DSK_FILE`

**Descripción:** Define el disco DSK por defecto para todos los comandos (excepto `new`).

**Tipo:** String (ruta al archivo .dsk)

### Configuración

```bash
# macOS / Linux
export DISC_DSK_FILE=midisco.dsk
export DISC_DSK_FILE=/ruta/completa/midisco.dsk

# Windows (PowerShell)
$env:DISC_DSK_FILE="midisco.dsk"
$env:DISC_DSK_FILE="C:\cpc\midisco.dsk"

# Windows (CMD)
set DISC_DSK_FILE=midisco.dsk
set DISC_DSK_FILE=C:\cpc\midisco.dsk
```

### Ejemplo de Uso

```bash
# Sin variable de entorno (repetitivo)
iDSK20 save midisco.dsk "archivo1.bas",A
iDSK20 save midisco.dsk "archivo2.bin",B,8000,8010
iDSK20 cat midisco.dsk
iDSK20 get midisco.dsk "archivo1.bas"

# Con variable de entorno (más limpio)
export DISC_DSK_FILE=midisco.dsk
iDSK20 save "archivo1.bas",A
iDSK20 save "archivo2.bin",B,8000,8010
iDSK20 cat
iDSK20 get "archivo1.bas"
```

### Script de Ejemplo

```bash
#!/bin/bash
# Configurar disco por defecto
export DISC_DSK_FILE=proyecto.dsk

# Crear disco
iDSK20 new $DISC_DSK_FILE

# Añadir archivos sin repetir el nombre del disco
iDSK20 save "loader.bas",A
iDSK20 save "main.bin",B,8000,8000
iDSK20 save "level1.dat",R

# Ver contenido
iDSK20 cat
```

---

## `RVM_PATH`

**Descripción:** Ruta al ejecutable de RetroVirtualMachine (requerido para el comando `run`).

**Tipo:** String (ruta completa al ejecutable)

### Configuración

```bash
# macOS
export RVM_PATH="/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"

# Linux
export RVM_PATH="/usr/bin/retrovirtualmachine"
export RVM_PATH="/opt/RetroVirtualMachine/rvm"

# Windows (PowerShell)
$env:RVM_PATH="C:\Program Files\RetroVirtualMachine\RetroVirtualMachine.exe"

# Windows (CMD)
set RVM_PATH="C:\Program Files\RetroVirtualMachine\RetroVirtualMachine.exe"
```

### Ejemplo de Uso

```bash
# Configurar RetroVirtualMachine
export RVM_PATH="/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"
export DISC_DSK_FILE=juego.dsk

# Ejecutar programa
iDSK20 run "juego.bas"
```

### Verificar Instalación

```bash
# Verificar que el ejecutable existe
ls -la "$RVM_PATH"  # macOS/Linux
dir "$env:RVM_PATH"  # Windows PowerShell

# Probar ejecución
"$RVM_PATH" --version
```

---

## `CPC_MODEL`

**Descripción:** Modelo de Amstrad CPC a emular al ejecutar programas.

**Tipo:** String (número de modelo)

**Valores posibles:**
- `464` - Amstrad CPC 464 (64 KB RAM, cassette)
- `664` - Amstrad CPC 664 (64 KB RAM, disco)
- `6128` - Amstrad CPC 6128 (128 KB RAM, disco) **[Por defecto]**

### Configuración

```bash
# macOS / Linux
export CPC_MODEL=464
export CPC_MODEL=664
export CPC_MODEL=6128

# Windows (PowerShell)
$env:CPC_MODEL="464"

# Windows (CMD)
set CPC_MODEL=464
```

### Ejemplo de Uso

```bash
# Probar en CPC 464 (64 KB)
export CPC_MODEL=464
iDSK20 run "programa.bas"

# Probar en CPC 6128 (128 KB)
export CPC_MODEL=6128
iDSK20 run "programa.bas"

# Cambiar modelo temporalmente
CPC_MODEL=464 iDSK20 run "test.bas"
```

---

## `NO_COLOR`

**Descripción:** Desactiva los colores ANSI en la salida de iDSK20.

**Tipo:** Boolean (cualquier valor activa la desactivación)

### Configuración

```bash
# macOS / Linux
export NO_COLOR=1
export NO_COLOR=true
export NO_COLOR=yes

# Windows (PowerShell)
$env:NO_COLOR="1"

# Windows (CMD)
set NO_COLOR=1
```

### Ejemplo de Uso

```bash
# Desactivar colores permanentemente
export NO_COLOR=1
iDSK20 help
iDSK20 cat

# Desactivar colores temporalmente
NO_COLOR=1 iDSK20 help

# Guardar salida sin colores
export NO_COLOR=1
iDSK20 cat > contenido.txt
```

---

## Configuración Permanente

### macOS / Linux

#### Bash (~/.bashrc)

```bash
# Editar archivo de configuración
nano ~/.bashrc

# Añadir al final del archivo:
export RVM_PATH="/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"
export CPC_MODEL=6128

# Guardar (Ctrl+O, Enter, Ctrl+X)

# Recargar configuración
source ~/.bashrc
```

#### Zsh (~/.zshrc)

```bash
# Editar archivo de configuración
nano ~/.zshrc

# Añadir al final del archivo:
export RVM_PATH="/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"
export CPC_MODEL=6128

# Guardar y recargar
source ~/.zshrc
```

#### Fish (~/.config/fish/config.fish)

```fish
# Editar archivo de configuración
nano ~/.config/fish/config.fish

# Añadir al final del archivo:
set -x RVM_PATH "/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2"
set -x CPC_MODEL 6128

# Guardar y recargar
source ~/.config/fish/config.fish
```

### Windows

#### Variables de Usuario (GUI)

1. Presionar `Win + R` y escribir: `sysdm.cpl`
2. Ir a la pestaña "Opciones avanzadas"
3. Clic en "Variables de entorno"
4. En "Variables de usuario", clic en "Nueva"
5. Añadir cada variable:
   - Nombre: `RVM_PATH`, Valor: `C:\Program Files\RetroVirtualMachine\RetroVirtualMachine.exe`
   - Nombre: `CPC_MODEL`, Valor: `6128`
6. Clic en "Aceptar"
7. Reiniciar la terminal

#### PowerShell Profile

```powershell
# Editar perfil de PowerShell
notepad $PROFILE

# Si el archivo no existe, créalo:
New-Item -Path $PROFILE -Type File -Force

# Añadir al archivo:
$env:DISC_DSK_FILE="C:\cpc\midisco.dsk"
$env:RVM_PATH="C:\Program Files\RetroVirtualMachine\RetroVirtualMachine.exe"
$env:CPC_MODEL="6128"

# Guardar y recargar
. $PROFILE
```

---

## Verificar Variables Configuradas

### macOS / Linux

```bash
# Ver todas las variables de iDSK20
env | grep -E "DISC|RVM|CPC|NO_COLOR"

# Ver variable específica
echo $DISC_DSK_FILE
echo $RVM_PATH
echo $CPC_MODEL
```

### Windows (PowerShell)

```powershell
# Ver todas las variables
Get-ChildItem Env: | Where-Object { $_.Name -match "DISC|RVM|CPC|NO_COLOR" }

# Ver variable específica
echo $env:DISC_DSK_FILE
echo $env:RVM_PATH
echo $env:CPC_MODEL
```

### Windows (CMD)

```cmd
# Ver todas las variables
set | findstr /I "DISC RVM CPC NO_COLOR"

# Ver variable específica
echo %DISC_DSK_FILE%
echo %RVM_PATH%
echo %CPC_MODEL%
```

---

[← Volver al índice](README.md) | [Siguiente: Tipos de Archivo →](TIPOS.md)
