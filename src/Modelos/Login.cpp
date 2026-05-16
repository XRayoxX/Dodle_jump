#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "../../include/Modelos/Login.h"
#include <string.h>
#include <vector>

// Login UI with pixel-art assets, custom font, and animated character.

Login::Login() {
    username[0] = '\0';
    password[0] = '\0';

    usernameEditMode = false;
    passwordEditMode = false;

    loginFailed = false;

    textboxTexture = Texture2D{};
    buttonNormalTexture = Texture2D{};
    buttonHoverTexture = Texture2D{};
    uiFont = Font{};
    uiFontLoaded = false;

    // Init animated login character state.
    loginFrames.clear();
    loginFrameIndex = 0;
    loginFrameTimer = 0.0f;
    loginFrameDuration = 0.10f;
}

Login::~Login() {
    // Release textures and font allocated for the login screen.
    UnloadTexture(background);
    if (textboxTexture.id != 0) UnloadTexture(textboxTexture);
    if (buttonNormalTexture.id != 0) UnloadTexture(buttonNormalTexture);
    if (buttonHoverTexture.id != 0) UnloadTexture(buttonHoverTexture);
    if (uiFontLoaded) UnloadFont(uiFont);

    for (Texture2D& frame : loginFrames) {
        if (frame.id != 0) UnloadTexture(frame);
    }
}

