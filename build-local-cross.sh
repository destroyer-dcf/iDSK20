#!/bin/bash

# Script de cross-compilation local simplificado
# Usa herramientas instaladas localmente (sin Docker)

set -e

# Colores
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

print_step() {
    echo -e "${CYAN}🔧 $1${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

# Variables
BUILD_DIR="dist"
VERSION="1.0.0"

# Limpiar y crear estructura
print_step "Preparando directorios..."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"/{windows-x64,macos-x64,macos-arm64,linux-x64,linux-arm64}

# Compilación nativa
print_step "Compilando para plataforma nativa..."
./build.sh

# Copiar binario nativo
case "$(uname -s)" in
    Darwin)
        if [[ "$(uname -m)" == "arm64" ]]; then
            cp build/iDSK20 "$BUILD_DIR/macos-arm64/"
            print_success "macOS ARM64 (nativo)"
        else
            cp build/iDSK20 "$BUILD_DIR/macos-x64/"
            print_success "macOS x64 (nativo)"
        fi
        ;;
    Linux)
        if [[ "$(uname -m)" == "aarch64" ]]; then
            cp build/iDSK20 "$BUILD_DIR/linux-arm64/"
            print_success "Linux ARM64 (nativo)"
        else
            cp build/iDSK20 "$BUILD_DIR/linux-x64/"
            print_success "Linux x64 (nativo)"
        fi
        ;;
esac

# Cross-compilation para Windows (MinGW)
if command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    print_step "Cross-compilando para Windows (MinGW)..."
    
    # Crear toolchain
    mkdir -p cmake
    cat > cmake/mingw-toolchain.cmake << 'EOF'
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
EOF
    
    # Compilar
    mkdir -p build-windows
    cd build-windows
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-toolchain.cmake ..
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
    if [[ -f iDSK20.exe ]]; then
        cp iDSK20.exe "../$BUILD_DIR/windows-x64/"
        print_success "Windows x64 (MinGW)"
    fi
    cd ..
else
    print_warning "MinGW no encontrado, instala con: brew install mingw-w64 (macOS) o apt install mingw-w64 (Linux)"
fi

# Cross-compilation para macOS (desde Linux)
if [[ "$(uname -s)" == "Linux" ]] && command -v osxcross-clang &> /dev/null; then
    print_step "Cross-compilando para macOS..."
    
    mkdir -p build-macos
    cd build-macos
    CC=osxcross-clang CXX=osxcross-clang++ cmake ..
    make -j$(nproc)
    
    if [[ -f iDSK20 ]]; then
        cp iDSK20 "../$BUILD_DIR/macos-x64/"
        print_success "macOS x64 (osxcross)"
    fi
    cd ..
else
    if [[ "$(uname -s)" == "Linux" ]]; then
        print_warning "osxcross no encontrado para compilar macOS desde Linux"
    fi
fi

# Cross-compilation para macOS x64 (desde macOS ARM64)
if [[ "$(uname -s)" == "Darwin" ]] && [[ "$(uname -m)" == "arm64" ]]; then
    print_step "Cross-compilando para macOS x64 (desde ARM64)..."
    
    # Crear toolchain si no existe
    mkdir -p cmake
    if [[ ! -f cmake/macos-x64-toolchain.cmake ]]; then
        cat > cmake/macos-x64-toolchain.cmake << 'EOF'
# Toolchain para cross-compilation de macOS ARM64 a macOS x64
set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Usar clang con target específico
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Flags para cross-compilation a x86_64
set(CMAKE_C_FLAGS_INIT "-target x86_64-apple-macos10.15")
set(CMAKE_CXX_FLAGS_INIT "-target x86_64-apple-macos10.15")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-target x86_64-apple-macos10.15")

# Configurar el SDK
set(CMAKE_OSX_ARCHITECTURES x86_64)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

# Configurar paths de búsqueda
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
EOF
    fi
    
    # Compilar
    mkdir -p build-macos-x64
    cd build-macos-x64
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/macos-x64-toolchain.cmake ..
    make -j$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
    if [[ -f iDSK20 ]]; then
        cp iDSK20 "../$BUILD_DIR/macos-x64/"
        print_success "macOS x64 (clang cross-compilation)"
    fi
    cd ..
fi

# Mostrar resumen
echo
print_step "Resumen de binarios generados:"
for dir in "$BUILD_DIR"/*/; do
    if [[ -d "$dir" ]]; then
        platform=${dir%/}
        platform=${platform##*/}
        
        if [[ -f "$dir/iDSK20" ]]; then
            size=$(ls -lh "$dir/iDSK20" | awk '{print $5}')
            echo "  ✅ $platform: iDSK20 ($size)"
        elif [[ -f "$dir/iDSK20.exe" ]]; then
            size=$(ls -lh "$dir/iDSK20.exe" | awk '{print $5}')
            echo "  ✅ $platform: iDSK20.exe ($size)"
        else
            echo "  ❌ $platform: No generado"
        fi
    fi
done

print_success "Cross-compilation local completada!"
echo -e "${YELLOW}Los binarios están en: $BUILD_DIR/${NC}"

# Limpiar builds temporales
echo
print_step "Limpiando builds temporales..."
rm -rf build-windows build-linux build-macos build-native
rm -rf build-linux-x64 build-linux-arm64 build-macos-x64 build-macos-arm64
rm -f CMakeCache.txt cmake_install.cmake Makefile
rm -rf CMakeFiles/
print_success "Builds temporales limpiados (mantenido: dist/ y build/)"