#pragma once
#include <SDL.h>

class Game;

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
public:
    GameObject(Game *game, int type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, const char *texturePath);
    SDL_FRect *getHitbox();
    SDL_FPoint getPos();
    int getType();
    void render();

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    int m_Type;
    int m_Rotation;
    SDL_FPoint m_Position;
    SDL_FRect m_Hitbox;
    SDL_Texture *m_ObjectTexture;
};

