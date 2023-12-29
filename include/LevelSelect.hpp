#pragma once
#include "GameStates.hpp"
#include "Global.hpp"

class LevelSelect {
private:
    SDL_Rect m_left_corner_dst, m_right_corner_dst, m_top_dst, m_left_level_arrow_dst, m_right_level_arrow_dst, m_title_arrow_dst, m_rect_with_level_name;
    TTF_Font *m_font;
    TTF_Font *m_font_outline;
    SDL_Texture *m_corner, *m_top, *m_level_arrow, *m_title_arrow;
    SDL_Point m_mouse_pos;
    Mix_Chunk *m_play_sound;
    bool m_mouse_held, m_need_to_recall_playing_state_constructor;
    std::vector<std::string> m_level_strings;

public:
    LevelSelect();
    void update(int &game_state, SDL_Point *mouse_pos, bool mouse_held);
    void render();
    bool get_need_to_recall_playing_state_constructor();
};

