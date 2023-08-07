#pragma once
#include "Global.hpp"

class Player
{
private:
    SDL_Texture *playerTexture;
    SDL_FPoint pos;
    double xVelocity, jumpStrength, gravity, rotationAdder, rotation;
public:
    Player();
    void update(float delta);
    void render();
};

