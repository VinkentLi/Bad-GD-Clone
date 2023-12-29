#pragma once
#include "Global.hpp"

enum ObjectType {
    BLOCK,
    HAZARD,
    ORB,
    PAD,
    SHIP_PORTAL,
    CUBE_PORTAL,
    UPSIDE_DOWN_PORTAL,
    NORMAL_PORTAL
};

class GameObject {
private:
    int m_type, m_rotation;
    SDL_FPoint m_pos;
    SDL_FRect m_hitbox;
    SDL_Texture *m_object_texture;

public:
    GameObject(int type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, const char *texture_path);
    SDL_FRect *get_hitbox();
    SDL_FPoint get_pos();
    int get_type();
    void render();
};

