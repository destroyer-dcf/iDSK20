# Ejemplos Prácticos

[← Volver al índice](README.md)

---

## Introducción

Esta sección contiene ejemplos prácticos y casos de uso reales para trabajar con iDSK20. Desde workflows básicos hasta automatización avanzada.

---

## 📋 Índice de Ejemplos

- [Workflow Básico](#workflow-básico)
- [Desarrollo de Juegos](#desarrollo-de-juegos)
- [Scripts de Automatización](#scripts-de-automatización)
- [Integración con Compiladores](#integración-con-compiladores)
- [Gestión de Proyectos](#gestión-de-proyectos)
- [Conversión Masiva](#conversión-masiva)
- [Backup y Restauración](#backup-y-restauración)

---

## Workflow Básico

### Ejemplo 1: Mi Primer Programa

Crear un programa BASIC simple y ejecutarlo.

```bash
# 1. Crear el programa BASIC
cat > hola.bas << 'EOF'
10 MODE 1
20 PRINT "HOLA MUNDO"
30 PRINT "MI PRIMER PROGRAMA"
40 END
EOF

# 2. Configurar disco por defecto
export DISC_DSK_FILE=tutorial.dsk

# 3. Crear disco nuevo
iDSK20 new

# 4. Añadir el programa
iDSK20 save "hola.bas",A

# 5. Verificar contenido
iDSK20 cat

# 6. Ver el programa
iDSK20 list "hola.bas"
```

### Ejemplo 2: Trabajar con Múltiples Archivos

```bash
# Configurar disco
export DISC_DSK_FILE=proyecto.dsk

# Crear disco
iDSK20 new

# Añadir varios archivos BASIC
iDSK20 save "menu.bas",A
iDSK20 save "juego.bas",A
iDSK20 save "creditos.bas",A

# Añadir datos
iDSK20 save "config.dat",R
iDSK20 save "niveles.dat",R

# Ver contenido
iDSK20 cat
```

### Ejemplo 3: Extraer y Modificar

```bash
export DISC_DSK_FILE=midisco.dsk

# Extraer archivo
iDSK20 get "programa.bas"

# Modificar localmente
nano programa.bas

# Borrar versión antigua
iDSK20 era "programa.bas"

# Añadir versión nueva
iDSK20 save "programa.bas",A

# Verificar
iDSK20 list "programa.bas"
```

---

## Desarrollo de Juegos

### Ejemplo 4: Estructura de Juego Completo

```bash
#!/bin/bash
# create-game.sh

DISK="mygame.dsk"
export DISC_DSK_FILE=$DISK

echo "🎮 Creando disco de juego..."

# Crear disco
iDSK20 new

# Loader BASIC
cat > loader.bas << 'EOF'
10 MODE 0
20 PRINT "CARGANDO..."
30 LOAD "MAIN.BIN",8000
40 CALL 8000
EOF

iDSK20 save "loader.bas",A

# Código principal (ejemplo)
iDSK20 save "main.bin",B,8000,8000

# Niveles
iDSK20 save "level1.dat",R
iDSK20 save "level2.dat",R
iDSK20 save "level3.dat",R

# Recursos
iDSK20 save "sprites.bin",R
iDSK20 save "music.bin",B,4000,4000
iDSK20 save "screen.scr",R

# Mostrar resultado
echo "📀 Contenido del disco:"
iDSK20 cat

echo "✅ Disco creado: $DISK"
```

### Ejemplo 5: Juego con Pantalla de Carga

```bash
export DISC_DSK_FILE=adventure.dsk

# Crear disco
iDSK20 new

# Loader con pantalla
cat > loader.bas << 'EOF'
10 MODE 0
20 LOAD "SCREEN.SCR",C000
30 PRINT "CARGANDO AVENTURA..."
40 LOAD "GAME.BIN",8000
50 CALL 8000
EOF

iDSK20 save "loader.bas",A
iDSK20 save "screen.scr",R
iDSK20 save "game.bin",B,8000,8000

# Ver resultado
iDSK20 cat
```

### Ejemplo 6: Juego Multi-Disco

```bash
#!/bin/bash
# create-multi-disk.sh

# Disco 1: Programa principal
export DISC_DSK_FILE=game-disk1.dsk
iDSK20 new
iDSK20 save "loader.bas",A
iDSK20 save "main.bin",B,8000,8000
iDSK20 save "level1.dat",R
iDSK20 save "level2.dat",R

# Disco 2: Niveles adicionales
export DISC_DSK_FILE=game-disk2.dsk
iDSK20 new
iDSK20 save "level3.dat",R
iDSK20 save "level4.dat",R
iDSK20 save "level5.dat",R
iDSK20 save "level6.dat",R

# Disco 3: Recursos
export DISC_DSK_FILE=game-disk3.dsk
iDSK20 new
iDSK20 save "music1.bin",B,4000,4000
iDSK20 save "music2.bin",B,4000,4000
iDSK20 save "sprites.bin",R
iDSK20 save "fonts.bin",R

echo "✅ 3 discos creados"
```

---

## Scripts de Automatización

### Ejemplo 7: Backup Automático

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

if [ ! -f "$DISK" ]; then
    echo "❌ Disco no encontrado: $DISK"
    exit 1
fi

# Crear directorio de backups
mkdir -p $BACKUP_DIR

# Copiar disco
BACKUP_FILE="$BACKUP_DIR/${DISK%.dsk}-$DATE.dsk"
cp "$DISK" "$BACKUP_FILE"

echo "✅ Backup creado: $BACKUP_FILE"

# Listar backups
echo ""
echo "📦 Backups disponibles:"
ls -lh $BACKUP_DIR/*.dsk
```

Uso:
```bash
chmod +x backup-disk.sh
./backup-disk.sh midisco.dsk
```

### Ejemplo 8: Conversión Masiva de Archivos BASIC

```bash
#!/bin/bash
# convert-all-bas.sh

DISK="programs.dsk"
SOURCE_DIR="basic-files"

export DISC_DSK_FILE=$DISK

echo "📀 Creando disco: $DISK"
iDSK20 new

echo "📝 Convirtiendo archivos BASIC..."
COUNT=0

for file in "$SOURCE_DIR"/*.bas; do
    if [ -f "$file" ]; then
        BASENAME=$(basename "$file")
        echo "  → $BASENAME"
        iDSK20 save "$file",A
        COUNT=$((COUNT + 1))
    fi
done

echo ""
echo "✅ $COUNT archivos convertidos"
echo ""
echo "📀 Contenido del disco:"
iDSK20 cat
```

### Ejemplo 9: Validación de Discos

```bash
#!/bin/bash
# validate-disk.sh

DISK=$1

if [ -z "$DISK" ]; then
    echo "Uso: $0 <disco.dsk>"
    exit 1
fi

if [ ! -f "$DISK" ]; then
    echo "❌ Disco no encontrado: $DISK"
    exit 1
fi

echo "🔍 Validando disco: $DISK"
export DISC_DSK_FILE=$DISK

# Verificar que se puede leer
if iDSK20 cat > /dev/null 2>&1; then
    echo "✅ Disco válido"
    
    # Estadísticas
    TOTAL=$(iDSK20 cat | grep -v "^DSK" | grep -v "^---" | grep -v "free" | wc -l)
    echo "📊 Archivos en disco: $TOTAL"
    
    # Espacio libre
    echo ""
    iDSK20 cat | grep "free"
    
    # Listar archivos
    echo ""
    echo "📄 Archivos:"
    iDSK20 cat
else
    echo "❌ Disco corrupto o inválido"
    exit 1
fi
```

### Ejemplo 10: Extraer Todos los Archivos

```bash
#!/bin/bash
# extract-all.sh

DISK=$1
OUTPUT_DIR="extracted"

if [ -z "$DISK" ]; then
    echo "Uso: $0 <disco.dsk>"
    exit 1
fi

export DISC_DSK_FILE=$DISK

# Crear directorio de salida
mkdir -p "$OUTPUT_DIR"
cd "$OUTPUT_DIR"

echo "📦 Extrayendo archivos de: $DISK"

# Obtener lista de archivos
FILES=$(iDSK20 cat | grep -v "^DSK" | grep -v "^---" | grep -v "free" | awk '{print $1}')

COUNT=0
for file in $FILES; do
    echo "  → $file"
    iDSK20 get "$file"
    COUNT=$((COUNT + 1))
done

cd ..

echo ""
echo "✅ $COUNT archivos extraídos en: $OUTPUT_DIR"
ls -lh "$OUTPUT_DIR"
```

---

## Integración con Compiladores

### Ejemplo 11: Compilar con Z88DK

```bash
#!/bin/bash
# compile-z88dk.sh

SOURCE="game.c"
OUTPUT="game.bin"
DISK="game.dsk"
LOAD_ADDR="8000"
EXEC_ADDR="8000"

echo "🔨 Compilando con Z88DK..."
z88dk-zcc +cpc -create-app -o ${OUTPUT%.bin} $SOURCE

if [ $? -eq 0 ]; then
    echo "📀 Creando disco..."
    export DISC_DSK_FILE=$DISK
    iDSK20 new
    
    # Loader BASIC
    cat > loader.bas << EOF
10 MODE 0
20 LOAD "$OUTPUT",$LOAD_ADDR
30 CALL $EXEC_ADDR
EOF
    
    iDSK20 save "loader.bas",A
    iDSK20 save "$OUTPUT",B,$LOAD_ADDR,$EXEC_ADDR
    
    echo "✅ Compilación exitosa"
    iDSK20 cat
else
    echo "❌ Error en compilación"
    exit 1
fi
```

### Ejemplo 12: Compilar con SDCC

```bash
#!/bin/bash
# compile-sdcc.sh

SOURCE="game.c"
OUTPUT="game.bin"
DISK="game.dsk"

echo "🔨 Compilando con SDCC..."
sdcc -mz80 --code-loc 0x8000 --data-loc 0x9000 $SOURCE

if [ $? -eq 0 ]; then
    # Convertir IHX a BIN
    hex2bin game.ihx
    
    echo "📀 Creando disco..."
    export DISC_DSK_FILE=$DISK
    iDSK20 new
    iDSK20 save "$OUTPUT",B,8000,8000
    
    echo "✅ Compilación exitosa"
    iDSK20 cat
else
    echo "❌ Error en compilación"
    exit 1
fi
```

### Ejemplo 13: Workflow Completo de Desarrollo

```bash
#!/bin/bash
# dev-workflow.sh

PROJECT="mygame"
DISK="${PROJECT}.dsk"

echo "🚀 Workflow de desarrollo: $PROJECT"

# 1. Compilar código
echo "🔨 Compilando..."
z88dk-zcc +cpc -create-app -o main main.c

if [ $? -ne 0 ]; then
    echo "❌ Error en compilación"
    exit 1
fi

# 2. Crear disco
echo "📀 Creando disco..."
export DISC_DSK_FILE=$DISK
iDSK20 new

# 3. Añadir archivos
echo "📝 Añadiendo archivos..."
iDSK20 save "loader.bas",A
iDSK20 save "main.bin",B,8000,8000

# Añadir recursos
for file in data/*.dat; do
    if [ -f "$file" ]; then
        iDSK20 save "$file",R
    fi
done

# 4. Verificar
echo "📋 Contenido del disco:"
iDSK20 cat

# 5. Ejecutar en emulador (opcional)
if [ -n "$RVM_PATH" ]; then
    echo "🎮 Ejecutando en RetroVirtualMachine..."
    iDSK20 run "loader.bas"
fi

echo "✅ Workflow completado"
```

---

## Gestión de Proyectos

### Ejemplo 14: Estructura de Proyecto

```bash
#!/bin/bash
# setup-project.sh

PROJECT_NAME=$1

if [ -z "$PROJECT_NAME" ]; then
    echo "Uso: $0 <nombre-proyecto>"
    exit 1
fi

echo "📁 Creando estructura de proyecto: $PROJECT_NAME"

# Crear directorios
mkdir -p $PROJECT_NAME/{src,data,graphics,music,build}

# Crear archivos de ejemplo
cat > $PROJECT_NAME/src/loader.bas << 'EOF'
10 MODE 0
20 PRINT "CARGANDO..."
30 LOAD "MAIN.BIN",8000
40 CALL 8000
EOF

# Crear Makefile
cat > $PROJECT_NAME/Makefile << 'EOF'
DISK = build/game.dsk

all: $(DISK)

$(DISK):
	@echo "📀 Creando disco..."
	@export DISC_DSK_FILE=$(DISK) && \
	iDSK20 new && \
	iDSK20 save "src/loader.bas",A && \
	iDSK20 save "build/main.bin",B,8000,8000 && \
	for file in data/*.dat; do \
		[ -f "$$file" ] && iDSK20 save "$$file",R; \
	done

clean:
	rm -f $(DISK)

list:
	@export DISC_DSK_FILE=$(DISK) && iDSK20 cat

.PHONY: all clean list
EOF

# Crear README
cat > $PROJECT_NAME/README.md << EOF
# $PROJECT_NAME

Proyecto de Amstrad CPC

## Estructura

- \`src/\` - Código fuente
- \`data/\` - Datos del juego
- \`graphics/\` - Gráficos
- \`music/\` - Música
- \`build/\` - Archivos compilados

## Compilar

\`\`\`bash
make
\`\`\`

## Ver contenido del disco

\`\`\`bash
make list
\`\`\`
EOF

echo "✅ Proyecto creado: $PROJECT_NAME"
echo ""
echo "Estructura:"
tree $PROJECT_NAME
```

### Ejemplo 15: Makefile para Proyecto CPC

```makefile
# Makefile para proyecto CPC

PROJECT = mygame
DISK = build/$(PROJECT).dsk
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
DATA = $(wildcard data/*.dat)

CC = z88dk-zcc
CFLAGS = +cpc -create-app

.PHONY: all clean disk list run

all: disk

# Compilar código
build/main.bin: $(SOURCES)
	@mkdir -p build
	$(CC) $(CFLAGS) -o build/main $(SOURCES)

# Crear disco
disk: build/main.bin
	@echo "📀 Creando disco..."
	@export DISC_DSK_FILE=$(DISK) && \
	iDSK20 new && \
	iDSK20 save "src/loader.bas",A && \
	iDSK20 save "build/main.bin",B,8000,8000 && \
	for file in $(DATA); do \
		iDSK20 save "$$file",R; \
	done
	@echo "✅ Disco creado: $(DISK)"

# Listar contenido
list:
	@export DISC_DSK_FILE=$(DISK) && iDSK20 cat

# Ejecutar en emulador
run: disk
	@export DISC_DSK_FILE=$(DISK) && iDSK20 run "loader.bas"

# Limpiar
clean:
	rm -rf build/*.o build/*.bin build/*.dsk

# Backup
backup:
	@./scripts/backup-disk.sh $(DISK)
```

---

## Conversión Masiva

### Ejemplo 16: Convertir Biblioteca de Programas

```bash
#!/bin/bash
# convert-library.sh

SOURCE_DIR="basic-programs"
OUTPUT_DIR="dsk-images"

mkdir -p "$OUTPUT_DIR"

echo "📚 Convirtiendo biblioteca de programas..."

# Crear un disco por cada 10 programas
DISK_NUM=1
FILE_COUNT=0
FILES_PER_DISK=10

for file in "$SOURCE_DIR"/*.bas; do
    if [ -f "$file" ]; then
        # Crear nuevo disco si es necesario
        if [ $FILE_COUNT -eq 0 ]; then
            DISK="$OUTPUT_DIR/library-$DISK_NUM.dsk"
            export DISC_DSK_FILE=$DISK
            iDSK20 new
            echo "📀 Disco $DISK_NUM: $DISK"
        fi
        
        # Añadir archivo
        BASENAME=$(basename "$file")
        echo "  → $BASENAME"
        iDSK20 save "$file",A
        
        FILE_COUNT=$((FILE_COUNT + 1))
        
        # Siguiente disco
        if [ $FILE_COUNT -eq $FILES_PER_DISK ]; then
            FILE_COUNT=0
            DISK_NUM=$((DISK_NUM + 1))
        fi
    fi
done

echo ""
echo "✅ Conversión completada"
echo "📦 Discos creados:"
ls -lh "$OUTPUT_DIR"/*.dsk
```

---

## Backup y Restauración

### Ejemplo 17: Sistema de Backup Incremental

```bash
#!/bin/bash
# incremental-backup.sh

DISK=$1
BACKUP_DIR="backups"
LATEST_LINK="$BACKUP_DIR/latest.dsk"

if [ -z "$DISK" ]; then
    echo "Uso: $0 <disco.dsk>"
    exit 1
fi

mkdir -p "$BACKUP_DIR"

# Verificar si hay cambios
if [ -L "$LATEST_LINK" ]; then
    if cmp -s "$DISK" "$LATEST_LINK"; then
        echo "ℹ️  No hay cambios desde el último backup"
        exit 0
    fi
fi

# Crear backup
DATE=$(date +%Y%m%d-%H%M%S)
BACKUP_FILE="$BACKUP_DIR/${DISK%.dsk}-$DATE.dsk"

cp "$DISK" "$BACKUP_FILE"
ln -sf "$(basename $BACKUP_FILE)" "$LATEST_LINK"

echo "✅ Backup creado: $BACKUP_FILE"

# Limpiar backups antiguos (mantener últimos 10)
ls -t "$BACKUP_DIR"/*.dsk | tail -n +11 | xargs -r rm

echo "📦 Backups disponibles: $(ls "$BACKUP_DIR"/*.dsk | wc -l)"
```

### Ejemplo 18: Restaurar desde Backup

```bash
#!/bin/bash
# restore-backup.sh

BACKUP_DIR="backups"

echo "📦 Backups disponibles:"
ls -lht "$BACKUP_DIR"/*.dsk | nl

echo ""
read -p "Número de backup a restaurar: " NUM

BACKUP=$(ls -t "$BACKUP_DIR"/*.dsk | sed -n "${NUM}p")

if [ -z "$BACKUP" ]; then
    echo "❌ Backup no válido"
    exit 1
fi

echo "📀 Restaurando: $BACKUP"

# Extraer nombre original
ORIGINAL=$(basename "$BACKUP" | sed 's/-[0-9]\{8\}-[0-9]\{6\}\.dsk/.dsk/')

# Confirmar
read -p "¿Restaurar como $ORIGINAL? (s/n): " CONFIRM

if [ "$CONFIRM" = "s" ]; then
    cp "$BACKUP" "$ORIGINAL"
    echo "✅ Restaurado: $ORIGINAL"
else
    echo "❌ Cancelado"
fi
```

---

## Casos de Uso Avanzados

### Ejemplo 19: CI/CD para Proyectos CPC

```yaml
# .github/workflows/build-cpc.yml

name: Build CPC Project

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v2
    
    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake build-essential
    
    - name: Build iDSK20
      run: |
        cd tools/disc
        ./build.sh
        export PATH=$PATH:$(pwd)/build
    
    - name: Create DSK image
      run: |
        export DISC_DSK_FILE=game.dsk
        iDSK20 new
        iDSK20 save "src/loader.bas",A
        iDSK20 save "build/main.bin",B,8000,8000
    
    - name: Upload artifact
      uses: actions/upload-artifact@v2
      with:
        name: game-dsk
        path: game.dsk
```

### Ejemplo 20: Testing Automatizado

```bash
#!/bin/bash
# test-disk.sh

DISK=$1
EXPECTED_FILES=$2

if [ -z "$DISK" ] || [ -z "$EXPECTED_FILES" ]; then
    echo "Uso: $0 <disco.dsk> <archivos-esperados>"
    exit 1
fi

export DISC_DSK_FILE=$DISK

echo "🧪 Testing disco: $DISK"

# Test 1: Disco existe
if [ ! -f "$DISK" ]; then
    echo "❌ Test 1 FAILED: Disco no existe"
    exit 1
fi
echo "✅ Test 1 PASSED: Disco existe"

# Test 2: Disco es válido
if ! iDSK20 cat > /dev/null 2>&1; then
    echo "❌ Test 2 FAILED: Disco inválido"
    exit 1
fi
echo "✅ Test 2 PASSED: Disco válido"

# Test 3: Número de archivos
ACTUAL_FILES=$(iDSK20 cat | grep -v "^DSK" | grep -v "^---" | grep -v "free" | wc -l)
if [ "$ACTUAL_FILES" -ne "$EXPECTED_FILES" ]; then
    echo "❌ Test 3 FAILED: Esperados $EXPECTED_FILES archivos, encontrados $ACTUAL_FILES"
    exit 1
fi
echo "✅ Test 3 PASSED: $ACTUAL_FILES archivos"

# Test 4: Espacio libre
FREE_SPACE=$(iDSK20 cat | grep "free" | awk '{print $3}')
if [ -z "$FREE_SPACE" ]; then
    echo "❌ Test 4 FAILED: No se puede determinar espacio libre"
    exit 1
fi
echo "✅ Test 4 PASSED: $FREE_SPACE espacio libre"

echo ""
echo "🎉 Todos los tests pasaron"
```

---

[← Volver al índice](README.md)

