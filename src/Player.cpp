#include <algorithm>
#include "Player.hpp"

Player::Player()
{
    playerTexture = IMG_LoadTexture(renderer, "res/gfx/icon.png");
    deathSound = Mix_LoadWAV("res/sfx/deathSound.ogg");
    pos = {-TILE_SIZE, HEIGHT - 300 - TILE_SIZE};
    xVelocity = 17.31;
    yVelocity = 0;
    jumpStrength = -37.2671;
    padStrength = -52.61467;
    gravity = 2.874767;
    rotationAdder = 6.92308;
    shipUpAdderHigh = 1.4373;
    shipUpAdderLow = 1.293567;
    shipDownAdder = 0.91987;
    rotation = 0;
    targetRotation = 0;
    hazardHitbox = {pos.x, pos.y, TILE_SIZE, TILE_SIZE};
    solidHitbox = {pos.x + TILE_SIZE/3, pos.y + TILE_SIZE/3, TILE_SIZE/3, TILE_SIZE/3};
    grounded = true;
    mouseHeld = false;
    orbBuffered = false;
    dead = false;
    deadTimer = 0;
    gravityMultiplier = 1;
    gamemode = CUBE;
}

void Player::update(float delta, bool mouseHeld, std::vector<GameObject> objects)
{
    bool mouseClicked = (!this->mouseHeld && mouseHeld);
    bool mouseReleased = (this->mouseHeld && !mouseHeld);
    this->mouseHeld = mouseHeld;

    if (dead)
    {
        deadTimer -= delta;

        if (deadTimer < 0)
        {
            gamemode = CUBE;
            pressedOrbs.clear();
            dead = false;
            grounded = true;
            hazardHitbox.x = -TILE_SIZE;
            hazardHitbox.y = HEIGHT - 300 - TILE_SIZE;
            solidHitbox.x = hazardHitbox.x + TILE_SIZE/3;
            solidHitbox.y = hazardHitbox.y + TILE_SIZE/3;
            pos = {hazardHitbox.x, hazardHitbox.y};
            cameraPos = {0, 0};
            rotation = 0;
            targetRotation = 0;
            gravityMultiplier = 1;
        }
        return;
    }

    switch (gamemode)
    {
    case CUBE:    
        if (mouseClicked)
        {
            orbBuffered = true;
        }
        else if (mouseReleased)
        {
            orbBuffered = false;
        }

        if (grounded && this->mouseHeld)
        {
            yVelocity = jumpStrength;
            grounded = false;
        }

        yVelocity += gravity * delta;
        rotation += gravityMultiplier == 1 ? rotationAdder * delta : -rotationAdder * delta;

        if (gravityMultiplier == 1)
        {
            if (!grounded && rotation > targetRotation)
            {
                targetRotation = ((int) rotation/90)*90 + 90;
                // std::cout << targetRotation << ' ';
            }
            else if (rotation > targetRotation)
            {
                rotation = targetRotation;
            }
        }
        else
        {
            if (!grounded && rotation < targetRotation)
            {
                targetRotation = ((int) rotation/90)*90 - 90;
                // std::cout << targetRotation << ' ';
            }
            else if (rotation < targetRotation)
            {
                rotation = targetRotation;
            }
        }

        if (yVelocity > TILE_SIZE/2)
        {
            yVelocity = TILE_SIZE/2;
        }
        break;
    case SHIP:
        if (mouseHeld)
        {
            if (yVelocity < 1.7)
            {
                yVelocity -= shipUpAdderLow * delta;
            }
            else
            {
                yVelocity -= shipUpAdderHigh * delta;
            }
        }
        else
        {
            yVelocity += shipDownAdder * delta;
        }

        if (yVelocity < -80.0/3.0)
        {
            yVelocity = -80.0f/3.0f;
        }
        else if (yVelocity > 64.0/3.0)
        {
            yVelocity = 64.0/3.0;
        }
        break;
    }

    yVelocity *= gravityMultiplier;

    hazardHitbox.x += xVelocity * delta;
    hazardHitbox.y += yVelocity * delta;
    solidHitbox.x += xVelocity * delta;
    solidHitbox.y += yVelocity * delta;

    yVelocity /= gravityMultiplier;

    handleCollisions(objects);

    if (pos.x - cameraPos.x > CAMERA_SCROLL)
    {
        cameraPos.x = pos.x - CAMERA_SCROLL;
    }

    switch (gamemode)
    {
    case CUBE:        
        if (pos.y - cameraPos.y < CAMERA_UP_SCROLL)
        {
            cameraPos.y = pos.y - CAMERA_UP_SCROLL;
        }
        break;
    }
    

    if (pos.y < -7*BACKGROUND_SIZE)
    {
        die();
    }

    if (grounded)
    {
        orbBuffered = false;
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
        SDL_FRect intersect; // this is useless but im too lazy to remove it lmfao

        if (SDL_IntersectFRect(&hazardHitbox, object.getHitbox(), &intersect))
        {
            switch (object.getType())
            {
            case HAZARD:
                die();
                break;
            case BLOCK:
                if (!SDL_HasIntersectionF(&solidHitbox, object.getHitbox()))
                {
                    if (gravityMultiplier == 1)
                    {
                        if (yVelocity > 0 && solidHitbox.y + solidHitbox.h < object.getHitbox()->y)
                        {
                            hazardHitbox.y = object.getPos().y - TILE_SIZE;
                            yVelocity = 0;
                        }
                    }
                    else
                    {
                        if (yVelocity > 0 && solidHitbox.y > object.getHitbox()->y + object.getHitbox()->h)
                        {
                            hazardHitbox.y = object.getPos().y + TILE_SIZE;
                            yVelocity = 0;
                        }
                    }
                    break;
                }
                die();
                break;
            case PAD:
                yVelocity = padStrength;
                break;
            case ORB:
                if (orbBuffered && (pressedOrbs.empty() || std::find(pressedOrbs.begin(), pressedOrbs.end(), object.getHitbox()) == pressedOrbs.end()))
                {
                    yVelocity = jumpStrength;
                    pressedOrbs.push_back(object.getHitbox());
                }
                break;
            case UPSIDE_DOWN_PORTAL:
                if (gravityMultiplier == -1) 
                {
                    break;
                }

                yVelocity /= 2;
                gravityMultiplier = -1;

                if (yVelocity < 0)
                {
                    yVelocity *= -1;
                }
                break;
            case NORMAL_PORTAL:
                if (gravityMultiplier == 1)
                {
                    break;
                }

                yVelocity /= 2;
                gravityMultiplier = 1;

                if (yVelocity < 0)
                {
                    yVelocity *= -1;
                }
                break;
            case SHIP_PORTAL:
                gamemode = SHIP;
                rotation = 0;
                break;
            case CUBE_PORTAL:
                gamemode = CUBE;
                break;
            }
        }
    }

    grounded = yVelocity == 0;
    solidHitbox.x = hazardHitbox.x + TILE_SIZE/3;
    solidHitbox.y = hazardHitbox.y + TILE_SIZE/3;
    pos.x = hazardHitbox.x;
    pos.y = hazardHitbox.y;
}

void Player::render()
{
    SDL_FRect dst = {pos.x - cameraPos.x, pos.y - cameraPos.y, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopyExF(renderer, playerTexture, NULL, &dst, rotation, NULL, SDL_FLIP_NONE);
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
    // SDL_FRect temp = hazardHitbox;
    // temp.x -= cameraPos.x;
    // SDL_RenderFillRectF(renderer, &temp);
    // SDL_SetRenderDrawColor(renderer, 0, 0, 255, 125);
    // SDL_FRect temp2 = solidHitbox;
    // temp2.x -= cameraPos.x;
    // SDL_RenderFillRectF(renderer, &temp2);
}

void Player::die()
{
    dead = true;
    deadTimer = 90;
    Mix_HaltMusic();
    Mix_PlayChannel(-1, deathSound, 0);
    yVelocity = 0;
}

bool Player::isDead()
{
    return dead;
}

// SDL_FRect Player::getHazardHitbox()
// {
//     return hazardHitbox;
// }

// SDL_FRect Player::getSolidHitbox()
// {
//     return solidHitbox;
// }
