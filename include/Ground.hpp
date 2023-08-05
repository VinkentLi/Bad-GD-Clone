#pragma once
#include "Global.hpp"

class Ground
{
private:
    constexpr static int GROUND_SIZE = 432;
    int squareCount;
    uint8_t r, g, b;
    SDL_Texture *groundTexture;
    SDL_Rect src;
    SDL_FPoint pos;

public:
    Ground() = default;
    Ground(uint8_t r, uint8_t g, uint8_t b);
    void setPos(SDL_FPoint pos);
    SDL_FPoint getPos();
    void resetPos();
    void update();
    void move(float distance, float delta);
    void render();
};

