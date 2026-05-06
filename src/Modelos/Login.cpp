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

bool Login::Draw() {
    int screenW = GetScreenWidth();   // 800
    int screenH = GetScreenHeight();  // 600

    // Fondo completo
    DrawTexturePro(
        background,
        Rectangle{ 0, 0, (float)background.width, (float)background.height },
        Rectangle{ 0, 0, (float)screenW, (float)screenH },
        Vector2{ 0, 0 },
        0.0f,
        WHITE
    );

    // Panel fijo para ventana 800x600
    // Ajusta estos valores mirando donde estan los campos en tu imagen
    float panelW = 260.0f;
    float panelX = (screenW - panelW) / 2.0f;  // centrado = 270

    // Coordenadas Y fijas segun la imagen
    float yUsuario    = 330.0f;   // donde dice "Usuario"
    float yContrasena = 385.0f;   // donde dice "Contrasena"
    float yBoton      = 430.0f;   // donde dice "INICIAR SESION"
    float yError      = 470.0f;   // debajo del boton

    Rectangle recUser = { panelX, yUsuario,    panelW, 38 };
    Rectangle recPass = { panelX, yContrasena, panelW, 38 };
    Rectangle recBtn  = { panelX, yBoton,      panelW, 42 };

    // Quitar foco al hacer click fuera
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = GetMousePosition();
        if (!CheckCollisionPointRec(m, recUser)) usernameEditMode = false;
        if (!CheckCollisionPointRec(m, recPass)) passwordEditMode = false;
    }

    // Estilo de los campos: fondo semitransparente, texto visible
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL,   0xFFFFFF80);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL,   0x000000FF);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, 0x00000000);
    GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED,  0xFFFFFFCC);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED,  0x000000FF);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED,0x4444FFFF);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

    // Campo Usuario
    if (GuiTextBox(recUser, username, 32, usernameEditMode))
        usernameEditMode = !usernameEditMode;

    // Campo Contrasena
    if (GuiTextBox(recPass, password, 32, passwordEditMode))
        passwordEditMode = !passwordEditMode;

    // Mensaje de error
    if (loginFailed) {
        DrawText("Usuario o contrasena incorrectos",
                 (int)panelX, (int)yError, 15, RED);
    }

    // Boton invisible sobre "INICIAR SESION"
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,   0x00000000);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x00000000);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED,  0x00000033);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED,0x00000000);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED,  0x00000066);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,   0x00000000);

    bool pressed = GuiButton(recBtn, "");
    GuiLoadStyleDefault();

    // Validacion
    if (pressed) {
        if (strcmp(username, "Prueba") == 0 && strcmp(password, "123") == 0) {
            loginFailed = false;
            return true;
        } else {
            loginFailed = true;
        }
    }

    return false;
}