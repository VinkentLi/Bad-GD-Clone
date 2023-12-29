#include "Global.hpp"
#include "Background.hpp"
#include "GameStates.hpp"

Background::Background(uint8_t r, uint8_t g, uint8_t b) : m_red(r), m_green(g), m_blue(b) {
    m_bg_texture = IMG_LoadTexture(renderer, "res/gfx/background.png");
    m_empty_bg = IMG_LoadTexture(renderer, "res/gfx/emptyBG.png");
    SDL_SetTextureColorMod(m_bg_texture, r, g, b);
    SDL_SetTextureColorMod(m_empty_bg, r, g, b);
    m_bg_count = WIDTH / BACKGROUND_SIZE + 2;
    m_pos = {0, 0};
    m_moving = false;
    m_fading = false;
    m_target_red = r;
    m_target_green = g;
    m_target_blue = b;
}

void Background::fade(uint8_t r, uint8_t g, uint8_t b, float time) {
    m_target_red = r;
    m_target_green = g;
    m_target_blue = b;
    m_fade_time = time * 60;
    m_fading = true;
}

SDL_FPoint Background::get_pos() {
    return m_pos;
}

void Background::reset_pos() {
    m_pos.x = 0;   
}

void Background::set_moving(bool value) {
    m_moving = value;
}

void Background::update(float delta) {
    if (m_moving) {
        m_pos.x -= 2 * delta;

        if (m_pos.x < -BACKGROUND_SIZE) {
            m_pos.x += BACKGROUND_SIZE;
        }
    }

    if (m_fading) {
        m_fade_time -= delta;
        m_red += delta * (m_target_red - m_red) / m_fade_time;
        m_green += delta * (m_target_green - m_green) / m_fade_time;
        m_blue += delta * (m_target_blue - m_blue) / m_fade_time;

        if (m_fade_time <= 0) {
            m_red = m_target_red;
            m_green = m_target_green;
            m_blue = m_target_blue;
            m_fade_time = 0;
            m_fading = false;
        }

        SDL_SetTextureColorMod(m_bg_texture, m_red, m_green, m_blue);
    }
}

void Background::render(int game_state) {
    for (int i = 0; i < m_bg_count; i++) {
        SDL_FRect dst = {m_pos.x + i * BACKGROUND_SIZE, m_pos.y - (BACKGROUND_SIZE - HEIGHT) - camera_pos.y/10, BACKGROUND_SIZE, BACKGROUND_SIZE};

        if (game_state == LEVEL_SELECT) {
            dst.y += (BACKGROUND_SIZE - HEIGHT);
            SDL_RenderCopyExF(renderer, m_empty_bg, NULL, &dst, 0, NULL, SDL_FLIP_VERTICAL);
            continue;
        }


        if (SDL_RenderCopyF(renderer, m_bg_texture, NULL, &dst) != 0) {
            std::cerr << "Failed to render background! " << SDL_GetError() << std::endl;
        }
    }
}
