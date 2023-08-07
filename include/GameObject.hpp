#pragma once
#include "Global.hpp"

enum ObjectType
{
    BLOCK,
    HAZARD,
    ORB,
    PAD
};

class GameObject
{
private:
    int id, type;
    SDL_FPoint pos;
    SDL_FRect hitbox;

public:
    GameObject(int id, int type, SDL_FPoint pos, SDL_FRect hitbox);
    void render();
};

