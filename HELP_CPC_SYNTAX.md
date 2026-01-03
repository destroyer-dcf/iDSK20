# Añadida Sintaxis CPC a la Ayuda de iDSK20

## Resumen
Se ha añadido una sección en la ayuda general que explica la sintaxis CPC para el comando `save`, similar al comando SAVE de Amstrad CPC.

## Cambio Realizado

### Archivo Modificado
- `disc/src/Main.cpp` - Función de ayuda general

### Contenido Añadido

```
CPC SYNTAX for 'save' command (like Amstrad SAVE):
  iDSK20 save <dskfile> "<file>"[,type,load,exec]
  Types: A = ASCII/BASIC, B = Binary, P = Protected (read-only)
  Examples:
    iDSK20 save floppy.dsk "program.bas",A           # ASCII file
    iDSK20 save floppy.dsk "loader.bin",B,8000,8010  # Binary with addresses
    iDSK20 save floppy.dsk "data.dat",P              # Protected file
```

## Ubicación en la Ayuda

La nueva sección aparece:
1. Después de "OPTIONS for 'save' command"
2. Antes de "ENVIRONMENT VARIABLES"

Esto proporciona un flujo lógico:
- Primero se muestran las opciones tradicionales (-t, -e, -c, etc.)
- Luego se muestra la sintaxis CPC alternativa
- Finalmente las variables de entorno

## Formato CPC Soportado

### Sintaxis Completa
```
iDSK20 save <dskfile> "<filename>"[,type,load,exec]
```

### Tipos de Archivo
- **A** - ASCII/BASIC (tipo 0)
- **B** - Binary (tipo 1)
- **P** - Protected/Read-only (marca el archivo como protegido)

### Parámetros Opcionales
- **type** - Tipo de archivo (A, B, o P)
- **load** - Dirección de carga en hexadecimal (solo para tipo B)
- **exec** - Dirección de ejecución en hexadecimal (solo para tipo B)

## Ejemplos de Uso

### Archivo ASCII/BASIC
```bash
iDSK20 save floppy.dsk "program.bas",A
```
Equivalente a:
```bash
iDSK20 save floppy.dsk program.bas -t 0
```

### Archivo Binario con Direcciones
```bash
iDSK20 save floppy.dsk "loader.bin",B,8000,8010
```
Equivalente a:
```bash
iDSK20 save floppy.dsk loader.bin -t 1 -c 8000 -e 8010
```

### Archivo Protegido
```bash
iDSK20 save floppy.dsk "data.dat",P
```
Equivalente a:
```bash
iDSK20 save floppy.dsk data.dat -o
```

## Comparación con Amstrad CPC

### Comando SAVE en Amstrad CPC
```basic
SAVE "filename"
SAVE "filename",A
SAVE "filename",B,&8000,&8010
SAVE "filename",P
```

### Comando save en iDSK20
```bash
iDSK20 save disk.dsk "filename"
iDSK20 save disk.dsk "filename",A
iDSK20 save disk.dsk "filename",B,8000,8010
iDSK20 save disk.dsk "filename",P
```

La sintaxis es prácticamente idéntica, solo se añade el nombre del archivo DSK como primer parámetro.

## Beneficios

1. **Familiaridad** - Los usuarios de Amstrad CPC reconocen inmediatamente la sintaxis
2. **Compatibilidad** - Facilita la migración de scripts y comandos existentes
3. **Simplicidad** - Sintaxis más concisa que las opciones tradicionales
4. **Documentación** - Ahora está claramente documentado en la ayuda

## Visualización con Colores

La ayuda usa colores ANSI para mejorar la legibilidad:
- **Cyan brillante** - Comandos (save)
- **Amarillo** - Parámetros y opciones
- **Gris/Dim** - Comentarios y texto opcional
- **Verde brillante** - Variables de entorno
- **Blanco** - Nombre del programa (iDSK20)

## Verificación

Para ver la nueva ayuda:
```bash
./build/iDSK20 help
```

Para probar la sintaxis CPC:
```bash
# Crear disco de prueba
./build/iDSK20 new test.dsk

# Guardar archivo con sintaxis CPC
./build/iDSK20 save test.dsk "program.bas",A

# Verificar
./build/iDSK20 cat test.dsk
```

## Compatibilidad

- ✅ Sintaxis tradicional sigue funcionando
- ✅ Sintaxis CPC es completamente funcional
- ✅ Ambas sintaxis pueden usarse indistintamente
- ✅ La ayuda documenta ambas opciones

---

**Fecha:** 2 de enero de 2025  
**Autor:** Destroyer  
**Estado:** ✅ Completado y verificado  
**Líneas añadidas:** 8  
**Archivo modificado:** disc/src/Main.cpp
