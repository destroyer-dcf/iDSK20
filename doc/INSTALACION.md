# Instalación de iDSK20

[← Volver al índice](README.md)

---

## Instalación Local (Recomendada)

Puedes instalar desde el archivo compilado en la carpeta `build/` o descargan la version para tu sistema operativo desde **[Releases](https://github.com/destroyer.dcf/iDSK20/releases)** :

```bash
# En macOS/Linux
./build/iDSK20 help

# En Windows
.\build\Release\iDSK20.exe help
```

---

## Instalación Global

Para usar `iDSK20` desde cualquier carpeta, añádelo al PATH del sistema.

### macOS / Linux

#### Opción 1: PATH Temporal (Solo Sesión Actual)

```bash
# Añadir al PATH
export PATH=$PATH:$(pwd)/build

# Verificar
iDSK20 help
```

#### Opción 2: PATH Permanente

**Para Bash (~/.bashrc):**

```bash
# Editar archivo de configuración
nano ~/.bashrc

# Añadir al final (reemplaza /ruta/completa con tu ruta real):
export PATH=$PATH:/ruta/completa/a/tools/disc/build

# Guardar (Ctrl+O, Enter, Ctrl+X)

# Recargar configuración
source ~/.bashrc

# Verificar
iDSK20 help
```

**Para Zsh (~/.zshrc):**

```bash
# Editar archivo de configuración
nano ~/.zshrc

# Añadir al final:
export PATH=$PATH:/ruta/completa/a/tools/disc/build

# Guardar y recargar
source ~/.zshrc
```

**Para Fish (~/.config/fish/config.fish):**

```fish
# Editar archivo de configuración
nano ~/.config/fish/config.fish

# Añadir al final:
set -x PATH $PATH /ruta/completa/a/tools/disc/build

# Guardar y recargar
source ~/.config/fish/config.fish
```

#### Opción 3: Enlace Simbólico

```bash
# Crear enlace en /usr/local/bin
sudo ln -s $(pwd)/build/iDSK20 /usr/local/bin/iDSK20

# Verificar
iDSK20 help
```

### Windows

#### Opción 1: Variables de Entorno (GUI)

1. Presionar `Win + R` y escribir: `sysdm.cpl`
2. Ir a la pestaña "Opciones avanzadas"
3. Clic en "Variables de entorno"
4. En "Variables de usuario", seleccionar `Path` y clic en "Editar"
5. Clic en "Nuevo"
6. Añadir la ruta completa: `C:\ruta\a\tools\disc\build\Release`
7. Clic en "Aceptar" en todas las ventanas
8. Reiniciar la terminal
9. Verificar: `iDSK20 help`

#### Opción 2: PowerShell Profile

```powershell
# Editar perfil de PowerShell
notepad $PROFILE

# Si el archivo no existe, créalo:
New-Item -Path $PROFILE -Type File -Force

# Añadir al archivo:
$env:PATH += ";C:\ruta\a\tools\disc\build\Release"

# Guardar y recargar
. $PROFILE

# Verificar
iDSK20 help
```

---

### Configurar Variables de Entorno (Recomendado)

Para simplificar el uso diario:

```bash
# Configurar disco por defecto
export DISC_DSK_FILE=midisco.dsk

# Ahora puedes omitir el nombre del disco
iDSK20 save "archivo.bas",A
iDSK20 cat
iDSK20 get "archivo.bas"
```

Ver más en [Variables de Entorno](VARIABLES.md).

---

## Verificar la Instalación

### Comprobar que iDSK20 está en el PATH

```bash
# Verificar ubicación
which iDSK20  # macOS/Linux
where iDSK20  # Windows

# Verificar versión
iDSK20 help
```

### Probar Funcionalidad Básica

```bash
# Crear disco de prueba
iDSK20 new test.dsk

# Verificar que se creó
ls -la test.dsk  # macOS/Linux
dir test.dsk     # Windows

# Limpiar
rm test.dsk      # macOS/Linux
del test.dsk     # Windows
```

---

## Desinstalar iDSK20

### Instalación Local

```bash
# Simplemente eliminar el directorio
rm -rf iDSK20
```

### Instalación Global

#### macOS / Linux

```bash
# Si usaste enlace simbólico
sudo rm /usr/local/bin/iDSK20

# Si modificaste PATH, editar ~/.bashrc o ~/.zshrc
nano ~/.bashrc
# Eliminar la línea que añade iDSK20 al PATH
source ~/.bashrc
```

#### Windows

1. Abrir "Variables de entorno" (`Win + R` → `sysdm.cpl`)
2. Editar la variable `Path`
3. Eliminar la entrada de iDSK20
4. Aceptar y reiniciar la terminal

---

## Próximos Pasos

1. **[Comandos](COMANDOS.md)** - Comandos básicos
2. **[Configurar Variables de Entorno](VARIABLES.md)** - Simplifica tu workflow
3. **[Ejemplos](EJEMPLOS.md)** - Casos de uso

---

[← Volver al índice](README.md) | [Siguiente: Comandos →](COMANDOS.md)
