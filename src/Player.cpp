#include "Player.hpp"

Player::Player()
{
    playerTexture = IMG_LoadTexture(renderer, "res/gfx/icon.png");
    pos = {-TILE_SIZE, HEIGHT - 300 - TILE_SIZE};
    xVelocity = 21.1182;
    yVelocity = 0;
    jumpStrength = -45.465659;
    gravity = 3.507012;
    rotationAdder = 6.92308;
    rotation = 0;
    targetRotation = 0;
    hazardHitbox = {pos.x, pos.y, TILE_SIZE, TILE_SIZE};
    solidHitbox = {pos.x + TILE_SIZE/3, pos.y + TILE_SIZE/3, TILE_SIZE/3, TILE_SIZE/3};
    grounded = true;
    mouseHeld = false;
}

void Player::update(float delta, bool mouseHeld)
{
    this->mouseHeld = mouseHeld;

    if (grounded && this->mouseHeld)
    {
        yVelocity = jumpStrength;
        grounded = false;
    }

    if (!grounded)
    {
        yVelocity += gravity * delta;
    }

    rotation += rotationAdder * delta;

    if (!grounded && rotation > targetRotation)
    {
        targetRotation = ((int) rotation/90)*90 + 90;
    }
    else if (rotation > targetRotation)
    {
        rotation = targetRotation;
    }

    hazardHitbox.x += xVelocity;
    hazardHitbox.y += yVelocity;
    solidHitbox.x += xVelocity;
    solidHitbox.y += yVelocity;

    if (hazardHitbox.y > HEIGHT - 300 - TILE_SIZE)
    {
        hazardHitbox.y = HEIGHT - 300 - TILE_SIZE;
        solidHitbox.y = hazardHitbox.y + TILE_SIZE/3;
        grounded = true;
    }
    
    pos.x = hazardHitbox.x;
    pos.y = hazardHitbox.y;
}

void Player::render()
{
    SDL_FRect dst = {pos.x, pos.y, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopyExF(renderer, playerTexture, NULL, &dst, rotation, NULL, SDL_FLIP_NONE);
}
