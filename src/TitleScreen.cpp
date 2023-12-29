#include "TitleScreen.hpp"

TitleScreen::TitleScreen() {
    m_title_texture = IMG_LoadTexture(renderer, "res/gfx/title.png");

    if (m_title_texture == NULL) {
        std::cerr << "Failed to load title.png! " << SDL_GetError() << std::endl;
    }

    m_title_play = IMG_LoadTexture(renderer, "res/gfx/titlePlay.png");

    if (m_title_play == NULL) {
        std::cerr << "Failed to load titlePlay.png! " << SDL_GetError() << std::endl;
    }

    m_title_dst = {WIDTH / 2 - (1699*3/4) / 2, 100, 1699*3/4, 206*3/4}; // lol
    m_title_play_dst = {WIDTH/2 - 415/2*3/4, HEIGHT/2 - 415/2*3/4 - 40, 415*3/4, 415*3/4}; // super readable btw
    m_mouse_held = false;
}

void TitleScreen::update(int &game_state, SDL_Point *mouse_pos, bool mouseHeld) {
    if (m_mouse_held && !mouseHeld) {
        SDL_Rect scaled_title_play;
        float w_scale = SCREEN_WIDTH / (float) WIDTH;
        float h_scale = SCREEN_HEIGHT / (float) HEIGHT;
        scaled_title_play.x = m_title_play_dst.x * w_scale;
        scaled_title_play.y = m_title_play_dst.y * h_scale;
        scaled_title_play.w = m_title_play_dst.w * w_scale;
        scaled_title_play.h = m_title_play_dst.h * h_scale;
        
        if (SDL_PointInRect(mouse_pos, &scaled_title_play)) {
            game_state = LEVEL_SELECT;
        }
    }

    this->m_mouse_held = mouseHeld;
    this->m_mouse_pos = *mouse_pos;
}

void TitleScreen::render() {
    SDL_RenderCopy(renderer, m_title_texture, NULL, &m_title_dst);
    SDL_RenderCopy(renderer, m_title_play, NULL, &m_title_play_dst);
}
