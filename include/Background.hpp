#pragma once
#include "Global.hpp"

class Background
{
private:
    int bgCount;
    float r, g, b;
    uint8_t targetR, targetG, targetB;
    SDL_Texture *bgTexture, *emptyBG;
    SDL_FPoint pos;
    bool moving, fading;
    float fadeTime = 0;

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
