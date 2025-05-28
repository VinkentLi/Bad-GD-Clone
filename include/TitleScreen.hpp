#pragma once
#include "GameState.hpp"
#include <SDL.h>

class Game;

class TitleScreen {
public:
    TitleScreen(Game *game);
    ~TitleScreen();
    TitleScreen(const TitleScreen &) = delete;
    TitleScreen &operator=(const TitleScreen &) = delete;
    void update(float deltaTime);
    void render();

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    SDL_Rect m_TitleDST;
    SDL_Rect m_TitlePlayDST;
    SDL_Texture *m_TitleTexture;
    SDL_Texture *m_TitlePlay;
    bool m_IsMouseHeld;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;
};
