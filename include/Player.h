#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <array>

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
    std::array<float, 3> backgroundColor;
    std::array<float, 3> groundColor;
    SDL_Color backgroundTargetColor;
    SDL_Color groundTargetColor;
    float backgroundFadeTime = 0;
    float groundFadeTime = 0;
};

class Game;
class GameObject;
class Trigger;

class Player {
public:
    Player() = default;
    ~Player();
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    void init(Game *game);
    void reset();
    void update(float deltaTime);
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
    int m_ShipWidth;
    int m_ShipHeight;
    SDL_Texture *m_CheckpointTexture;
    Mix_Chunk *m_DeathSound;
    SDL_FPoint m_Position;
    SDL_FPoint m_PreviousPosition;
    double m_XVelocity = 20.772;
    double m_YVelocity;
    double m_JumpStrength = -44.72;
    double m_PadStrength = -64;
    double m_Gravity = 3.456;
    double m_RotationAdder = 6.92308;
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

    void updatePractice();
    void respawn();
    void updatePhysics(float deltaTime, bool mouseClicked, bool mouseReleased);
    void handleCollisions(std::vector<GameObject> &objects);
    void collideWithGround();
    void collideWithObject(GameObject &object);
    void snapToObject(GameObject &object);
    void setShipBounds(GameObject &shipPortal);
    void activateTriggers(std::vector<Trigger> &triggers);
    void updateCubeRotation(float deltaTime);
    void updateShipRotation(float deltaTime);
    void scrollCamera(float deltaTime);
    void renderCube();
    void renderShip();
};
