#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>

enum class Gamemode {
    CUBE,
    SHIP
};

struct Checkpoint {
    SDL_FPoint position;
    SDL_FPoint cameraPosition;
    double yVelocity;
    double rotation;
    double targetRotation;
    int gravityMultiplier;
    Gamemode gamemode;
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
    inline void moveX(float dist) { m_Position.x += dist; }
    inline void moveY(float dist) { m_Position.y += dist; }
    void die();
    bool isDead();
    // SDL_FRect getHazardHitbox();
    // SDL_FRect getSolidHitbox();
    void render();
    Gamemode getGamemode();
    inline SDL_FPoint getPosition() { return m_Position; }

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_PlayerTexture;
    SDL_Texture *m_ShipTexture;
    SDL_Texture *m_CheckpointTexture;
    Mix_Chunk *m_DeathSound;
    SDL_FPoint m_Position;
    SDL_FPoint m_PreviousPosition;
    double m_XVelocity;
    double m_YVelocity;
    double m_JumpStrength;
    double m_PadStrength;
    double m_Gravity;
    double m_RotationAdder;
    double m_Rotation;
    double m_TargetRotation;
    SDL_FRect m_HazardHitbox, m_SolidHitbox;
    std::vector<SDL_FRect *> m_PressedOrbs;
    std::vector<Checkpoint> m_Checkpoints;
    bool m_IsGrounded;
    bool m_IsMouseHeld;
    bool m_IsZHeld;
    bool m_IsXHeld;
    bool m_IsDead;
    bool m_HasBufferedOrb;
    float m_DeadTimer;
    int m_GravityMultiplier;
    Gamemode m_Gamemode;
    std::pair<float, float> m_Bounds;
};
