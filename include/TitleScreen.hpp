#pragma once
#include "GameState.hpp"
#include <SDL.h>

class Game;

class TitleScreen : public GameState {
public:
    void init(Game *game) override;
    void destroy() override;
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render() override;
    inline static TitleScreen *get() { return &m_TitleScreen; }

private:
    static TitleScreen m_TitleScreen;
    SDL_Rect m_TitleDST;
    SDL_Rect m_TitlePlayDST;
    SDL_Texture *m_TitleTexture;
    SDL_Texture *m_TitlePlay;
    bool m_IsMouseHeld;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;
};
