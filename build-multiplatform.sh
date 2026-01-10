#!/bin/bash

# Script de compilación multiplataforma para iDSK20
# Autor: Destroyer 2025
# Genera binarios para Windows, macOS, Linux (x64 y ARM)

set -e

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m' # No Color

print_header() {
    echo -e "\n${BLUE}================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}================================${NC}"
}

print_step() {
    echo -e "${CYAN}🔧 $1${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_platform() {
    echo -e "${MAGENTA}🎯 Compilando para: $1${NC}"
}

# Variables
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/dist"
VERSION=$(grep "#define VERSION" src/Main.h | sed 's/.*Version: \([0-9.]*\).*/\1/' || echo "1.0.0")

# Función para limpiar builds anteriores
clean_builds() {
    print_step "Limpiando builds anteriores..."
    rm -rf "$BUILD_DIR"
    # Borrar directorios de build específicos (evitar borrar scripts build-*.sh)
    rm -rf build build-windows build-linux build-macos build-native
    rm -rf build-linux-x64 build-linux-arm64 build-macos-x64 build-macos-arm64
    rm -f CMakeCache.txt cmake_install.cmake Makefile
    rm -rf CMakeFiles/
    print_success "Limpieza completada"
}

# Función para limpiar builds temporales (mantener solo dist/)
clean_temp_builds() {
    print_step "Limpiando builds temporales..."
    # Mantener solo dist/ y build/ (nativo), limpiar el resto
    rm -rf build-windows build-linux build-macos build-native
    rm -rf build-linux-x64 build-linux-arm64 build-macos-x64 build-macos-arm64
    rm -f CMakeCache.txt cmake_install.cmake Makefile
    rm -rf CMakeFiles/
    print_success "Builds temporales limpiados (mantenido: dist/ y build/)"
}

# Función para crear estructura de directorios
create_structure() {
    print_step "Creando estructura de directorios..."
    mkdir -p "$BUILD_DIR"/{windows-x64,macos-x64,macos-arm64,linux-x64,linux-arm64}
    print_success "Estructura creada"
}

# Función para compilar nativo (actual plataforma)
build_native() {
    print_platform "Plataforma nativa ($(uname -s)-$(uname -m))"
    
    ./build.sh
    
    # Detectar plataforma y copiar binario
    case "$(uname -s)" in
        Darwin)
            if [[ "$(uname -m)" == "arm64" ]]; then
                cp build/iDSK20 "$BUILD_DIR/macos-arm64/iDSK20"
                print_success "Binario nativo copiado a macos-arm64/"
            else
                cp build/iDSK20 "$BUILD_DIR/macos-x64/iDSK20"
                print_success "Binario nativo copiado a macos-x64/"
            fi
            ;;
        Linux)
            if [[ "$(uname -m)" == "aarch64" ]]; then
                cp build/iDSK20 "$BUILD_DIR/linux-arm64/iDSK20"
                print_success "Binario nativo copiado a linux-arm64/"
            else
                cp build/iDSK20 "$BUILD_DIR/linux-x64/iDSK20"
                print_success "Binario nativo copiado a linux-x64/"
            fi
            ;;
        MINGW*|CYGWIN*|MSYS*)
            cp build/iDSK20.exe "$BUILD_DIR/windows-x64/iDSK20.exe"
            print_success "Binario nativo copiado a windows-x64/"
            ;;
    esac
}

# Función para compilar usando herramientas locales (sin Docker)
build_local_cross() {
    print_step "Intentando cross-compilation local..."
    
    # Verificar herramientas disponibles
    local has_mingw=false
    
    if command -v x86_64-w64-mingw32-gcc &> /dev/null; then
        has_mingw=true
        print_success "MinGW encontrado"
    fi
    
    # Compilar para Windows con MinGW (si está disponible)
    if $has_mingw; then
        print_platform "Windows x64 (MinGW local)"
        
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
        
        mkdir -p build-windows
        cd build-windows
        cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-toolchain.cmake ..
        make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
        if [[ -f iDSK20.exe ]]; then
            cp iDSK20.exe "$BUILD_DIR/windows-x64/iDSK20.exe"
            print_success "Binario Windows generado (MinGW local)"
        fi
        cd ..
    fi
    
    # Cross-compilation para macOS x64 (desde macOS ARM64)
    if [[ "$(uname -s)" == "Darwin" ]] && [[ "$(uname -m)" == "arm64" ]]; then
        print_platform "macOS x64 (clang cross-compilation)"
        
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
            cp iDSK20 "$BUILD_DIR/macos-x64/iDSK20"
            print_success "Binario macOS x64 generado (clang cross-compilation)"
        fi
        cd ..
    fi
}

