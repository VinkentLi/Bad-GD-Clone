#pragma once
#include "Global.hpp"

class Background {
private:
    int m_bg_count;
    float m_red, m_green, m_blue;
    uint8_t m_target_red, m_target_green, m_target_blue;
    SDL_Texture *m_bg_texture, *m_empty_bg;
    SDL_FPoint m_pos;
    bool m_moving, m_fading;
    float m_fade_time = 0;

public:
    Background() = default;
    Background(uint8_t r, uint8_t g, uint8_t b);
    void fade(uint8_t r, uint8_t g, uint8_t b, float time);
    SDL_FPoint get_pos();
    void reset_pos();
    void set_moving(bool value);
    void update(float delta);
    void render(int game_state);
};
