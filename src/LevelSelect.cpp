#include "LevelSelect.hpp"

LevelSelect::LevelSelect() {
    m_left_corner_dst = {0, HEIGHT - 282, 285, 282};
    m_right_corner_dst = {WIDTH - 285, HEIGHT - 282, 285, 282};
    m_top_dst = {WIDTH/2 - 1226/2*3/4, 0, 1226*3/4, 144*3/4};
    m_left_level_arrow_dst = {40, HEIGHT/2 - 238/2, 106, 238};
    m_right_level_arrow_dst = {WIDTH - 40 - 106, HEIGHT/2 - 238/2, 106, 238};
    m_title_arrow_dst = {40, 20, 124, 150};
    m_corner = IMG_LoadTexture(renderer, "res/gfx/selectCorner.png");
    m_top = IMG_LoadTexture(renderer, "res/gfx/top.png");
    m_level_arrow = IMG_LoadTexture(renderer, "res/gfx/levelArrow.png");
    m_title_arrow = IMG_LoadTexture(renderer, "res/gfx/toTitleScreen.png");
    m_mouse_pos = {0, 0};
    m_mouse_held = false;
    m_need_to_recall_playing_state_constructor = false;
    level_selected = 0;
    m_level_strings.push_back("Test level1");
    m_level_strings.push_back("Test Level2");
    m_font = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    m_font_outline = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    TTF_SetFontOutline(m_font_outline, 4);
    m_rect_with_level_name.w = 1000;
    m_rect_with_level_name.h = 300;
    m_rect_with_level_name.x = WIDTH / 2 - 1000/2;
    m_rect_with_level_name.y = HEIGHT / 2 - 350;
    m_play_sound = Mix_LoadWAV("res/sfx/playSound.ogg");
}

void LevelSelect::update(int &game_state, SDL_Point *mouse_pos, bool mouse_held) {
    if (m_mouse_held && !mouse_held) {  
        float w_scale = SCREEN_WIDTH / (float) WIDTH;
        float h_scale = SCREEN_HEIGHT / (float) HEIGHT;
        SDL_Rect scaled_title_arrow = m_title_arrow_dst, scaled_left = m_left_level_arrow_dst, scaled_right = m_right_level_arrow_dst, scaled_level_rect = m_rect_with_level_name;
        scaled_title_arrow.x *= w_scale;
        scaled_title_arrow.y *= h_scale;
        scaled_title_arrow.w *= w_scale;
        scaled_title_arrow.h *= h_scale;
        scaled_left.x *= w_scale;
        scaled_left.y *= h_scale;
        scaled_left.w *= w_scale;
        scaled_left.h *= h_scale;
        scaled_right.x *= w_scale;
        scaled_right.y *= h_scale;
        scaled_right.w *= w_scale;
        scaled_right.h *= h_scale;
        scaled_level_rect.x *= w_scale;
        scaled_level_rect.y *= h_scale;
        scaled_level_rect.w *= w_scale;
        scaled_level_rect.h *= h_scale;

        if (SDL_PointInRect(mouse_pos, &scaled_title_arrow)) {
            game_state = TITLE_SCREEN;
        }
        else if (SDL_PointInRect(mouse_pos, &scaled_left)) {
            level_selected--;
            
            if (level_selected < 0) {
                level_selected = LEVEL_COUNT - 1;
            }
        }
        else if (SDL_PointInRect(mouse_pos, &scaled_right)) {
            level_selected++;

            if (level_selected == LEVEL_COUNT) {
                level_selected = 0;
            }
        }
        else if (SDL_PointInRect(mouse_pos, &scaled_level_rect)) {
            game_state = PLAYING;
            m_need_to_recall_playing_state_constructor = true;
            Mix_HaltMusic();
            Mix_PlayChannel(0, m_play_sound, 0);
        }
    }

    m_mouse_held = mouse_held;
    m_mouse_pos = *mouse_pos;
}

void LevelSelect::render() {
    // render textures
    SDL_RenderCopyEx(renderer, m_corner, NULL, &m_left_corner_dst, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, m_corner, NULL, &m_right_corner_dst, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(renderer, m_top, NULL, &m_top_dst, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, m_level_arrow, NULL, &m_left_level_arrow_dst, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(renderer, m_level_arrow, NULL, &m_right_level_arrow_dst, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, m_title_arrow, NULL, &m_title_arrow_dst, 0, NULL, SDL_FLIP_NONE);

    // render the rectangle with the level name
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &m_rect_with_level_name);

    // render level name
    std::string level_name = m_level_strings[level_selected];
    SDL_Surface *level_shadow_surface = TTF_RenderText_Blended(m_font, level_name.c_str(), {0, 0, 0, 100});
    SDL_Surface *level_string_surface = TTF_RenderText_Blended(m_font, level_name.c_str(), {255, 255, 255});
    SDL_Surface *level_string_outline = TTF_RenderText_Blended(m_font_outline, m_level_strings[level_selected].c_str(), {0, 0, 0});
    SDL_SetSurfaceBlendMode(level_string_surface, SDL_BLENDMODE_BLEND);
    SDL_Rect rect = {4, 4, level_string_surface->w, level_string_surface->h};
    SDL_BlitSurface(level_string_surface, NULL, level_string_outline, &rect);
    SDL_FreeSurface(level_string_surface);
    SDL_Texture *level_shadow_texture = SDL_CreateTextureFromSurface(renderer, level_shadow_surface);
    SDL_Texture *level_string_texture = SDL_CreateTextureFromSurface(renderer, level_string_outline);
    SDL_Rect string_dst = {WIDTH/2 - level_string_outline->w/2, m_rect_with_level_name.y + m_rect_with_level_name.h/2 - level_string_outline->h/2, level_string_outline->w, level_string_outline->h};
    SDL_Rect shadow_dst = string_dst;
    shadow_dst.x += 5;
    shadow_dst.y += 5;
    SDL_RenderCopy(renderer, level_shadow_texture, NULL, &shadow_dst);
    SDL_RenderCopy(renderer, level_string_texture, NULL, &string_dst);
    SDL_FreeSurface(level_shadow_surface);
    SDL_DestroyTexture(level_shadow_texture);
    SDL_FreeSurface(level_string_outline);
    SDL_DestroyTexture(level_string_texture);
}

bool LevelSelect::get_need_to_recall_playing_state_constructor() {
    if (m_need_to_recall_playing_state_constructor) {
        m_need_to_recall_playing_state_constructor = false;
        return true;
    }
    return false;
}
