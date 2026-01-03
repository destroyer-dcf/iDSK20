# Cambio de Nombre: disc → iDSK20

## Resumen
El proyecto y binario han sido renombrados de "disc" a "iDSK20" para reflejar mejor su identidad como versión 2.0 de iDSK.

## Cambios Realizados

### 1. CMakeLists.txt
- ✅ Nombre del ejecutable cambiado de `disc` a `iDSK20`
- ✅ Nombre del proyecto ya era `iDSK20`

### 2. Scripts de Build
Todos los scripts han sido actualizados para usar el nuevo nombre:

#### build.sh
- ✅ Comentarios actualizados
- ✅ Verificación del binario `build/iDSK20`
- ✅ Mensajes de salida actualizados

#### build-local-cross.sh
- ✅ Referencias al binario actualizadas en todas las plataformas:
  - `iDSK20` para macOS y Linux
  - `iDSK20.exe` para Windows
- ✅ Resumen de binarios generados actualizado

#### build-multiplatform.sh
- ✅ Comentarios del script actualizados
- ✅ Todas las referencias al binario cambiadas
- ✅ Nombres de paquetes actualizados: `iDSK20-$VERSION-$platform.zip`
- ✅ Resumen de compilación actualizado

## Verificación

### Compilación Exitosa
```bash
./build.sh
# ✅ Binario generado: build/iDSK20
# ✅ Tamaño: 337K
# ✅ Binario funciona correctamente
```

### Prueba del Binario
```bash
./build/iDSK20 help
# ✅ Muestra ayuda correctamente
# ✅ Versión: 1.0.0
```

## Estructura de Archivos

### Antes
```
disc/
├── build/
│   └── disc          # Binario antiguo
├── CMakeLists.txt    # add_executable(disc ...)
└── build.sh          # Referencias a "disc"
```

### Después
```
disc/
├── build/
│   └── iDSK20        # Binario nuevo ✅
├── CMakeLists.txt    # add_executable(iDSK20 ...) ✅
└── build.sh          # Referencias a "iDSK20" ✅
```

## Compatibilidad

### Plataformas Soportadas
Todos los scripts mantienen soporte completo para:
- ✅ macOS ARM64 (Apple Silicon)
- ✅ macOS x64 (Intel)
- ✅ Linux x64
- ✅ Linux ARM64
- ✅ Windows x64

### Nombres de Binarios por Plataforma
- **Unix (macOS/Linux):** `iDSK20`
- **Windows:** `iDSK20.exe`

## Paquetes de Distribución

Los paquetes generados ahora usan el nuevo nombre:
- `iDSK20-1.0.0-macos-arm64.tar.gz`
- `iDSK20-1.0.0-macos-x64.tar.gz`
- `iDSK20-1.0.0-linux-x64.tar.gz`
- `iDSK20-1.0.0-linux-arm64.tar.gz`
- `iDSK20-1.0.0-windows-x64.zip`

## Próximos Pasos

Para usar el nuevo binario:

```bash
# Compilar
./build.sh

# Ejecutar
./build/iDSK20 help
./build/iDSK20 cat mydisk.dsk
./build/iDSK20 new newdisk.dsk

# Con environment variable (opcional)
export PATH=$PATH:$(pwd)/build
iDSK20 help
```

## Notas

- El nombre del directorio del proyecto sigue siendo `disc/` (no se cambió para mantener compatibilidad con la estructura del repositorio)
- Solo el nombre del binario y las referencias internas fueron actualizadas
- Todos los scripts de build funcionan correctamente con el nuevo nombre
- La funcionalidad del programa no ha cambiado, solo el nombre del ejecutable

---

**Fecha:** 2 de enero de 2025  
**Autor:** Destroyer  
**Estado:** ✅ Completado y verificado
