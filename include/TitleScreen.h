#pragma once
#include "GameState.h"
#include "Button.h"
#include <SDL.h>

class Game;

class TitleScreen : public GameState {
public:
    void init(Game *game) override;
    void destroy() override;
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render() const override;
    inline static TitleScreen &get() { return m_TitleScreen; }

private:
    static TitleScreen m_TitleScreen;
    SDL_Rect m_TitleRect;
    SDL_Texture *m_TitleTexture;
    SDL_Texture *m_TitlePlayTexture;
    Button m_TitlePlay;
    bool m_IsMouseHeld;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;
};
