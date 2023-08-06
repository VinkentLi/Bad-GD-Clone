#pragma once
#include "GameStates.hpp"
#include "Global.hpp"

class TitleScreen
{
private:
    SDL_Rect titleDST, titlePlayDST;
    SDL_Texture *titleTexture, *titlePlay;
    SDL_Point mousePos;
    bool mouseHeld;

public:
    TitleScreen();
    void update(int &gameState, SDL_Point *mousePos, bool mouseHeld);
    void render();
};
