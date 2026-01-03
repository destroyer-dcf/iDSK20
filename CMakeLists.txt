cmake_minimum_required(VERSION 3.10)
PROJECT(iDSK20)

# Configurar el directorio de salida para el ejecutable
# En out-of-source builds, esto pondrá el binario en build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(iDSK20
	src/Basic.cpp
	src/BitmapCPC.cpp
	src/Dams.cpp
	src/Desass.cpp
	src/endianPPC.cpp
	src/GestDsk.cpp
	src/getopt_pp.cpp
	src/Main.cpp
	src/Outils.cpp
	src/ViewFile.cpp
	src/Ascii.cpp
)
