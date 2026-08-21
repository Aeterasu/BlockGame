# Toolchain file for cross-compiling from Linux Mint to 64-bit Windows
# using MinGW-w64's GCC.
#
# Usage:
#   cmake -B build-windows -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-toolchain.cmake
#   cmake --build build-windows

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER  x86_64-w64-mingw32-windres)

# Where MinGW's cross libraries (and the mingw SDL2 dev package) live.
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Statically link the compiler runtime so the .exe doesn't need
# libgcc/libstdc++ DLLs sitting next to it on someone else's machine.
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
