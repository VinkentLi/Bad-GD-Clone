#pragma once
#include "Global.hpp"

enum ObjectType
{
    BLOCK,
    HAZARD,
    ORB,
    PAD,
    SHIP_PORTAL,
    CUBE_PORTAL,
    UPSIDE_DOWN_PORTAL,
    NORMAL_PORTAL
};

class GameObject
{
private:
    int type;
    SDL_FPoint pos;
    SDL_FRect hitbox;
    SDL_Texture *objectTexture;

public:
    GameObject(int type, SDL_FPoint pos, SDL_FRect hitbox, const char *texturePath);
    SDL_FRect *getHitbox();
    SDL_FPoint getPos();
    int getType();
    void render();
};

