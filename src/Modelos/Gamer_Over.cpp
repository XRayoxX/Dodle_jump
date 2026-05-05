//
// Created by Dell on 05/05/2026.
//

#include "../../include/Modelos/Gamer_Over.h"
GamerOver::GamerOver() {
    result = GO_NONE;

    // Posición de botones (ajústalos si quieres)
    btnRetry = { 300, 350, 200, 50 };
    btnMenu  = { 300, 420, 200, 50 };
    btnExit  = { 300, 490, 200, 50 };
}

GamerOver::~GamerOver() {
    UnloadTexture(background);
}

void GamerOver::Load(const char* imagePath) {
    background = LoadTexture(imagePath);
}

void GamerOver::Update() {
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, btnRetry)) {
            result = GO_RETRY;
        }
        if (CheckCollisionPointRec(mouse, btnMenu)) {
            result = GO_MENU;
        }
        if (CheckCollisionPointRec(mouse, btnExit)) {
            result = GO_EXIT;
        }
    }
}

void GamerOver::Draw() {
    // Escalar la imagen al tamaño de la ventana
    DrawTexturePro(
        background,
        (Rectangle){0, 0, (float)background.width, (float)background.height}, // origen
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, // destino (pantalla)
        (Vector2){0, 0},
        0.0f,
        WHITE
    );

    // Botones
    DrawRectangleRec(btnRetry, DARKGRAY);
    DrawRectangleRec(btnMenu, DARKGRAY);
    DrawRectangleRec(btnExit, DARKGRAY);

    DrawText("REINTENTAR", btnRetry.x + 40, btnRetry.y + 15, 20, WHITE);
    DrawText("MENU", btnMenu.x + 70, btnMenu.y + 15, 20, WHITE);
    DrawText("SALIR", btnExit.x + 70, btnExit.y + 15, 20, WHITE);
}

GameOverResult GamerOver::GetResult() {
    return result;
}

void GamerOver::Reset() {
    result = GO_NONE;
}