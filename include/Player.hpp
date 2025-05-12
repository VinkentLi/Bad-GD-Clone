#pragma once
#include "GameObject.hpp"

enum Gamemodes
{
    CUBE,
    SHIP
};

class Player
{
private:
    SDL_Texture *m_PlayerTexture, *m_ShipTexture;
    Mix_Chunk *m_DeathSound;
    SDL_FPoint m_Pos, m_PreviousPos;
    double m_XVelocity, m_YVelocity, m_JumpStrength, m_PadStrength, m_Gravity, m_RotationAdder, m_ShipUpAdder, m_ShipDownAdder, m_Rotation, m_TargetRotation;
    SDL_FRect m_HazardHitbox, m_SolidHitbox;
    std::vector<SDL_FRect *> m_PressedOrbs;
    bool m_Grounded, m_MouseHeld, m_Dead, m_OrbBuffered;
    float m_DeadTimer;
    int m_GravityMultiplier, m_Gamemode;
    std::pair<float, float> m_Bounds;

public:
    Player();
    void update(float delta, bool mouseHeld, std::vector<GameObject> objects);
    void handleCollisions(std::vector<GameObject> objects);
    void die();
    bool isDead();
    // SDL_FRect getHazardHitbox();
    // SDL_FRect getSolidHitbox();
    void render();
    int getGamemode();
};
