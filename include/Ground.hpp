#pragma once
#include <SDL.h>
#include <cstdint>

class Game;

class Ground {
public:
    Ground() = default;
    Ground(Game *game, uint8_t r, uint8_t g, uint8_t b);
    void setPos(SDL_FPoint pos);
    SDL_FPoint getPosition();
    void resetPosition();
    void update();
    void move(float distance, float delta);
    void render();
    void setOnTop(bool value);

private:
    constexpr static int GROUND_SIZE = 432;
    Game *m_Game;
    int m_SquareCount;
    uint8_t m_Red;
    uint8_t m_Green;
    uint8_t m_Blue;
    SDL_Texture *m_GroundTexture;
    SDL_Rect m_Source;
    SDL_FPoint m_Position;
    SDL_Renderer *m_Renderer;
    bool m_ShouldRenderOnTop;
};

