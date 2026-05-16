#ifndef DODLE_JUMP_LOGIN_H
#define DODLE_JUMP_LOGIN_H

#include "raylib.h"
#include <string>
#include <vector>

class Login {
private:
    // Background and pixel-art UI textures.
    Texture2D background;
    Texture2D textboxTexture;
    Texture2D buttonNormalTexture;
    Texture2D buttonHoverTexture;

    // Retro UI font.
    Font uiFont;
    bool uiFontLoaded;

    // Animated Marco frames for the login screen.
    std::vector<Texture2D> loginFrames;
    int loginFrameIndex;
    float loginFrameTimer;
    float loginFrameDuration;

    // Input buffers.
    char username[32];
    char password[32];

    // Input focus flags.
    bool usernameEditMode;
    bool passwordEditMode;

    // Login error flag.
    bool loginFailed;

public:
    Login();
    ~Login();

    void Init();

    bool Draw();

    std::string GetUsername() const;
    std::string GetPassword() const;

    void SetLoginFailed(bool value);
};

#endif