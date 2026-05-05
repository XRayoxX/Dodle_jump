#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "Modelos/Jugador.h"
#include "Modelos/Plataforma.h"
#include "Modelos/Mapas.h"
#include "Audio/AudioManager.h"

int main() {
    srand(time(NULL));

    const int screenWidth  = 600;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Dodle Jump -");

    // --- Audio ---
    InitAudioManager();

    // --- Fondo y texturas ---
    Mapas miMapa;
    InicializarMapas(miMapa, R"(G:/Ale/Documents/Duddle Jump/Imagenes/Fondo2.png)");

    CargarTexturasPlataformas();

    // --- Entidades de juego ---
    Plataforma misPlataformas[MAX_PLATAFORMAS];
    InicializarPlataformas(misPlataformas);

    Jugador miJugador;
    InicializarJugador(miJugador);

    SetTargetFPS(60);

    int puntaje = 0;

    while (!WindowShouldClose()) {

        // --- 1. SCROLL Y PUNTAJE ---
        float scrollSpeed = 0;
        if (miJugador.posicion.y < screenHeight / 2 && miJugador.velocidad.y < 0) {
            scrollSpeed = -miJugador.velocidad.y;
            miJugador.posicion.y = screenHeight / 2;
            puntaje += (int)scrollSpeed;
        }

        // --- 2. ACTUALIZACIÓN ---
        ActualizarPlataformas(misPlataformas, scrollSpeed, puntaje);
        ActualizarJugador(miJugador);

        // --- 3. COLISIONES ---
        for (int i = 0; i < MAX_PLATAFORMAS; i++) {
            if (misPlataformas[i].activa &&
                miJugador.velocidad.y > 0 &&
                CheckCollisionCircleRec(miJugador.posicion, miJugador.radio, misPlataformas[i].rect))
            {
                // Impulso según tipo (configurable desde Plataforma.h)
                if (misPlataformas[i].tipo == MOVIL_BOOST) {
                    miJugador.velocidad.y = IMPULSO_BOOST;
                } else {
                    miJugador.velocidad.y = IMPULSO_NORMAL;
                }

                // Sonido segun plataforma para evitar solapes.
                PlayJumpSound(misPlataformas[i].tipo);

                if (misPlataformas[i].tipo == UN_USO) {
                    misPlataformas[i].activa = false;
                }
            }
        }

        // --- 4. DERROTA ---
        if (miJugador.posicion.y > screenHeight) {
            InicializarPlataformas(misPlataformas);
            InicializarJugador(miJugador);
            puntaje = 0;
        }

        // --- 5. DIBUJO ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DibujarMapas(miMapa, screenWidth, screenHeight);
        DibujarPlataformas(misPlataformas);
        DibujarJugador(miJugador);

        DrawText(TextFormat("Puntaje: %i", puntaje), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Nivel:   %i", ObtenerNivel(puntaje)), 10, 35, 20, RED);

        EndDrawing(); // ← faltaba esto también
    }

    // --- Limpieza ---
    LiberarMapas(miMapa);
    LiberarTexturasPlataformas();
    ShutdownAudioManager();
    CloseWindow();
    return 0;
}