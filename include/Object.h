#pragma once
#include <SDL.h>
#include <optional>

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

class Object {
public:
    Object(Game *game, ObjectType type, int rotation, SDL_FPoint pos, std::optional<SDL_FRect> hitbox, bool xFlip, bool yFlip, SDL_Texture *objectTexture);
    std::optional<SDL_FRect> getHitbox() const;
    SDL_FPoint getPos() const;
    int getWidth() const;
    int getHeight() const;
    ObjectType getType() const;
    void render() const;

protected:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    ObjectType m_Type;
    int m_Width;
    int m_Height;
    int m_Rotation;
    SDL_FPoint m_Position;
    std::optional<SDL_FRect> m_Hitbox;
    bool m_FlipX;
    bool m_FlipY;
    SDL_Texture *m_ObjectTexture;
};

