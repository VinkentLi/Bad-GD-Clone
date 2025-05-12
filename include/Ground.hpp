#pragma once
#include "Global.hpp"

class Ground
{
private:
    constexpr static int GROUND_SIZE = 432;
    int m_SquareCount;
    uint8_t m_Red, m_Green, m_Blue;
    SDL_Texture *m_GroundTexture;
    SDL_Rect m_Source;
    SDL_FPoint m_Pos;
    bool m_RenderOnTop;

public:
    Ground() = default;
    Ground(uint8_t r, uint8_t g, uint8_t b);
    void setPos(SDL_FPoint pos);
    SDL_FPoint getPos();
    void resetPos();
    void update();
    void move(float distance, float delta);
    void render();
    void setOnTop(bool value);
};

