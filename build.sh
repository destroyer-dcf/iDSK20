#!/bin/bash

# Script de build limpio para iDSK20
# Autor: Destroyer 2025

set -e  # Salir si hay algún error

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_step() {
    echo -e "${BLUE}🔧 $1${NC}"
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

# Función para limpiar build anterior
clean_build() {
    print_step "Limpiando build anterior..."
    
    # Remover directorio build completo
    if [ -d "build" ]; then
        rm -rf build
        echo "  ✓ Directorio build/ eliminado"
    fi
    
    # Remover archivos de cmake del directorio raíz (si existen)
    rm -f CMakeCache.txt cmake_install.cmake Makefile
    rm -rf CMakeFiles/
    
    print_success "Build anterior limpiado"
}

# Función para crear build limpio
create_build() {
    print_step "Creando build limpio..."
    
    # Crear directorio build
    mkdir -p build
    cd build
    
    # Configurar con cmake (out-of-source build)
    print_step "Configurando con CMake..."
    cmake ..
    
    # Compilar
    print_step "Compilando..."
    make -j $(nproc 2>/dev/null || echo 4)
    
    cd ..
    print_success "Build completado exitosamente"
}

# Función para verificar el resultado
verify_build() {
    print_step "Verificando build..."
    
    if [ -f "build/iDSK20" ]; then
        print_success "Binario generado: build/iDSK20"
        echo "  Tamaño: $(ls -lh build/iDSK20 | awk '{print $5}')"
        echo "  Permisos: $(ls -l build/iDSK20 | awk '{print $1}')"
        
        # Probar que funciona
        print_step "Probando binario..."
        if ./build/iDSK20 help >/dev/null 2>&1; then
            print_success "Binario funciona correctamente"
        else
            print_error "Error: El binario no funciona correctamente"
            exit 1
        fi
    else
        print_error "Error: No se generó el binario build/iDSK20"
        exit 1
    fi
}

# Función principal
main() {
    echo -e "${BLUE}================================${NC}"
    echo -e "${BLUE}  BUILD iDSK20${NC}"
    echo -e "${BLUE}================================${NC}"
    echo
    
    clean_build
    echo
    create_build
    echo
    verify_build
    echo
    
    print_success "¡Build completado exitosamente!"
    echo -e "${YELLOW}Ejecuta: ./build/iDSK20 help${NC}"
}

# Ejecutar si se llama directamente
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi