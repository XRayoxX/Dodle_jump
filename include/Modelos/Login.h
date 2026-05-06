#ifndef DODLE_JUMP_LOGIN_H
#define DODLE_JUMP_LOGIN_H

#include "raylib.h"

class Login {
private:
    Texture2D background;
    char username[32];
    char password[32];
    bool usernameEditMode;
    bool passwordEditMode;
    bool loginFailed;

public:
    Login();
    ~Login();
    void Init();
    bool Draw();
};

#endif