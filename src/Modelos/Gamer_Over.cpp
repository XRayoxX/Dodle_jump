#include "Modelos/Gamer_Over.h"

GamerOver::GamerOver() {

    result = GO_NONE;

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

    DrawTexturePro(
        background,

        (Rectangle){
            0,
            0,
            (float)background.width,
            (float)background.height
        },

        (Rectangle){
            0,
            0,
            (float)GetScreenWidth(),
            (float)GetScreenHeight()
        },

        (Vector2){0, 0},
        0.0f,
        WHITE
    );

    DrawRectangleRec(btnRetry, DARKGRAY);
    DrawRectangleRec(btnMenu, DARKGRAY);
    DrawRectangleRec(btnExit, DARKGRAY);

    DrawText(
        "REINTENTAR",
        (int)btnRetry.x + 40,
        (int)btnRetry.y + 15,
        20,
        WHITE
    );

    DrawText(
        "MENU",
        (int)btnMenu.x + 70,
        (int)btnMenu.y + 15,
        20,
        WHITE
    );

    DrawText(
        "SALIR",
        (int)btnExit.x + 70,
        (int)btnExit.y + 15,
        20,
        WHITE
    );
}

GameOverResult GamerOver::GetResult() {

    return result;
}

void GamerOver::Reset() {

    result = GO_NONE;
}