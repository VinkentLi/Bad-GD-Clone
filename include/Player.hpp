#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>

enum Gamemodes {
    CUBE,
    SHIP
};

class Game;
class GameObject;

class Player {
public:
    Player() = default;
    ~Player();
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    void init(Game *game);
    void reset();
    void update(float delta, bool IsMouseHeld, std::vector<GameObject> &objects);
    void handleCollisions(std::vector<GameObject> &objects);
    void die();
    bool isDead();
    // SDL_FRect getHazardHitbox();
    // SDL_FRect getSolidHitbox();
    void render();
    int getGamemode();

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_PlayerTexture;
    SDL_Texture *m_ShipTexture;
    Mix_Chunk *m_DeathSound;
    SDL_FPoint m_Position;
    SDL_FPoint m_PreviousPosition;
    double m_XVelocity;
    double m_YVelocity;
    double m_JumpStrength;
    double m_PadStrength;
    double m_Gravity;
    double m_RotationAdder;
    double m_ShipUpAdder;
    double m_ShipDownAdder;
    double m_Rotation;
    double m_TargetRotation;
    SDL_FRect m_HazardHitbox, m_SolidHitbox;
    std::vector<SDL_FRect *> m_PressedOrbs;
    bool m_IsGrounded;
    bool m_IsMouseHeld;
    bool m_IsDead;
    bool m_HasBufferedOrb;
    float m_DeadTimer;
    int m_GravityMultiplier;
    int m_Gamemode;
    std::pair<float, float> m_Bounds;
};
