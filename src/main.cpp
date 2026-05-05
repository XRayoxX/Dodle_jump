#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "Modelos/Jugador.h"
#include "Modelos/Plataforma.h"
#include "Modelos/Mapas.h"
#include "Audio/AudioManager.h"
#include "Modelos/Gamer_Over.h"   // 👈 IMPORTANTE

enum EstadoJuego {
    JUGANDO,
    GAME_OVER
};

int main() {
    srand(time(NULL));

    const int screenWidth  = 600;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Dodle Jump -");

    // --- Audio ---
    InitAudioManager();

    // --- Fondo y texturas ---
    Mapas miMapa;
    InicializarMapas(miMapa, R"(Imagenes/Fondo2.png)");

    CargarTexturasPlataformas();

    // --- Entidades ---
    Plataforma misPlataformas[MAX_PLATAFORMAS];
    InicializarPlataformas(misPlataformas);

    Jugador miJugador;
    InicializarJugador(miJugador);

    // --- GAME OVER ---
    GamerOver gameOver;
    gameOver.Load("Imagenes/Gamer Over.png"); // 👈 tu imagen

    EstadoJuego estado = JUGANDO;

    SetTargetFPS(60);

    int puntaje = 0;

    while (!WindowShouldClose()) {

        // =========================
        // 🎮 ESTADO: JUGANDO
        // =========================
        if (estado == JUGANDO) {

            float scrollSpeed = 0;
            if (miJugador.posicion.y < screenHeight / 2 && miJugador.velocidad.y < 0) {
                scrollSpeed = -miJugador.velocidad.y;
                miJugador.posicion.y = screenHeight / 2;
                puntaje += (int)scrollSpeed;
            }

            ActualizarPlataformas(misPlataformas, scrollSpeed, puntaje);
            ActualizarJugador(miJugador);

            // --- Colisiones ---
            for (int i = 0; i < MAX_PLATAFORMAS; i++) {
                if (misPlataformas[i].activa &&
                    miJugador.velocidad.y > 0 &&
                    CheckCollisionCircleRec(miJugador.posicion, miJugador.radio, misPlataformas[i].rect))
                {
                    if (misPlataformas[i].tipo == MOVIL_BOOST) {
                        miJugador.velocidad.y = IMPULSO_BOOST;
                    } else {
                        miJugador.velocidad.y = IMPULSO_NORMAL;
                    }

                    PlayJumpSound(misPlataformas[i].tipo);

                    if (misPlataformas[i].tipo == UN_USO) {
                        misPlataformas[i].activa = false;
                    }
                }
            }

            // --- DERROTA ---
            if (miJugador.posicion.y > screenHeight) {
                estado = GAME_OVER;   // 👈 CAMBIO CLAVE
                gameOver.Reset();
            }
        }

        // =========================
        // 💀 ESTADO: GAME OVER
        // =========================
        else if (estado == GAME_OVER) {

            gameOver.Update();

            if (gameOver.GetResult() == GO_RETRY) {
                InicializarPlataformas(misPlataformas);
                InicializarJugador(miJugador);
                puntaje = 0;
                estado = JUGANDO;
                gameOver.Reset();
            }

            if (gameOver.GetResult() == GO_MENU) {
                // Aquí luego puedes mandar a login/menu
                InicializarPlataformas(misPlataformas);
                InicializarJugador(miJugador);
                puntaje = 0;
                estado = JUGANDO;
                gameOver.Reset();
            }

            if (gameOver.GetResult() == GO_EXIT) {
                break;
            }
        }

        // =========================
        // 🎨 DIBUJO
        // =========================
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (estado == JUGANDO) {
            DibujarMapas(miMapa, screenWidth, screenHeight);
            DibujarPlataformas(misPlataformas);
            DibujarJugador(miJugador);

            DrawText(TextFormat("Puntaje: %i", puntaje), 10, 10, 20, DARKGRAY);
            DrawText(TextFormat("Nivel:   %i", ObtenerNivel(puntaje)), 10, 35, 20, RED);
        }
        else if (estado == GAME_OVER) {
            gameOver.Draw(); // 👈 aquí se muestra la pantalla
        }

        EndDrawing();
    }

    // --- Limpieza ---
    LiberarMapas(miMapa);
    LiberarTexturasPlataformas();
    ShutdownAudioManager();
    CloseWindow();

    return 0;
}