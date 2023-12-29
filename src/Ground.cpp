#include "Ground.hpp"

Ground::Ground(uint8_t r, uint8_t g, uint8_t b) : m_red(r), m_green(g), m_blue(b) {
    m_ground_texture = IMG_LoadTexture(renderer, "res/gfx/ground.png");
    SDL_SetTextureColorMod(m_ground_texture, r, g, b);
    m_square_count = WIDTH / m_GROUND_SIZE + 2;
    m_src = {0, 0, m_GROUND_SIZE, m_GROUND_SIZE};
    m_pos = {0, HEIGHT - 300};
    m_render_on_top = false;
}

void Ground::set_pos(SDL_FPoint pos) {
    m_pos = pos;
}

SDL_FPoint Ground::get_pos() {
    return m_pos;
}

void Ground::move(float distance, float delta) {
    m_pos.x += distance * delta;
}

void Ground::reset_pos() {
    m_pos.x = 0;
}

void Ground::update() {
    if (m_pos.x < -m_GROUND_SIZE) {
        m_pos.x += m_GROUND_SIZE;
    }
}

void Ground::render() {
    for (int i = 0; i < m_square_count; i++) {
        SDL_FRect dst = {m_pos.x + i * m_GROUND_SIZE - camera_pos.x, m_pos.y - camera_pos.y, m_GROUND_SIZE, m_GROUND_SIZE};

        while (dst.x + m_GROUND_SIZE < 0) {
            dst.x += m_square_count * m_GROUND_SIZE;
        }

        SDL_RenderCopyF(renderer, m_ground_texture, &m_src, &dst);

        if (m_render_on_top) {
            SDL_FRect new_dst = dst;
            new_dst.y = HEIGHT - dst.y - dst.h;

            SDL_RenderCopyExF(renderer, m_ground_texture, &m_src, &new_dst, 0.0, NULL, SDL_FLIP_VERTICAL);
        }
    }
}

void Ground::set_on_top(bool value) {
    m_render_on_top = value;
}
