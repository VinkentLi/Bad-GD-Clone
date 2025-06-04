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
    NORMAL_PORTAL,
    DECO,
    BG_TRIGGER,
    G_TRIGGER
};

class GameObject {
public:
    GameObject(Game *game, ObjectType type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, bool xFlip, bool yFlip, SDL_Texture *objectTexture, SDL_Color color, float duration);
    void activate();
    void reset();
    SDL_FRect *getHitbox();
    SDL_FPoint getPos();
    float getWidth();
    float getHeight();
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
    bool m_FlipX;
    bool m_FlipY;
    SDL_Texture *m_ObjectTexture;
    SDL_Color m_Color;
    float m_Duration;
    bool m_IsActivated = false;
};

