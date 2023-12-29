#pragma once
#include "GameStates.hpp"
#include "Global.hpp"

class TitleScreen {
private:
    SDL_Rect m_title_dst, m_title_play_dst;
    SDL_Texture *m_title_texture, *m_title_play;
    SDL_Point m_mouse_pos;
    bool m_mouse_held;

public:
    TitleScreen();
    void update(int &game_state, SDL_Point *mouse_pos, bool mouse_held);
    void render();
};
