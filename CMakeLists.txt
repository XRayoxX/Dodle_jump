cmake_minimum_required(VERSION 3.20)
project(Dodle_jump)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenSSL para CPR/CURL
set(ZLIB_ROOT "C:/msys64/ucrt64")
set(OPENSSL_ROOT_DIR "C:/msys64/ucrt64")

# Raylib
add_subdirectory(raylib)

# Buscar librerías
find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)

# Copiar carpetas
file(COPY Imagenes DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Audio DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Ejecutable
add_executable(Dodle_jump
        src/main.cpp
        src/Modelos/Jugador.cpp
        include/Modelos/Jugador.h
        src/Modelos/Plataforma.cpp
        include/Modelos/Plataforma.h
        src/Modelos/Mapas.cpp
        include/Modelos/Mapas.h
        src/Audio/AudioManager.cpp
        include/Audio/AudioManager.h
        include/Modelos/GameApiConfig.h
        include/Modelos/ApiClient.h
        src/Modelos/ApiClient.cpp
        src/Modelos/Login.cpp
        src/Modelos/Gamer_Over.cpp
        include/Modelos/Gamer_Over.h
)

# Includes
target_include_directories(Dodle_jump PRIVATE include)

# Librerías
target_link_libraries(Dodle_jump PRIVATE
        raylib
        cpr::cpr
        nlohmann_json::nlohmann_json
)