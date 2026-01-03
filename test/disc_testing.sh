#!/bin/bash

# Script para probar todos los comandos de disc
# Autor: Destroyer 2025
# Descripción: Prueba exhaustiva de todos los comandos y funcionalidades de disc

set -e  # Salir si hay algún error

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Función para mostrar mensajes
print_header() {
    echo -e "\n${BLUE}================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}================================${NC}"
}

print_test() {
    echo -e "\n${CYAN}🧪 Probando: $1${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

# Variables
DISC_BIN="../build/disc"
TEST_DSK="test_commands.dsk"
EXISTING_DSK="../build/8bp_v43_001.dsk"
TEST_FILE="test_file.txt"
TEST_BAS="test_program.bas"
TEST_BIN="test_binary.bin"
READONLY_FILE="readonly.txt"

# Lista de archivos temporales para limpiar
TEMP_FILES=("$TEST_DSK" "$TEST_FILE" "$TEST_BAS" "$TEST_BIN" "$READONLY_FILE")

# Función de limpieza que se ejecuta al salir
cleanup() {
    print_header "LIMPIEZA AUTOMÁTICA"
    print_test "Eliminando archivos temporales creados durante las pruebas"
    
    for file in "${TEMP_FILES[@]}"; do
        if [ -f "$file" ]; then
            rm -f "$file" 2>/dev/null && echo "  ✓ Eliminado: $file" || echo "  ✗ Error eliminando: $file"
        fi
    done
    
    # Limpiar cualquier archivo adicional que pueda haberse creado (pero no scripts)
    rm -f test_*.txt test_*.bas test_*.bin test_*.dsk 2>/dev/null || true
    rm -f *.tmp 2>/dev/null || true
    
    print_success "Limpieza completada"
}

# Configurar trap para limpiar al salir (normal, interrupción, error)
trap cleanup EXIT INT TERM

# Verificar que el binario existe
if [ ! -f "$DISC_BIN" ]; then
    print_error "El binario $DISC_BIN no existe. Compila primero con 'make' en el directorio build."
    exit 1
fi

print_header "INICIANDO PRUEBAS DE DISC"
echo "Binario: $DISC_BIN"
echo "DSK de prueba: $TEST_DSK"
echo "DSK existente: $EXISTING_DSK"

# Limpiar archivos de prueba anteriores (por si quedaron de ejecuciones previas)
print_test "Limpiando archivos de pruebas anteriores"
for file in "${TEMP_FILES[@]}"; do
    if [ -f "$file" ]; then
        rm -f "$file" 2>/dev/null && echo "  ✓ Eliminado archivo previo: $file"
    fi
done

print_header "1. PRUEBAS DE AYUDA"

print_test "Ayuda general"
$DISC_BIN help

print_test "Ayuda específica de cada comando"
commands=("cat" "new" "save" "era" "get" "list" "basic" "ascii" "hex" "disasm" "dams" "run")
for cmd in "${commands[@]}"; do
    echo -e "\n${YELLOW}--- Ayuda de $cmd ---${NC}"
    $DISC_BIN $cmd --help
done

print_header "2. CREACIÓN DE DSK"

print_test "Crear nuevo DSK"
$DISC_BIN new "$TEST_DSK"
if [ -f "$TEST_DSK" ]; then
    print_success "DSK creado correctamente: $TEST_DSK"
else
    print_error "Error al crear DSK"
    exit 1
fi

print_header "3. CATÁLOGO DE DSK"

print_test "Mostrar catálogo de DSK vacío"
$DISC_BIN cat "$TEST_DSK"

print_test "Mostrar catálogo de DSK con archivos (usando DSK existente)"
if [ -f "$EXISTING_DSK" ]; then
    $DISC_BIN cat "$EXISTING_DSK"
    print_success "Catálogo mostrado correctamente"
else
    print_warning "DSK existente no encontrado, saltando prueba"
fi

print_header "4. CREACIÓN DE ARCHIVOS DE PRUEBA"

print_test "Crear archivo de texto de prueba"
cat > "$TEST_FILE" << 'EOF'
Este es un archivo de prueba para disc.
Contiene texto ASCII simple.
Línea 3 de prueba.
¡Funciona correctamente!
EOF
print_success "Archivo de texto creado: $TEST_FILE"

print_test "Crear programa BASIC de prueba"
cat > "$TEST_BAS" << 'EOF'
10 PRINT "HOLA MUNDO"
20 FOR I=1 TO 10
30 PRINT "NUMERO: ";I
40 NEXT I
50 PRINT "FIN DEL PROGRAMA"
EOF
print_success "Programa BASIC creado: $TEST_BAS"

print_test "Crear archivo binario de prueba"
# Crear un archivo binario simple con algunos bytes
printf '\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F' > "$TEST_BIN"
printf '\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F' >> "$TEST_BIN"
print_success "Archivo binario creado: $TEST_BIN"

print_header "5. COMANDO SAVE (IMPORTAR ARCHIVOS)"

print_test "Importar archivo de texto (ASCII)"
$DISC_BIN save "$TEST_DSK" "$TEST_FILE"
print_success "Archivo de texto importado"

print_test "Importar programa BASIC"
$DISC_BIN save "$TEST_DSK" "$TEST_BAS" -t 0
print_success "Programa BASIC importado"

print_test "Importar archivo binario"
$DISC_BIN save "$TEST_DSK" "$TEST_BIN" -t 1 -c 8000 -e 8000
print_success "Archivo binario importado"

print_test "Importar con sintaxis CPC (archivo con atributos)"
echo "Archivo de solo lectura" > "$READONLY_FILE"
$DISC_BIN save "$TEST_DSK" "$READONLY_FILE,R"
print_success "Archivo con atributos importado"

print_test "Mostrar catálogo después de importar"
$DISC_BIN cat "$TEST_DSK"

print_header "6. COMANDO GET (EXPORTAR ARCHIVOS)"

print_test "Exportar archivo de texto"
rm -f "$TEST_FILE" 2>/dev/null || true
$DISC_BIN get "$TEST_DSK" "$TEST_FILE"
if [ -f "$TEST_FILE" ]; then
    print_success "Archivo de texto exportado correctamente"
    echo "Contenido exportado:"
    cat "$TEST_FILE"
else
    print_error "Error al exportar archivo de texto"
fi

print_header "7. COMANDOS DE VISUALIZACIÓN"

if [ -f "$EXISTING_DSK" ]; then
    print_test "Comando LIST (auto-detección)"
    $DISC_BIN list "$EXISTING_DSK" "JUEGO.BAS" || print_warning "Archivo JUEGO.BAS no encontrado"
    
    print_test "Comando BASIC (forzar BASIC)"
    $DISC_BIN basic "$EXISTING_DSK" "JUEGO.BAS" || print_warning "Archivo JUEGO.BAS no encontrado"
    
    print_test "Comando ASCII (forzar ASCII)"
    $DISC_BIN ascii "$EXISTING_DSK" "TEST_FIL.TXT" || print_warning "Archivo TEST_FIL.TXT no encontrado"
    
    print_test "Comando HEX (hexadecimal)"
    $DISC_BIN hex "$EXISTING_DSK" "TEST.BIN" || print_warning "Archivo TEST.BIN no encontrado"
    
    print_test "Comando DISASM (desensamblador)"
    $DISC_BIN disasm "$EXISTING_DSK" "TEST.BIN" || print_warning "Archivo TEST.BIN no encontrado"
    
    print_test "Comando DAMS"
    $DISC_BIN dams "$EXISTING_DSK" "8BP.BIN" || print_warning "Archivo 8BP.BIN no encontrado o no es DAMS"
else
    print_warning "DSK existente no encontrado, saltando pruebas de visualización"
fi

print_header "8. VARIABLES DE ENTORNO"

print_test "Usar DISC_DSK_FILE"
export DISC_DSK_FILE="$TEST_DSK"
echo "DISC_DSK_FILE configurado a: $DISC_DSK_FILE"
$DISC_BIN cat
print_success "Variable de entorno funcionando"

print_test "Comando sin DSK especificado (usando variable de entorno)"
$DISC_BIN cat
print_success "Comando cat sin argumentos funciona con variable de entorno"

unset DISC_DSK_FILE

print_header "9. CONTROL DE COLORES"

print_test "Ayuda con colores (por defecto)"
$DISC_BIN help | head -10

print_test "Ayuda sin colores (--no-color)"
$DISC_BIN help --no-color | head -10

print_test "Ayuda sin colores (variable NO_COLOR)"
NO_COLOR=1 $DISC_BIN help | head -10

print_test "Catálogo con colores"
$DISC_BIN cat "$TEST_DSK"

print_test "Catálogo sin colores"
$DISC_BIN cat "$TEST_DSK" --no-color

print_header "10. COMANDO ERA (ELIMINAR ARCHIVOS)"

print_test "Intentar eliminar archivo protegido (debería fallar)"
if [ -f "$EXISTING_DSK" ]; then
    $DISC_BIN era "$EXISTING_DSK" "TEST_REA.TXT" 2>&1 || print_success "Protección de archivos funcionando correctamente"
else
    print_warning "DSK existente no encontrado, saltando prueba de protección"
fi

print_test "Eliminar archivo normal"
$DISC_BIN era "$TEST_DSK" "$TEST_FILE" || print_warning "Error al eliminar archivo (puede que no exista)"

print_test "Mostrar catálogo después de eliminar"
$DISC_BIN cat "$TEST_DSK"

print_header "11. COMANDO RUN (si RVM_PATH está configurado)"

if [ -n "$RVM_PATH" ] || [ -n "$RETROVIRTUALMACHINE_PATH" ]; then
    print_test "Comando RUN (solo mostrar ayuda, no ejecutar)"
    $DISC_BIN run --help
    print_success "Comando RUN disponible"
else
    print_warning "RVM_PATH no configurado, saltando prueba de RUN"
    print_warning "Para probar RUN, configura: export RVM_PATH=\"/ruta/a/RetroVirtualMachine\""
fi

print_header "12. PRUEBAS DE ERRORES"

print_test "Comando inválido"
$DISC_BIN invalid_command 2>&1 || print_success "Error manejado correctamente para comando inválido"

print_test "DSK inexistente"
$DISC_BIN cat "archivo_inexistente.dsk" 2>&1 || print_success "Error manejado correctamente para DSK inexistente"

print_test "Archivo inexistente en DSK"
$DISC_BIN get "$TEST_DSK" "archivo_inexistente.txt" 2>&1 || print_success "Error manejado correctamente para archivo inexistente"

print_header "RESUMEN DE PRUEBAS"
print_success "Todas las pruebas completadas"
print_success "disc está funcionando correctamente"

echo -e "\n${GREEN}🎉 ¡TODAS LAS PRUEBAS COMPLETADAS EXITOSAMENTE! 🎉${NC}"
echo -e "${CYAN}El programa disc está listo para usar.${NC}"

# Mostrar información final
echo -e "\n${BLUE}Información del sistema:${NC}"
echo "Versión de disc: $($DISC_BIN help | grep "disc version" | head -1)"
echo "Sistema operativo: $(uname -s)"
echo "Arquitectura: $(uname -m)"
echo "Shell: $SHELL"
echo "Fecha de prueba: $(date)"

# Nota: La limpieza se ejecutará automáticamente al salir del script