void Login::Init() {
    // Resolve asset paths for dev/exe layouts and load textures/fonts.
    std::string baseDir = GetApplicationDirectory();
    if (!baseDir.empty() && baseDir.back() != '/' && baseDir.back() != '\\') {
        baseDir += "/";
    }

    auto resolveAsset = [&](const std::vector<std::string>& relCandidates) {
        for (const std::string& rel : relCandidates) {
            std::string pathDev = baseDir + "../" + rel;
            std::string pathExe = baseDir + rel;

            if (FileExists(pathDev.c_str())) return pathDev;
            if (FileExists(pathExe.c_str())) return pathExe;
        }
        return baseDir + "../" + relCandidates.front();
    };

    std::string pathDev = baseDir + "../Imagenes/Login.png";
    std::string pathExe = baseDir + "Imagenes/Login.png";

    const char* loginPath = FileExists(pathDev.c_str())
        ? pathDev.c_str()
        : (FileExists(pathExe.c_str()) ? pathExe.c_str() : pathDev.c_str());

    TraceLog(LOG_INFO, "Login: usando %s", loginPath);

    background = LoadTexture(loginPath);
    SetTextureFilter(background, TEXTURE_FILTER_POINT);

    std::string textboxPath = resolveAsset({
        "Imagenes/UI/textbox.png",
        "Imagenes/Ui/textbox.png",
        "Imagenes/Ui/Textbox.png"
    });

    std::string buttonNormalPath = resolveAsset({
        "Imagenes/UI/iniciar_1.png",
        "Imagenes/Ui/iniciar_1.png",
        "Imagenes/Ui/Iniciar_1.png",
        "Imagenes/Ui/Iniciar1.png",
        "Imagenes/Ui/Inicar1.png"
    });

    std::string buttonHoverPath = resolveAsset({
        "Imagenes/UI/iniciar_2.png",
        "Imagenes/Ui/iniciar_2.png",
        "Imagenes/Ui/Iniciar_2.png",
        "Imagenes/Ui/Iniciar2.png"
    });

    std::string fontPath = resolveAsset({
        "Imagenes/UI/pixel_font.ttf",
        "Imagenes/Ui/pixel_font.ttf",
        "Imagenes/Ui/PressStart2P-Regular.ttf"
    });

    // Load pixel-art UI textures.
    textboxTexture = LoadTexture(textboxPath.c_str());
    buttonNormalTexture = LoadTexture(buttonNormalPath.c_str());
    buttonHoverTexture = LoadTexture(buttonHoverPath.c_str());

    if (textboxTexture.id != 0) SetTextureFilter(textboxTexture, TEXTURE_FILTER_POINT);
    if (buttonNormalTexture.id != 0) SetTextureFilter(buttonNormalTexture, TEXTURE_FILTER_POINT);
    if (buttonHoverTexture.id != 0) SetTextureFilter(buttonHoverTexture, TEXTURE_FILTER_POINT);

    // Load retro font if available.
    if (FileExists(fontPath.c_str())) {
        uiFont = LoadFontEx(fontPath.c_str(), 18, nullptr, 0);
        SetTextureFilter(uiFont.texture, TEXTURE_FILTER_POINT);
        uiFontLoaded = true;
    } else {
        uiFont = GetFontDefault();
        uiFontLoaded = false;
    }

    // Load Marco animation frames (marco_salto10..13).
    loginFrames.clear();
    for (int i = 10; i <= 13; ++i) {
        std::string frameNameUpper = "Imagenes/marco_salto" + std::to_string(i) + ".PNG";
        std::string frameNameLower = "Imagenes/marco_salto" + std::to_string(i) + ".png";
        std::string framePath = resolveAsset({ frameNameUpper, frameNameLower });
        if (FileExists(framePath.c_str())) {
            Texture2D frame = LoadTexture(framePath.c_str());
            SetTextureFilter(frame, TEXTURE_FILTER_POINT);
            loginFrames.push_back(frame);
        }
    }
    TraceLog(LOG_INFO, "Login: frames cargados %d", (int)loginFrames.size());
    loginFrameIndex = 0;
    loginFrameTimer = 0.0f;
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

    // Background scaled to current screen.
    DrawTexturePro(
        background,
        Rectangle{ 0, 0, (float)background.width, (float)background.height },
        Rectangle{ 0, 0, (float)screenW, (float)screenH },
        Vector2{ 0, 0 },
        0.0f,
        WHITE
    );

    // Animated Marco on the left platform.
    if (!loginFrames.empty()) {
        loginFrameTimer += GetFrameTime();
        if (loginFrameTimer >= loginFrameDuration) {
            loginFrameTimer = 0.0f;
            loginFrameIndex = (loginFrameIndex + 1) % (int)loginFrames.size();
        }

        float scale = (float)screenH / 896.0f;
        Texture2D frame = loginFrames[loginFrameIndex];
        float scaleFactor = 3.6f;
        float targetHeight = frame.height * scaleFactor * scale;
        float aspect = (frame.height > 0) ? ((float)frame.width / (float)frame.height) : 1.0f;
        float targetWidth = targetHeight * aspect;

        float baseX = 170.0f * scale;
        float baseFootY = 580.0f * scale;

        float x = baseX;
        float y = baseFootY - targetHeight;

        DrawTexturePro(
            frame,
            Rectangle{ 0.0f, 0.0f, (float)frame.width, (float)frame.height },
            Rectangle{ x, y, targetWidth, targetHeight },
            Vector2{ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }

    // Login panel layout and input areas.
    float textboxW = (textboxTexture.id != 0) ? (float)textboxTexture.width : 260.0f;
    float textboxH = (textboxTexture.id != 0) ? (float)textboxTexture.height : 38.0f;
    float buttonW = (buttonNormalTexture.id != 0) ? (float)buttonNormalTexture.width : 260.0f;
    float buttonH = (buttonNormalTexture.id != 0) ? (float)buttonNormalTexture.height : 42.0f;

    const float buttonScale = 0.6f;
    float buttonDrawW = buttonW * buttonScale;
    float buttonDrawH = buttonH * buttonScale;

    float panelW = (textboxW > buttonDrawW) ? textboxW : buttonDrawW;
    float panelH = 240.0f;
    float panelX = (screenW - panelW) / 2.0f;
    float panelY = 360.0f;

    float gapLabelToBox = 22.0f;
    float gapBetweenFields = 78.0f;
    float gapBetweenPassAndButton = 86.0f;

    float yUsuario    = panelY;
    float yContrasena = panelY + gapBetweenFields;
    float yBoton      = yContrasena + gapBetweenPassAndButton;
    float yError      = yBoton + buttonDrawH + 18.0f;

    float yLabelUser = yUsuario - gapLabelToBox;
    float yLabelPass = yContrasena - gapLabelToBox;

    Rectangle recUser = { panelX + (panelW - textboxW) / 2.0f, yUsuario,    textboxW, textboxH };
    Rectangle recPass = { panelX + (panelW - textboxW) / 2.0f, yContrasena, textboxW, textboxH };
    Rectangle recBtn  = { panelX + (panelW - buttonDrawW) / 2.0f, yBoton,   buttonDrawW,  buttonDrawH };

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
    // ESTILO INVISIBLE (RayGUI)
    // =========================
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL,    0x00000000);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL,    0x00000000);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL,  0x00000000);

    GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED,   0x00000000);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED,   0x00000000);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, 0x00000000);

    GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED,   0x00000000);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED,   0x00000000);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, 0x00000000);

    GuiSetStyle(TEXTBOX, BASE_COLOR_DISABLED,  0x00000000);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_DISABLED,  0x00000000);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_DISABLED,0x00000000);
    GuiSetStyle(TEXTBOX, BORDER_WIDTH,         0);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);

    const float fontSize = 18.0f;
    const float letterSpacing = 1.0f;
    Color labelColor = { 74, 46, 31, 255 };
    Color textColor  = { 74, 46, 31, 255 };

    Color outlineColor = { 58, 36, 24, 255 };
    Color fillColor = { 243, 223, 162, 255 };

    // Retro outlined text helper.
    auto DrawOutlinedText = [&](const char* text, Vector2 pos) {
        DrawTextEx(uiFont, text, Vector2{ pos.x - 1.0f, pos.y }, fontSize, letterSpacing, outlineColor);
        DrawTextEx(uiFont, text, Vector2{ pos.x + 1.0f, pos.y }, fontSize, letterSpacing, outlineColor);
        DrawTextEx(uiFont, text, Vector2{ pos.x, pos.y - 1.0f }, fontSize, letterSpacing, outlineColor);
        DrawTextEx(uiFont, text, Vector2{ pos.x, pos.y + 1.0f }, fontSize, letterSpacing, outlineColor);
        DrawTextEx(uiFont, text, pos, fontSize, letterSpacing, fillColor);
    };

    // Blinking cursor for active textbox.
    auto ShouldBlink = []() {
        return ((int)(GetTime() * 2.0)) % 2 == 0;
    };

    const char* cursorGlyph = "_";

    DrawOutlinedText("Usuario", Vector2{ recUser.x, yLabelUser });

    // =========================
    // USUARIO
    // =========================
    if (GuiTextBox(recUser, username, 32, usernameEditMode)) {
        usernameEditMode = !usernameEditMode;
    }

    if (textboxTexture.id != 0) {
        DrawTextureV(textboxTexture, Vector2{ recUser.x, recUser.y }, WHITE);
    }

    Vector2 userTextPos = {
        recUser.x + 8.0f,
        recUser.y + (recUser.height - fontSize) / 2.0f
    };
    DrawOutlinedText(username, userTextPos);
    if (usernameEditMode && ShouldBlink()) {
        float userTextWidth = MeasureTextEx(uiFont, username, fontSize, letterSpacing).x;
        Vector2 cursorPos = { userTextPos.x + userTextWidth, userTextPos.y };
        DrawOutlinedText(cursorGlyph, cursorPos);
    }

    // =========================
    // CONTRASENA
    // =========================
    if (GuiTextBox(recPass, password, 32, passwordEditMode)) {
        passwordEditMode = !passwordEditMode;
    }

    if (textboxTexture.id != 0) {
        DrawTextureV(textboxTexture, Vector2{ recPass.x, recPass.y }, WHITE);
    }

    DrawOutlinedText("Contrasena", Vector2{ recPass.x, yLabelPass });

    Vector2 passTextPos = {
        recPass.x + 8.0f,
        recPass.y + (recPass.height - fontSize) / 2.0f
    };
    std::string maskedPassword(strlen(password), '*');
    DrawOutlinedText(maskedPassword.c_str(), passTextPos);
    if (passwordEditMode && ShouldBlink()) {
        float passTextWidth = MeasureTextEx(uiFont, maskedPassword.c_str(), fontSize, letterSpacing).x;
        Vector2 cursorPos = { passTextPos.x + passTextWidth, passTextPos.y };
        DrawOutlinedText(cursorGlyph, cursorPos);
    }

    // =========================
    // ERROR LOGIN
    // =========================
    if (loginFailed) {
        DrawTextEx(
            uiFont,
            "Usuario o contrasena incorrectos",
            Vector2{ recUser.x, yError },
            15.0f,
            letterSpacing,
            RED
        );
    }

    // =========================
    // BOTON INVISIBLE
    // =========================
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,    0xFFFFFF00);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL,  0xFFFFFF00);

    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED,   0xFFFFFF00);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0xFFFFFF00);

    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED,   0xFFFFFF00);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,    0xFFFFFF00);

    // Invisible RayGUI button to keep click logic; draw PNG above.
    bool pressed = GuiButton(recBtn, "");

    GuiLoadStyleDefault();

    Vector2 mousePos = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mousePos, recBtn);
    bool down = hovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    Texture2D buttonTexture = (hovered || down) ? buttonHoverTexture : buttonNormalTexture;
    if (buttonTexture.id != 0) {
        DrawTexturePro(
            buttonTexture,
            Rectangle{ 0.0f, 0.0f, (float)buttonTexture.width, (float)buttonTexture.height },
            Rectangle{ recBtn.x, recBtn.y, recBtn.width, recBtn.height },
            Vector2{ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }


    // =========================
    // BOTON LOGIN
    // =========================
    if (pressed) {
        loginFailed = false;
        return true;
    }


    return false;
}