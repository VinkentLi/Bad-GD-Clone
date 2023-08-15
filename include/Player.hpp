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
    SDL_Texture *playerTexture, *shipTexture;
    Mix_Chunk *deathSound;
    SDL_FPoint pos, previousPos;
    double xVelocity, yVelocity, jumpStrength, padStrength, gravity, rotationAdder, shipUpAdder, shipDownAdder, rotation, targetRotation;
    SDL_FRect hazardHitbox, solidHitbox;
    std::vector<SDL_FRect *> pressedOrbs;
    bool grounded, mouseHeld, dead, orbBuffered;
    float deadTimer;
    int gravityMultiplier, gamemode;
    std::pair<float, float> bounds;

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
