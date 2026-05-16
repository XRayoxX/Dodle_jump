#ifndef GAMER_OVER_H
#define GAMER_OVER_H

#include "raylib.h"
#include <vector>

// Estados del Game Over
enum GameOverResult {
    GO_NONE = 0,
    GO_RETRY,
    GO_MENU,
    GO_EXIT
};

class GamerOver {
private:
    // Background and pixel-art button textures.
    Texture2D background;
    Texture2D retryTexture;
    Texture2D menuTexture;
    Texture2D exitTexture;

    // Retro font (kept for consistency).
    Font uiFont;
    bool uiFontLoaded;

    // Animated Marco frames for Game Over corner.
    std::vector<Texture2D> playerFrames;
    int playerFrameIndex;
    float playerFrameTimer;
    float playerFrameDuration;

    // Button hitboxes.
    Rectangle btnRetry;
    Rectangle btnMenu;
    Rectangle btnExit;

    // Current selection result.
    GameOverResult result;

    void UpdateLayout();

public:
    GamerOver();
    ~GamerOver();

    void Load(const char* imagePath);
    void Update();
    void Draw();

    GameOverResult GetResult();
    void Reset();
};

#endif // GAMER_OVER_H