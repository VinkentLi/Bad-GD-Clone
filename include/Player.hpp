#pragma once
#include "Global.hpp"

class Player
{
private:
    SDL_Texture *playerTexture;
    SDL_FPoint pos;
    double xVelocity, yVelocity, jumpStrength, gravity, rotationAdder, rotation, targetRotation;
    SDL_FRect hazardHitbox, solidHitbox;
    bool grounded, mouseHeld;

public:
    Player();
    void update(float delta, bool mouseHeld);
    SDL_FRect getHazardHitbox();
    SDL_FRect getSolidHitbox();
    void render();
};
