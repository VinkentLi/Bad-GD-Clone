#include "Player.hpp"

Player::Player()
{
    playerTexture = IMG_LoadTexture(renderer, "res/gfx/icon.png");
    pos = {-TILE_SIZE, HEIGHT - 300 - TILE_SIZE};
    xVelocity = 17.31;
    yVelocity = 0;
    jumpStrength = -37.2671;
    gravity = 2.874767;
    rotationAdder = 6.92308;
    rotation = 0;
    targetRotation = 0;
    hazardHitbox = {pos.x, pos.y, TILE_SIZE, TILE_SIZE};
    solidHitbox = {pos.x + TILE_SIZE/3, pos.y + TILE_SIZE/3, TILE_SIZE/3, TILE_SIZE/3};
    grounded = true;
    mouseHeld = false;
}

void Player::update(float delta, bool mouseHeld, std::vector<GameObject> objects)
{
    this->mouseHeld = mouseHeld;

    if (grounded && this->mouseHeld)
    {
        yVelocity = jumpStrength;
        grounded = false;
    }

    yVelocity += gravity * delta;
    rotation += rotationAdder * delta;

    if (!grounded && rotation > targetRotation)
    {
        targetRotation = ((int) rotation/90)*90 + 90;
    }
    else if (rotation > targetRotation)
    {
        rotation = targetRotation;
    }

    hazardHitbox.x += xVelocity * delta;
    hazardHitbox.y += yVelocity * delta;
    solidHitbox.x += xVelocity * delta;
    solidHitbox.y += yVelocity * delta;

    handleCollisions(objects);

    if (pos.x - cameraPos.x > CAMERA_SCROLL)
    {
        cameraPos.x = pos.x - CAMERA_SCROLL;
    }
}

void Player::handleCollisions(std::vector<GameObject> objects)
{
    if (hazardHitbox.y > HEIGHT - 300 - TILE_SIZE)
    {
        hazardHitbox.y = HEIGHT - 300 - TILE_SIZE;
        solidHitbox.y = hazardHitbox.y + TILE_SIZE/3;
        grounded = true;
        yVelocity = 0;
    }

    for (GameObject &object : objects)
    {
        SDL_FRect intersect;

        if (SDL_IntersectFRect(&hazardHitbox, object.getHitbox(), &intersect))
        {
            switch (object.getType())
            {
            case HAZARD:
                break;
            case BLOCK:
                if (!SDL_HasIntersectionF(&solidHitbox, object.getHitbox()))
                {
                    if (yVelocity > 0)
                    {
                        hazardHitbox.y = object.getPos().y - TILE_SIZE;
                        grounded = true;
                        yVelocity = 0;
                    }
                    else
                    {
                        grounded = false;
                    }
                }
                break;
            }
        }
    }

    solidHitbox.x = hazardHitbox.x + TILE_SIZE/3;
    solidHitbox.y = hazardHitbox.y + TILE_SIZE/3;
    pos.x = hazardHitbox.x;
    pos.y = hazardHitbox.y;
}

void Player::render()
{
    SDL_FRect dst = {pos.x - cameraPos.x, pos.y, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopyExF(renderer, playerTexture, NULL, &dst, rotation, NULL, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
    SDL_FRect temp = hazardHitbox;
    temp.x -= cameraPos.x;
    SDL_RenderFillRectF(renderer, &temp);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 125);
    SDL_FRect temp2 = solidHitbox;
    temp2.x -= cameraPos.x;
    SDL_RenderFillRectF(renderer, &temp2);
}

// SDL_FRect Player::getHazardHitbox()
// {
//     return hazardHitbox;
// }

// SDL_FRect Player::getSolidHitbox()
// {
//     return solidHitbox;
// }
