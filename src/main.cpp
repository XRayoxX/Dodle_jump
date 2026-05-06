#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "Modelos/Jugador.h"
#include "Modelos/Plataforma.h"
#include "Modelos/Mapas.h"
#include "Audio/AudioManager.h"
#include "Modelos/Login.h"

// Estados del juego
enum GameScreen {
    SCREEN_LOGIN,
    SCREEN_GAME
};

int main() {
    srand(time(NULL));

    const int screenWidth  = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Doodle Jump - Metal Slug Edition");

    InitAudioManager();

    // --- LOGIN ---
    GameScreen currentScreen = SCREEN_LOGIN;

    Login login;
    login.Init();

    // --- MAPA Y JUEGO ---
    Mapas miMapa;
    InicializarMapas(miMapa, "Imagenes/Fondo2.png");
    CargarTexturasPlataformas();

    // --- ANIMACIONES ---
    const int maxFrames = 13;
    Texture2D marcoAnimacion[maxFrames];

    for (int i = 0; i < maxFrames; i++) {
        marcoAnimacion[i] = LoadTexture(TextFormat("Imagenes/marco_salto%i.PNG", i + 1));
    }

    int frameActual = 0;
    int framesCounter = 0;
    float direccion = 1.0f;

    Plataforma misPlataformas[MAX_PLATAFORMAS];
    InicializarPlataformas(misPlataformas);

    Jugador miJugador;
    InicializarJugador(miJugador);

    SetTargetFPS(60);
    int puntaje = 0;

    // --- LOOP PRINCIPAL ---
    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // =========================
        // PANTALLA LOGIN
        // =========================
        if (currentScreen == SCREEN_LOGIN) {

            // login.Draw() dibuja la imagen, procesa campos y boton
            // retorna true cuando el usuario presiona "Iniciar Sesion"
            if (login.Draw()) {
                currentScreen = SCREEN_GAME;
            }
        }

        // =========================
        // PANTALLA JUEGO
        // =========================
        else if (currentScreen == SCREEN_GAME) {

            // --- 1. SCROLL Y PUNTAJE ---
            float scrollSpeed = 0;
            if (miJugador.posicion.y < screenHeight / 2 && miJugador.velocidad.y < 0) {
                scrollSpeed = -miJugador.velocidad.y;
                miJugador.posicion.y = screenHeight / 2;
                puntaje += (int)scrollSpeed;
            }

            // --- 2. ACTUALIZACION ---
            ActualizarPlataformas(misPlataformas, scrollSpeed, puntaje);
            ActualizarJugador(miJugador);

            if (IsKeyDown(KEY_LEFT)) direccion = -1.0f;
            if (IsKeyDown(KEY_RIGHT)) direccion = 1.0f;

            // --- 3. COLISIONES ---
            for (int i = 0; i < MAX_PLATAFORMAS; i++) {
                if (misPlataformas[i].activa &&
                    miJugador.velocidad.y > 0 &&
                    CheckCollisionCircleRec(miJugador.posicion, miJugador.radio, misPlataformas[i].rect))
                {
                    frameActual = 0;
                    framesCounter = 0;
                    miJugador.velocidad.y = (misPlataformas[i].tipo == MOVIL_BOOST) ? IMPULSO_BOOST : IMPULSO_NORMAL;
                    PlayJumpSound(misPlataformas[i].tipo);
                    if (misPlataformas[i].tipo == UN_USO) misPlataformas[i].activa = false;
                }
            }

            // --- 4. ANIMACION ---
            if (miJugador.velocidad.y != 0) {
                framesCounter++;
                int velocidadAnimacionActual = 15;

                if (framesCounter >= (60 / velocidadAnimacionActual)) {
                    framesCounter = 0;
                    frameActual++;
                    if (frameActual >= maxFrames) frameActual = 0;
                }
            } else {
                frameActual = 0;
            }

            // --- 5. DERROTA ---
            if (miJugador.posicion.y > screenHeight) {
                InicializarPlataformas(misPlataformas);
                InicializarJugador(miJugador);
                puntaje = 0;
            }

            // --- 6. DIBUJO ---
            DibujarMapas(miMapa, screenWidth, screenHeight);
            DibujarPlataformas(misPlataformas);

            float currentWidth  = (float)marcoAnimacion[frameActual].width;
            float currentHeight = (float)marcoAnimacion[frameActual].height;

            Vector2 posicionDibujo = {
                miJugador.posicion.x - currentWidth,
                miJugador.posicion.y - (currentHeight * 1.5f)
            };

            DrawTexturePro(marcoAnimacion[frameActual],
                           { 0, 0, currentWidth * direccion, currentHeight },
                           { posicionDibujo.x, posicionDibujo.y, currentWidth * 2.0f, currentHeight * 2.0f },
                           { 0, 0 }, 0.0f, WHITE);

            DrawText(TextFormat("Puntaje: %i", puntaje), 10, 10, 20, DARKGRAY);
            DrawText(TextFormat("Nivel:   %i", ObtenerNivel(puntaje)), 10, 35, 20, RED);
        }

        EndDrawing();
    }

    // --- LIMPIEZA ---
    for (int i = 0; i < maxFrames; i++) {
        UnloadTexture(marcoAnimacion[i]);
    }

    LiberarMapas(miMapa);
    LiberarTexturasPlataformas();
    ShutdownAudioManager();
    CloseWindow();

    return 0;
}