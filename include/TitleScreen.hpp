#pragma once
#include "GameStates.hpp"
#include <SDL.h>

class Game;

class TitleScreen {
public:
    TitleScreen(Game *game);
    void update(int &gameState);
    void render();

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    SDL_Rect m_TitleDST, m_TitlePlayDST;
    SDL_Texture *m_TitleTexture, *m_TitlePlay;
    bool m_IsMouseHeld;
};
