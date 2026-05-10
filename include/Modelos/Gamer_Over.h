#ifndef GAMER_OVER_H
#define GAMER_OVER_H

#include "raylib.h"

// Estados del Game Over
enum GameOverResult {
    GO_NONE = 0,
    GO_RETRY,
    GO_MENU,
    GO_EXIT
};

class GamerOver {
private:
    Texture2D background;

    Rectangle btnRetry;
    Rectangle btnMenu;
    Rectangle btnExit;

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