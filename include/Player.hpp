#pragma once
#include "GameObject.hpp"

enum Gamemodes {
    CUBE,
    SHIP
};

class Player {
private:
    SDL_Texture *m_player_texture, *m_ship_texture;
    Mix_Chunk *m_death_sound;
    SDL_FPoint m_pos, m_previous_pos;
    double m_x_velocity, m_y_velocity, m_jump_strength, m_pad_strength, m_gravity, m_rotation_adder, m_ship_up_adder, m_ship_down_adder, m_rotation, m_target_rotation;
    SDL_FRect m_hazard_hitbox, m_solid_hitbox;
    std::vector<SDL_FRect *> m_pressed_orbs;
    bool m_grounded, m_mouse_held, m_dead, m_orb_buffered;
    float m_dead_timer;
    int m_gravity_multiplier, m_gamemode;
    std::pair<float, float> m_bounds;

public:
    Player();
    void update(float delta, bool mouse_held, std::vector<GameObject> objects);
    void handle_collisions(std::vector<GameObject> objects);
    void die();
    bool is_dead();
    // SDL_FRect getHazardHitbox();
    // SDL_FRect getSolidHitbox();
    void render();
    int get_gamemode();
};
