#pragma once
#include "Global.hpp"

enum ObjectType
{
    BLOCK,
    HAZARD,
    ORB,
    PAD,
    SHIP_PORTAL,
    UPSIDE_DOWN_PORTAL,
    NORMAL_PORTAL
};

class GameObject
{
private:
    SDL_Rect src;
    int type;
    SDL_FPoint pos;
    SDL_FRect hitbox;

public:
    GameObject(SDL_Rect src, int type, SDL_FPoint pos, SDL_FRect hitbox);
    void render();
};

