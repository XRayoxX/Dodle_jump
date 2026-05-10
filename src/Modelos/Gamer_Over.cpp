#include "Modelos/Gamer_Over.h"

namespace {
    const float kDesignWidth = 1200.0f;
    const float kDesignHeight = 896.0f;

    const Rectangle kRetryBase = { 450.0f, 522.6f, 300.0f, 74.7f };
    const Rectangle kMenuBase = { 450.0f, 627.2f, 300.0f, 74.7f };
    const Rectangle kExitBase = { 450.0f, 731.9f, 300.0f, 74.7f };
}

GamerOver::GamerOver() {

    result = GO_NONE;

    btnRetry = { 0, 0, 0, 0 };
    btnMenu  = { 0, 0, 0, 0 };
    btnExit  = { 0, 0, 0, 0 };
}

GamerOver::~GamerOver() {

    UnloadTexture(background);
}

void GamerOver::Load(const char* imagePath) {

    background = LoadTexture(imagePath);
    UpdateLayout();
}

void GamerOver::UpdateLayout() {

    float screenW = (float)GetScreenWidth();
    float screenH = (float)GetScreenHeight();

    float scaleX = screenW / kDesignWidth;
    float scaleY = screenH / kDesignHeight;

    btnRetry = {
        kRetryBase.x * scaleX,
        kRetryBase.y * scaleY,
        kRetryBase.width * scaleX,
        kRetryBase.height * scaleY
    };

    btnMenu = {
        kMenuBase.x * scaleX,
        kMenuBase.y * scaleY,
        kMenuBase.width * scaleX,
        kMenuBase.height * scaleY
    };

    btnExit = {
        kExitBase.x * scaleX,
        kExitBase.y * scaleY,
        kExitBase.width * scaleX,
        kExitBase.height * scaleY
    };
}

void GamerOver::Update() {

    UpdateLayout();

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

    UpdateLayout();

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

    int fontSize = (int)(20.0f * ((float)GetScreenHeight() / kDesignHeight));

    const char* retryText = "REINTENTAR";
    int retryWidth = MeasureText(retryText, fontSize);
    DrawText(
        retryText,
        (int)(btnRetry.x + (btnRetry.width - retryWidth) * 0.5f),
        (int)(btnRetry.y + (btnRetry.height - fontSize) * 0.5f),
        fontSize,
        WHITE
    );

    const char* menuText = "MENU";
    int menuWidth = MeasureText(menuText, fontSize);
    DrawText(
        menuText,
        (int)(btnMenu.x + (btnMenu.width - menuWidth) * 0.5f),
        (int)(btnMenu.y + (btnMenu.height - fontSize) * 0.5f),
        fontSize,
        WHITE
    );

    const char* exitText = "SALIR";
    int exitWidth = MeasureText(exitText, fontSize);
    DrawText(
        exitText,
        (int)(btnExit.x + (btnExit.width - exitWidth) * 0.5f),
        (int)(btnExit.y + (btnExit.height - fontSize) * 0.5f),
        fontSize,
        WHITE
    );
}

GameOverResult GamerOver::GetResult() {

    return result;
}

void GamerOver::Reset() {

    result = GO_NONE;
}