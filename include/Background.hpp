#pragma once
#include "Global.hpp"

class Background
{
private:
    int m_BGCount;
    float m_Red, m_Green, m_Blue;
    uint8_t m_TargetRed, m_TargetGreen, m_TargetBlue;
    SDL_Texture *m_BGTexture, *m_EmptyBG;
    SDL_FPoint m_Pos;
    bool m_Moving, m_Fading;
    float m_FadeTime = 0;

public:
    Background() = default;
    Background(uint8_t r, uint8_t g, uint8_t b);
    void fade(uint8_t r, uint8_t g, uint8_t b, float time);
    SDL_FPoint getPos();
    void resetPos();
    void setMoving(bool value);
    void update(float delta);
    void render(int gameState);
};
