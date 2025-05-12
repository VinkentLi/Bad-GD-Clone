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
    int m_Type, m_Rotation;
    SDL_FPoint m_Pos;
    SDL_FRect m_Hitbox;
    SDL_Texture *m_ObjectTexture;

public:
    GameObject(int type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, const char *texturePath);
    SDL_FRect *getHitbox();
    SDL_FPoint getPos();
    int getType();
    void render();
};

