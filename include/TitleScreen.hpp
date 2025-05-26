#pragma once
#include "GameStates.hpp"
#include <SDL.h>

class Game;

class TitleScreen {
public:
    TitleScreen(Game *game);
    void update(int &gameState, SDL_Point *mousePos, bool isMouseHeld);
    void render();

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    SDL_Rect m_TitleDST, m_TitlePlayDST;
    SDL_Texture *m_TitleTexture, *m_TitlePlay;
    SDL_Point m_MousePos;
    bool m_IsMouseHeld;
};
