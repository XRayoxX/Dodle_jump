#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#include "Modelos/Jugador.h"
#include "Modelos/Plataforma.h"
#include "Modelos/Mapas.h"
#include "Audio/AudioManager.h"
#include "Modelos/Login.h"
#include "Modelos/Gamer_Over.h"
#include "Modelos/ApiClient.h"
#include "Modelos/GameApiConfig.h"

// Estados del juego
enum GameScreen {
    SCREEN_LOGIN,
    SCREEN_GAME,
    SCREEN_GAMEOVER
};

int main() {

    srand(time(NULL));

    const int screenWidth  = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Doodle Jump - Metal Slug Edition");

    InitAudioManager();

    // =====================================================
    // API
    // =====================================================
    ApiClient api(
        GameApiConfig::BASE_URL,
        GameApiConfig::API_KEY,
        GameApiConfig::CODIGO_JUEGO
    );

    UsuarioApi usuario;
    PartidaApi partida;

    std::string errorMsg = "";

    bool loginOk = false;
    bool partidaIniciada = false;
    bool partidaFinalizada = false;

    // =====================================================
    // LOGIN
    // =====================================================
    GameScreen currentScreen = SCREEN_LOGIN;

    Login login;
    login.Init();

    // =====================================================
    // GAME OVER
    // =====================================================
    GamerOver gameOver;
    gameOver.Load("Imagenes/GamerOver.png");

    // =====================================================
    // MAPA Y JUEGO
    // =====================================================
    Mapas miMapa;
    InicializarMapas(miMapa,
                     R"(G:/Ale/Documents/Duddle Jump/Imagenes/Fondo2.png)",
                     R"(G:/Ale/Documents/Duddle Jump/Imagenes/Fondo1.png)");

    CargarTexturasPlataformas();

    // =====================================================
    // ANIMACIONES
    // =====================================================
    const int maxFrames = 13;
    Texture2D marcoAnimacion[maxFrames];

    for (int i = 0; i < maxFrames; i++) {

        marcoAnimacion[i] =
            LoadTexture(TextFormat("Imagenes/marco_salto%i.PNG", i + 1));
    }

    int frameActual = 0;
    int framesCounter = 0;

    float direccion = 1.0f;

    // =====================================================
    // PLATAFORMAS
    // =====================================================
    Plataforma misPlataformas[MAX_PLATAFORMAS];
    InicializarPlataformas(misPlataformas);

    // =====================================================
    // JUGADOR
    // =====================================================
    Jugador miJugador;
    InicializarJugador(miJugador);

    SetTargetFPS(60);

    int puntaje = 0;

    // =====================================================
    // LOOP PRINCIPAL
    // =====================================================
    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // =====================================================
        // LOGIN
        // =====================================================
        if (currentScreen == SCREEN_LOGIN) {

            if (login.Draw()) {

                std::string username = login.GetUsername();
                std::string password = login.GetPassword();

                if (api.loginJugador(
                        username,
                        password,
                        usuario,
                        errorMsg
                    ))
                {

                    if (api.iniciarPartida(
                            partida,
                            errorMsg,
                            GameApiConfig::VERSION_JUEGO,
                            GameApiConfig::COSTO_PARTIDA
                        ))
                    {

                        loginOk = true;
                        partidaIniciada = true;

                        currentScreen = SCREEN_GAME;
                    }
                    else {
                        login.SetLoginFailed(true);
                    }
                }
                else {
                    login.SetLoginFailed(true);
                }
            }
        }

        // =====================================================
        // JUEGO
        // =====================================================
        else if (currentScreen == SCREEN_GAME) {

            // =====================================================
            // SCROLL Y PUNTAJE
            // =====================================================
            float scrollSpeed = 0;

            if (miJugador.posicion.y < screenHeight / 2 &&
                miJugador.velocidad.y < 0)
            {

                scrollSpeed = -miJugador.velocidad.y;

                miJugador.posicion.y = screenHeight / 2;

                puntaje += (int)scrollSpeed;
            }

            // =====================================================
            // ACTUALIZACION
            // =====================================================
            ActualizarPlataformas(
                misPlataformas,
                scrollSpeed,
                puntaje
            );

            ActualizarJugador(miJugador);

            if (IsKeyDown(KEY_LEFT))
                direccion = -1.0f;

            if (IsKeyDown(KEY_RIGHT))
                direccion = 1.0f;

            // =====================================================
            // COLISIONES
            // =====================================================
            for (int i = 0; i < MAX_PLATAFORMAS; i++) {

                if (misPlataformas[i].activa &&
                    miJugador.velocidad.y > 0 &&
                    CheckCollisionCircleRec(
                        miJugador.posicion,
                        miJugador.radio,
                        misPlataformas[i].rect
                    ))
                {

                    frameActual = 0;
                    framesCounter = 0;

                    miJugador.velocidad.y =
                        (misPlataformas[i].tipo == MOVIL_BOOST)
                        ? IMPULSO_BOOST
                        : IMPULSO_NORMAL;

                    PlayJumpSound(misPlataformas[i].tipo);

                    if (misPlataformas[i].tipo == UN_USO)
                        misPlataformas[i].activa = false;
                }
            }

            // =====================================================
            // ANIMACION
            // =====================================================
            if (miJugador.velocidad.y != 0) {

                framesCounter++;

                int velocidadAnimacionActual = 15;

                if (framesCounter >= (60 / velocidadAnimacionActual)) {

                    framesCounter = 0;

                    frameActual++;

                    if (frameActual >= maxFrames)
                        frameActual = 0;
                }
            }
            else {
                frameActual = 0;
            }

            // =====================================================
            // DERROTA
            // =====================================================
            if (miJugador.posicion.y > screenHeight) {

                if (!partidaFinalizada) {

                    int tokensGanados =
                        (puntaje / GameApiConfig::SCORE_POR_PREMIO)
                        * GameApiConfig::TOKENS_POR_PREMIO;

                    std::string resultado = "LOSE";

                    if (tokensGanados > 0) {
                        resultado = "WIN";
                    }

                    api.finalizarPartida(
                        partida.idPartida,
                        puntaje,
                        ObtenerNivel(puntaje),
                        resultado,
                        120,
                        tokensGanados,
                        errorMsg
                    );

                    partidaFinalizada = true;
                }

                currentScreen = SCREEN_GAMEOVER;
            }

            // =====================================================
            // DIBUJO
            // =====================================================
            DibujarMapas(miMapa, screenWidth, screenHeight, ObtenerNivel(puntaje));

            DibujarPlataformas(misPlataformas);

            float currentWidth =
                (float)marcoAnimacion[frameActual].width;

            float currentHeight =
                (float)marcoAnimacion[frameActual].height;

            Vector2 posicionDibujo = {

                miJugador.posicion.x - currentWidth,

                miJugador.posicion.y - (currentHeight * 1.5f)
            };

            DrawTexturePro(
                marcoAnimacion[frameActual],

                { 0, 0,
                  currentWidth * direccion,
                  currentHeight },

                { posicionDibujo.x,
                  posicionDibujo.y,
                  currentWidth * 2.0f,
                  currentHeight * 2.0f },

                { 0, 0 },

                0.0f,

                WHITE
            );

            DrawText(
                TextFormat("Puntaje: %i", puntaje),
                10,
                10,
                20,
                DARKGRAY
            );

            DrawText(
                TextFormat("Nivel: %i", ObtenerNivel(puntaje)),
                10,
                35,
                20,
                RED
            );
        }

        // =====================================================
        // GAME OVER
        // =====================================================
        else if (currentScreen == SCREEN_GAMEOVER) {

            gameOver.Update();

            gameOver.Draw();

            if (gameOver.GetResult() == GO_RETRY) {

                InicializarPlataformas(misPlataformas);

                InicializarJugador(miJugador);

                puntaje = 0;

                partidaFinalizada = false;

                gameOver.Reset();

                currentScreen = SCREEN_GAME;
            }

            else if (gameOver.GetResult() == GO_MENU) {

                InicializarPlataformas(misPlataformas);

                InicializarJugador(miJugador);

                puntaje = 0;

                partidaFinalizada = false;

                gameOver.Reset();

                currentScreen = SCREEN_LOGIN;
            }

            else if (gameOver.GetResult() == GO_EXIT) {

                CloseWindow();
            }
        }

        EndDrawing();
    }

    // =====================================================
    // LIMPIEZA
    // =====================================================
    for (int i = 0; i < maxFrames; i++) {
        UnloadTexture(marcoAnimacion[i]);
    }

    LiberarMapas(miMapa);

    LiberarTexturasPlataformas();

    ShutdownAudioManager();

    CloseWindow();

    return 0;
}