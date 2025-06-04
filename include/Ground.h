#pragma once
#include <SDL.h>
#include <cstdint>

class Game;

class Ground {
public:
    Ground() = default;
    void init(Game *game, uint8_t r, uint8_t g, uint8_t b);
    void destroy();
    void fade(uint8_t r, uint8_t g, uint8_t b, float time);
    void setPosition(SDL_FPoint pos);
    SDL_FPoint getPosition();
    void resetPosition();
    void update(float delta);
    void move(float distance, float delta);
    void render();
    void setOnTop(bool value);

private:
    Game *m_Game;
    int m_GroundSize;
    int m_SquareCount;
    float m_Red;
    float m_Green;
    float m_Blue;
    uint8_t m_TargetRed;
    uint8_t m_TargetGreen;
    uint8_t m_TargetBlue;
    SDL_Texture *m_GroundTexture;
    SDL_Rect m_Source;
    SDL_FPoint m_Position;
    SDL_Renderer *m_Renderer;
    bool m_IsFading;
    bool m_ShouldRenderOnTop;
    float m_FadeTime;
};

