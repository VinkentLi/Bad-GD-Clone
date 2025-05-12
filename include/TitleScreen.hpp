#pragma once
#include "GameStates.hpp"
#include "Global.hpp"

class TitleScreen
{
private:
    SDL_Rect m_TitleDST, m_TitlePlayDST;
    SDL_Texture *m_TitleTexture, *m_TitlePlay;
    SDL_Point m_MousePos;
    bool m_MouseHeld;

public:
    TitleScreen();
    void update(int &gameState, SDL_Point *mousePos, bool mouseHeld);
    void render();
};
