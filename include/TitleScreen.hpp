#pragma once
#include "GameStates.hpp"
#include "Global.hpp"

class TitleScreen
{
private:
    SDL_Rect titleSRC, titlePlaySRC, titleDST, titlePlayDST;
    SDL_Texture *titleTexture, *titlePlay;
    SDL_Point mousePos;
    bool mouseHeld;

public:
    TitleScreen() = default;
    TitleScreen(SDL_Rect titleSRC, SDL_Rect titlePlaySRC);
    void update(int &gameState, SDL_Point *mousePos, bool mouseHeld);
    void render();
};
