#pragma once
#include <SDL.h>

class Game;

enum class ObjectType {
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
    GameObject(Game *game, ObjectType type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, SDL_Texture *objectTexture);
    SDL_FRect *getHitbox();
    SDL_FPoint getPos();
    float getWidth();
    ObjectType getType();
    void render();

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    ObjectType m_Type;
    int m_Rotation;
    SDL_FPoint m_Position;
    SDL_Rect m_Rect;
    SDL_FRect m_Hitbox;
    SDL_Texture *m_ObjectTexture;
};