# Función para compilar con Docker (cross-compilation)
build_with_docker() {
    local platform=$1
    local dockerfile=$2
    local output_dir=$3
    local binary_name=$4
    
    print_platform "$platform"
    
    if ! command -v docker &> /dev/null; then
        print_warning "Docker no encontrado, saltando $platform"
        return
    fi
    
    # Crear Dockerfile temporal si no existe
    if [[ ! -f "$dockerfile" ]]; then
        print_warning "Dockerfile $dockerfile no encontrado, saltando $platform"
        return
    fi
    
    # Construir imagen Docker
    print_step "Construyendo imagen Docker para $platform..."
    if ! docker build -f "$dockerfile" -t "disc-builder-$platform" . --quiet; then
        print_error "Error construyendo imagen Docker para $platform"
        return
    fi
    
    # Compilar en contenedor
    print_step "Compilando en contenedor..."
    if docker run --rm -v "$PWD:/workspace" "disc-builder-$platform"; then
        if [[ -f "$BUILD_DIR/$output_dir/$binary_name" ]]; then
            print_success "Binario $platform generado"
        else
            print_error "Error: binario $platform no encontrado después de compilación"
        fi
    else
        print_error "Error ejecutando contenedor para $platform"
    fi
}

# Función para crear paquetes
create_packages() {
    print_step "Creando paquetes de distribución..."
    
    cd "$BUILD_DIR"
    
    for dir in */; do
        if [[ -d "$dir" ]]; then
            platform=${dir%/}
            print_step "Empaquetando $platform..."
            
            # Crear directorio temporal
            temp_dir="iDSK20-$VERSION-$platform"
            mkdir -p "$temp_dir"
            
            # Copiar binario
            if [[ -f "$dir/iDSK20" ]]; then
                cp "$dir/iDSK20" "$temp_dir/"
            elif [[ -f "$dir/iDSK20.exe" ]]; then
                cp "$dir/iDSK20.exe" "$temp_dir/"
            fi
            
            # Copiar documentación
            cp ../README.md "$temp_dir/" 2>/dev/null || true
            cp ../AUTHORS "$temp_dir/" 2>/dev/null || true
            cp ../LICENSE.md "$temp_dir/" 2>/dev/null || true
            
            # Crear archivo ZIP
            if command -v zip &> /dev/null; then
                zip -r "iDSK20-$VERSION-$platform.zip" "$temp_dir"
                print_success "Paquete ZIP creado: iDSK20-$VERSION-$platform.zip"
            fi
            
            # Crear tarball para Unix
            if [[ "$platform" != "windows-x64" ]]; then
                tar -czf "iDSK20-$VERSION-$platform.tar.gz" "$temp_dir"
                print_success "Tarball creado: iDSK20-$VERSION-$platform.tar.gz"
            fi
            
            # Limpiar directorio temporal
            rm -rf "$temp_dir"
        fi
    done
    
    cd ..
}

# Función para mostrar resumen
show_summary() {
    print_header "RESUMEN DE COMPILACIÓN"
    
    echo -e "${CYAN}Versión:${NC} $VERSION"
    echo -e "${CYAN}Directorio de salida:${NC} $BUILD_DIR"
    echo
    
    print_step "Binarios generados:"
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
    
    echo
    print_step "Paquetes creados:"
    for package in "$BUILD_DIR"/*.{zip,tar.gz}; do
        if [[ -f "$package" ]]; then
            size=$(ls -lh "$package" | awk '{print $5}')
            echo "  📦 $(basename "$package") ($size)"
        fi
    done 2>/dev/null || echo "  (No se crearon paquetes)"
}

# Función principal
main() {
    print_header "COMPILACIÓN MULTIPLATAFORMA DE iDSK20"
    echo -e "${CYAN}Versión: $VERSION${NC}"
    echo -e "${CYAN}Plataforma actual: $(uname -s)-$(uname -m)${NC}"
    echo
    
    clean_builds
    create_structure
    
    # Compilar nativo primero
    build_native
    
    # Intentar cross-compilation local
    build_local_cross
    
    # Compilar con Docker (si está disponible)
    if command -v docker &> /dev/null; then
        print_step "Docker encontrado, intentando cross-compilation..."
        
        # Solo intentar plataformas que no se compilaron nativamente
        case "$(uname -s)" in
            Darwin)
                # Desde macOS, compilar Linux
                build_with_docker "linux-x64" "Dockerfile.linux-x64" "linux-x64" "iDSK20"
                build_with_docker "linux-arm64" "Dockerfile.linux-arm64" "linux-arm64" "iDSK20"
                ;;
            Linux)
                # Desde Linux, compilar para otras arquitecturas si no es nativa
                if [[ "$(uname -m)" != "aarch64" ]]; then
                    build_with_docker "linux-arm64" "Dockerfile.linux-arm64" "linux-arm64" "iDSK20"
                fi
                if [[ "$(uname -m)" != "x86_64" ]]; then
                    build_with_docker "linux-x64" "Dockerfile.linux-x64" "linux-x64" "iDSK20"
                fi
                ;;
        esac
    else
        print_warning "Docker no encontrado, solo compilación nativa disponible"
    fi
    
    create_packages
    clean_temp_builds
    show_summary
    
    echo
    print_success "¡Compilación multiplataforma completada!"
    echo -e "${YELLOW}Los binarios están en: $BUILD_DIR${NC}"
}

# Ejecutar si se llama directamente
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi