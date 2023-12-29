#pragma once
#include "Global.hpp"

class Ground {
private:
    constexpr static int m_GROUND_SIZE = 432;
    int m_square_count;
    uint8_t m_red, m_green, m_blue;
    SDL_Texture *m_ground_texture;
    SDL_Rect m_src;
    SDL_FPoint m_pos;
    bool m_render_on_top;

public:
    Ground() = default;
    Ground(uint8_t r, uint8_t g, uint8_t b);
    void set_pos(SDL_FPoint pos);
    SDL_FPoint get_pos();
    void reset_pos();
    void update();
    void move(float distance, float delta);
    void render();
    void set_on_top(bool value);
};

