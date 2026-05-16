#include "Modelos/Gamer_Over.h"
#include <string>
#include <vector>

// Game Over screen with pixel-art background, buttons, and animated character.

namespace {
    const float kDesignWidth = 1200.0f;
    const float kDesignHeight = 896.0f;

    const Rectangle kRetryBase = { 450.0f, 522.6f, 300.0f, 74.7f };
    const Rectangle kMenuBase = { 450.0f, 627.2f, 300.0f, 74.7f };
    const Rectangle kExitBase = { 450.0f, 731.9f, 300.0f, 74.7f };
}

GamerOver::GamerOver() {

    // Default state and button rectangles.
    result = GO_NONE;

    btnRetry = { 0, 0, 0, 0 };
    btnMenu  = { 0, 0, 0, 0 };
    btnExit  = { 0, 0, 0, 0 };

    retryTexture = Texture2D{};
    menuTexture = Texture2D{};
    exitTexture = Texture2D{};
    uiFont = Font{};
    uiFontLoaded = false;

    // Init animated Marco state.
    playerFrames.clear();
    playerFrameIndex = 0;
    playerFrameTimer = 0.0f;
    playerFrameDuration = 0.08f;
}

GamerOver::~GamerOver() {

    // Release textures and font allocated for Game Over.
    UnloadTexture(background);
    if (retryTexture.id != 0) UnloadTexture(retryTexture);
    if (menuTexture.id != 0) UnloadTexture(menuTexture);
    if (exitTexture.id != 0) UnloadTexture(exitTexture);
    if (uiFontLoaded) UnloadFont(uiFont);

    for (Texture2D& frame : playerFrames) {
        if (frame.id != 0) UnloadTexture(frame);
    }
}

void GamerOver::Load(const char* imagePath) {

    // Resolve asset paths for dev/exe layouts and load textures.
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

    std::string backgroundPath = resolveAsset({
        "Imagenes/GameOver.png",
        "Imagenes/GamerOver.png"
    });

    // Background texture.
    background = LoadTexture(backgroundPath.c_str());
    SetTextureFilter(background, TEXTURE_FILTER_POINT);

    // Pixel-art button textures.
    std::string retryPath = resolveAsset({
        "Imagenes/UI/reintentar.png",
        "Imagenes/Ui/Reintentar.png",
        "Imagenes/Ui/reintentar.png"
    });

    std::string menuPath = resolveAsset({
        "Imagenes/UI/menu.png",
        "Imagenes/Ui/Menu.png",
        "Imagenes/Ui/menu.png"
    });

    std::string exitPath = resolveAsset({
        "Imagenes/UI/salir.png",
        "Imagenes/Ui/SALIR.png",
        "Imagenes/Ui/salir.png"
    });

    // Retro font (reserved for consistent UI styling).
    std::string fontPath = resolveAsset({
        "Imagenes/UI/pixel_font.ttf",
        "Imagenes/Ui/pixel_font.ttf",
        "Imagenes/Ui/PressStart2P-Regular.ttf"
    });

    retryTexture = LoadTexture(retryPath.c_str());
    menuTexture = LoadTexture(menuPath.c_str());
    exitTexture = LoadTexture(exitPath.c_str());

    if (retryTexture.id != 0) SetTextureFilter(retryTexture, TEXTURE_FILTER_POINT);
    if (menuTexture.id != 0) SetTextureFilter(menuTexture, TEXTURE_FILTER_POINT);
    if (exitTexture.id != 0) SetTextureFilter(exitTexture, TEXTURE_FILTER_POINT);

    // Load Marco frames for animation in Game Over.
    playerFrames.clear();
    int loadedFrames = 0;
    for (int i = 1; i <= 13; ++i) {
        std::string frameNameUpper = "Imagenes/marco_salto" + std::to_string(i) + ".PNG";
        std::string frameNameLower = "Imagenes/marco_salto" + std::to_string(i) + ".png";
        std::string framePath = resolveAsset({ frameNameUpper, frameNameLower });
        if (FileExists(framePath.c_str())) {
            Texture2D frame = LoadTexture(framePath.c_str());
            SetTextureFilter(frame, TEXTURE_FILTER_POINT);
            playerFrames.push_back(frame);
            loadedFrames++;
        } else {
            TraceLog(LOG_WARNING, "GameOver: no se encontro frame %d (%s)", i, frameNameUpper.c_str());
        }
    }

    if (loadedFrames == 0) {
        std::string fallbackPath = resolveAsset({
            "Imagenes/marco_salto.png",
            "Imagenes/marco_salto.PNG"
        });
        if (FileExists(fallbackPath.c_str())) {
            Texture2D frame = LoadTexture(fallbackPath.c_str());
            SetTextureFilter(frame, TEXTURE_FILTER_POINT);
            playerFrames.push_back(frame);
            loadedFrames = 1;
            TraceLog(LOG_INFO, "GameOver: usando frame fallback %s", fallbackPath.c_str());
        }
    }

    TraceLog(LOG_INFO, "GameOver: frames cargados %d", loadedFrames);
    playerFrameIndex = 0;
    playerFrameTimer = 0.0f;

    if (FileExists(fontPath.c_str())) {
        uiFont = LoadFontEx(fontPath.c_str(), 18, nullptr, 0);
        SetTextureFilter(uiFont.texture, TEXTURE_FILTER_POINT);
        uiFontLoaded = true;
    } else {
        uiFont = GetFontDefault();
        uiFontLoaded = false;
    }

    UpdateLayout();
}

