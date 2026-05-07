#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "../../include/Modelos/Login.h"
#include <string.h>

Login::Login() {
    username[0] = '\0';
    password[0] = '\0';

    usernameEditMode = false;
    passwordEditMode = false;

    loginFailed = false;
}

Login::~Login() {
    UnloadTexture(background);
}

void Login::Init() {
    background = LoadTexture("Imagenes/Login.png");
}

std::string Login::GetUsername() const {
    return username;
}

std::string Login::GetPassword() const {
    return password;
}

void Login::SetLoginFailed(bool value) {
    loginFailed = value;
}

bool Login::Draw() {

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    // =========================
    // FONDO
    // =========================
    DrawTexturePro(
        background,
        Rectangle{ 0, 0, (float)background.width, (float)background.height },
        Rectangle{ 0, 0, (float)screenW, (float)screenH },
        Vector2{ 0, 0 },
        0.0f,
        WHITE
    );

    // =========================
    // PANEL LOGIN
    // =========================
    float panelW = 260.0f;
    float panelX = (screenW - panelW) / 2.0f;

    float yUsuario    = 330.0f;
    float yContrasena = 385.0f;
    float yBoton      = 430.0f;
    float yError      = 470.0f;

    Rectangle recUser = { panelX, yUsuario,    panelW, 38 };
    Rectangle recPass = { panelX, yContrasena, panelW, 38 };
    Rectangle recBtn  = { panelX, yBoton,      panelW, 42 };

    // =========================
    // QUITAR FOCO
    // =========================
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

        Vector2 m = GetMousePosition();

        if (!CheckCollisionPointRec(m, recUser))
            usernameEditMode = false;

        if (!CheckCollisionPointRec(m, recPass))
            passwordEditMode = false;
    }

    // =========================
    // ESTILO TEXTBOX
    // =========================
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL,    0xFFFFFF80);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL,    0x000000FF);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL,  0x00000000);

    GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED,   0xFFFFFFCC);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED,   0x000000FF);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, 0x4444FFFF);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

    // =========================
    // USUARIO
    // =========================
    if (GuiTextBox(recUser, username, 32, usernameEditMode)) {
        usernameEditMode = !usernameEditMode;
    }

    // =========================
    // CONTRASEÑA
    // =========================
    if (GuiTextBox(recPass, password, 32, passwordEditMode)) {
        passwordEditMode = !passwordEditMode;
    }

    // =========================
    // ERROR LOGIN
    // =========================
    if (loginFailed) {

        DrawText(
            "Usuario o contrasena incorrectos",
            (int)panelX,
            (int)yError,
            15,
            RED
        );
    }

    // =========================
    // BOTON INVISIBLE
    // =========================
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,    0x00000000);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL,  0x00000000);

    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED,   0x00000033);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0x00000000);

    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED,   0x00000066);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,    0x00000000);

    bool pressed = GuiButton(recBtn, "");

    GuiLoadStyleDefault();

    // =========================
    // BOTON LOGIN
    // =========================
    if (pressed) {
        loginFailed = false;
        return true;
    }

    return false;
}