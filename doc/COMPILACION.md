# Compilación de iDSK20

[← Volver al índice](README.md)

---

## Requisitos Previos

**Nota**
> **Si no quieres compilar te puedes descarga la version para tu sistema operativo desde [Releases](https://github.com/destroyer.dcf/iDSK20/releases)**

Antes de compilar iDSK20, necesitas tener instalado:

### Herramientas Necesarias

- **CMake** (versión 3.10 o superior)
- **Compilador C++** compatible con C++11:
  - GCC 4.8+ (Linux)
  - Clang 3.3+ (macOS)
  - MSVC 2015+ (Windows)
- **Git** (opcional, para clonar el repositorio)

### Instalación de Requisitos

#### macOS

```bash
# Instalar Xcode Command Line Tools
xcode-select --install

# Instalar CMake con Homebrew
brew install cmake
```

#### Linux (Ubuntu/Debian)

```bash
# Instalar herramientas de compilación
sudo apt-get update
sudo apt-get install build-essential cmake git
```

#### Linux (Fedora/RHEL)

```bash
# Instalar herramientas de compilación
sudo dnf install gcc-c++ cmake git
```

#### Windows

1. **Instalar Visual Studio:**
   - Descargar [Visual Studio](https://visualstudio.microsoft.com/)
   - Durante la instalación, seleccionar "Desarrollo para escritorio con C++"

2. **Instalar CMake:**
   - Descargar desde [cmake.org](https://cmake.org/download/)
   - Durante la instalación, seleccionar "Add CMake to system PATH"

3. **Instalar Git (opcional):**
   - Descargar desde [git-scm.com](https://git-scm.com/)

---

## Descargar el Código Fuente

### Opción 1: Clonar con Git (Recomendado)

```bash
# Clonar el repositorio
git clone https://github.com/destroyer.dcf/iDSK20.git

# Entrar al directorio
cd iDSK20
```

### Opción 2: Descargar ZIP

1. Ir a https://github.com/destroyer.dcf/iDSK20
2. Clic en "Code" → "Download ZIP"
3. Extraer el archivo ZIP
4. Abrir terminal en la carpeta `iDSK20`

---

## Compilar iDSK20

### macOS y Linux

```bash
# Dar permisos de ejecución al script
chmod +x build.sh

# Compilar
./build.sh
```

El script `build.sh` realiza automáticamente:
1. Limpia compilaciones anteriores
2. Crea el directorio `build/`
3. Configura el proyecto con CMake
4. Compila el código
5. Verifica que el binario funciona

**Salida esperada:**
```
================================
  BUILD iDSK20
================================

🔧 Limpiando build anterior...
✅ Build anterior limpiado

🔧 Creando build limpio...
🔧 Configurando con CMake...
🔧 Compilando...
✅ Build completado exitosamente

🔧 Verificando build...
✅ Binario generado: build/iDSK20
  Tamaño: 353K
  Permisos: -rwxr-xr-x
✅ Binario funciona correctamente

✅ ¡Build completado exitosamente!
Ejecuta: ./build/iDSK20 help
```

### Windows

#### Opción 1: Con PowerShell/CMD

```bash
# Abrir PowerShell o CMD en la carpeta disc
.\build.sh
```

#### Opción 2: Compilación Manual

```bash
# Crear directorio de compilación
mkdir build
cd build

# Configurar con CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Compilar
cmake --build . --config Release

# El ejecutable estará en: build\Release\iDSK20.exe
```

---

## Verificar la Compilación

### Probar el Binario

```bash
# En macOS/Linux
./build/iDSK20 help

# En Windows
.\build\Release\iDSK20.exe help
```

Si ves el menú de ayuda con colores, ¡la compilación fue exitosa! 🎉

### Verificar la Versión

```bash
./build/iDSK20 help | head -10
```

Deberías ver:
```
▗ ▛▀▖▞▀▖▌ ▌▞▀▖▞▀▖
▄ ▌ ▌▚▄ ▙▞  ▗▘▌▞▌
▐ ▌ ▌▖ ▌▌▝▖▗▘ ▛ ▌
▀▘▀▀ ▝▀ ▘ ▘▀▀▘▝▀ 
2.0.0
by Destroyer 2025
```

---

## Compilación Avanzada

### Compilación con Opciones Personalizadas

```bash
# Crear directorio de compilación
mkdir build
cd build

# Configurar con opciones personalizadas
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_STANDARD=11

# Compilar con múltiples núcleos
make -j$(nproc)
```

### Compilación en Modo Debug

```bash
mkdir build-debug
cd build-debug

cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

# El binario tendrá símbolos de debug
./iDSK20 help
```

### Cross-Compilación

Para compilar para otras plataformas, consulta los archivos:
- `Dockerfile.linux-arm64` - Para Linux ARM64
- `Dockerfile.linux-x64` - Para Linux x64
- `cmake/mingw-toolchain.cmake` - Para Windows desde Linux

---

## Solución de Problemas de Compilación

### Error: "CMake not found"

**Solución:**
```bash
# macOS
brew install cmake

# Linux
sudo apt-get install cmake

# Windows: Descargar desde cmake.org
```

### Error: "No suitable C++ compiler found"

**Solución:**
```bash
# macOS
xcode-select --install

# Linux
sudo apt-get install build-essential

# Windows: Instalar Visual Studio
```

### Error: "C++11 features not supported"

**Causa:** Compilador muy antiguo.

**Solución:**
```bash
# Actualizar el compilador
# Linux
sudo apt-get update
sudo apt-get upgrade gcc g++

# macOS
brew upgrade gcc
```

### Warnings sobre `sprintf` deprecated

**Nota:** Estos son warnings, no errores. El programa compila correctamente.

**Explicación:** macOS y algunas versiones de Linux muestran warnings sobre funciones deprecated. Esto no afecta la funcionalidad.

---

## Limpiar la Compilación

### Limpiar Build

```bash
# Eliminar directorio de compilación
rm -rf build

# O usar el script
./build.sh
```

### Limpiar Todo

```bash
# Eliminar todos los archivos generados
rm -rf build build-debug dist
```

---

## Próximos Pasos

Una vez compilado exitosamente:

1. **[Instalar iDSK20](INSTALACION.md)** - Configura iDSK20 para uso diario
2. **[Aprender los Comandos](COMANDOS.md)** - Domina los comandos básicos

---

[← Volver al índice](README.md) | [Siguiente: Instalación →](INSTALACION.md)
