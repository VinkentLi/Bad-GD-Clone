#pragma once
#include <SDL.h>

class Game;

class Background
{
public:
    Background() = default;
    void init(Game *game, uint8_t r, uint8_t g, uint8_t b);
    void destroy();
    void fade(uint8_t r, uint8_t g, uint8_t b, float time);
    SDL_FPoint getPosition();
    void resetPosition();
    void setMoving(bool value);
    void update(float delta);
    void render(int gameState);

private:
    Game *m_Game;
    int m_BGCount;
    float m_Red;
    float m_Green;
    float m_Blue;
    uint8_t m_TargetRed;
    uint8_t m_TargetGreen;
    uint8_t m_TargetBlue;
    SDL_Texture *m_BGTexture;
    SDL_Texture *m_EmptyBG;
    SDL_FPoint m_Position;
    SDL_Renderer *m_Renderer;
    bool m_IsMoving;
    bool m_IsFading;
    float m_FadeTime = 0;
};
