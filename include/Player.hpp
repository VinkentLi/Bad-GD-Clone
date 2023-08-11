#pragma once
#include "GameObject.hpp"

class Player
{
private:
    SDL_Texture *playerTexture;
    Mix_Chunk *deathSound;
    SDL_FPoint pos;
    double xVelocity, yVelocity, jumpStrength, padStrength, gravity, rotationAdder, rotation, targetRotation;
    SDL_FRect hazardHitbox, solidHitbox;
    std::vector<SDL_FRect *> pressedOrbs;
    bool grounded, mouseHeld, dead, orbBuffered;
    float deadTimer;
    int gravityMultiplier;

public:
    Player();
    void update(float delta, bool mouseHeld, std::vector<GameObject> objects);
    void handleCollisions(std::vector<GameObject> objects);
    void die();
    bool isDead();
    // SDL_FRect getHazardHitbox();
    // SDL_FRect getSolidHitbox();
    void render();
};
