cmake_minimum_required(VERSION 3.20)
project(Dodle_jump)

set(CMAKE_CXX_STANDARD 20)

# 1. Añadimos raylib
add_subdirectory(raylib)

file(COPY Imagenes DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 2. Creamos el ejecutable incluyendo las rutas a las carpetas Modelos
add_executable(Dodle_jump
        src/main.cpp
        src/Modelos/Jugador.cpp
        include/Modelos/Jugador.h
        src/Modelos/Plataforma.cpp
        include/Modelos/Plataforma.h
        src/Modelos/Mapas.cpp
        include/Modelos/Mapas.h
)

# 3. Enlazamos la librería
target_link_libraries(Dodle_jump PRIVATE raylib)

# 4. IMPORTANTE: Decirle a CMake dónde buscar los archivos .h
# Al poner 'include', podrás usar #include "Modelos/Jugador.h"
target_include_directories(Dodle_jump PRIVATE include)