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