void GamerOver::UpdateLayout() {

    // Scale button hitboxes to current resolution.
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

    // Advance animation frames.
    playerFrameTimer += GetFrameTime();
    if (!playerFrames.empty() && playerFrameTimer >= playerFrameDuration) {
        playerFrameTimer = 0.0f;
        playerFrameIndex = (playerFrameIndex + 1) % (int)playerFrames.size();
    }

    Vector2 mouse = GetMousePosition();

    // Keep button click logic unchanged.
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

    // Draw background scaled to screen.
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

    // Draw animated Marco at mid-left.
    if (!playerFrames.empty()) {
        float scale = (float)GetScreenHeight() / kDesignHeight;
        float targetHeight = 32.0f * scale;
        if (targetHeight < 32.0f) targetHeight = 32.0f;
        Texture2D frame = playerFrames[playerFrameIndex];
        float aspect = (frame.height > 0) ? ((float)frame.width / (float)frame.height) : 1.0f;
        float targetWidth = targetHeight * aspect;

        float x = 64.0f * scale;
        if (x < 64.0f) x = 64.0f;
        float y = (GetScreenHeight() - targetHeight) * 0.5f;

        DrawTexturePro(
            frame,
            Rectangle{ 0.0f, 0.0f, (float)frame.width, (float)frame.height },
            Rectangle{ x, y, targetWidth, targetHeight },
            Vector2{ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }

    Vector2 mouse = GetMousePosition();
    bool hoverRetry = CheckCollisionPointRec(mouse, btnRetry);
    bool hoverMenu = CheckCollisionPointRec(mouse, btnMenu);
    bool hoverExit = CheckCollisionPointRec(mouse, btnExit);

    // Draw pixel-art buttons with subtle hover lift.
    auto DrawButton = [&](Texture2D tex, Rectangle dest, bool hovered) {
        if (tex.id == 0) return;
        float yOffset = hovered ? -3.0f : 0.0f;
        Rectangle drawRect = { dest.x, dest.y + yOffset, dest.width, dest.height };

        DrawTexturePro(
            tex,
            Rectangle{ 0.0f, 0.0f, (float)tex.width, (float)tex.height },
            drawRect,
            Vector2{ 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    };

    DrawButton(retryTexture, btnRetry, hoverRetry);
    DrawButton(menuTexture, btnMenu, hoverMenu);
    DrawButton(exitTexture, btnExit, hoverExit);
}

GameOverResult GamerOver::GetResult() {

    return result;
}

void GamerOver::Reset() {

    result = GO_NONE;
}