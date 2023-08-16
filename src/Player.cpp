#include <algorithm>
#include "Player.hpp"

Player::Player()
{
    playerTexture = IMG_LoadTexture(renderer, "res/gfx/icon.png");
    shipTexture = IMG_LoadTexture(renderer, "res/gfx/ship.png");
    deathSound = Mix_LoadWAV("res/sfx/deathSound.ogg");
    pos = {-TILE_SIZE, HEIGHT - 300 - TILE_SIZE};
    previousPos = pos;
    xVelocity = 17.31;
    yVelocity = 0;
    jumpStrength = -37.2671;
    padStrength = -52.61467;
    gravity = 2.874767;
    rotationAdder = 6.92308;
    shipUpAdder = 1.4373;
    shipDownAdder = 1.3798;
    rotation = 0;
    targetRotation = 0;
    hazardHitbox = {pos.x, pos.y, TILE_SIZE, TILE_SIZE};
    solidHitbox = {pos.x + TILE_SIZE / 3, pos.y + TILE_SIZE / 3, TILE_SIZE / 3, TILE_SIZE / 3};
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
            solidHitbox.x = hazardHitbox.x + TILE_SIZE / 3;
            solidHitbox.y = hazardHitbox.y + TILE_SIZE / 3;
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
                targetRotation = ((int)rotation / 90) * 90 + 90;
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
                targetRotation = ((int)rotation / 90) * 90 - 90;
                // std::cout << targetRotation << ' ';
            }
            else if (rotation < targetRotation)
            {
                rotation = targetRotation;
            }
        }

        if (yVelocity > TILE_SIZE / 2)
        {
            yVelocity = TILE_SIZE / 2;
        }
        break;
    case SHIP:
    {
        double shipUp = shipUpAdder;
        double shipDown = shipDownAdder;
        double lessHelpUp = 0.9;
        double lessHelpDown = 0.667;

        if (gravityMultiplier == -1)
        {
            double t1 = shipUp;
            shipUp = shipDown;
            shipDown = t1;
            double t2 = lessHelpUp;
            lessHelpUp = lessHelpDown;
            lessHelpDown = t2;
        }

        if (mouseHeld)
        {
            if (yVelocity > -7)
            {
                yVelocity -= shipUp * delta;
            }
            else
            {
                yVelocity -= shipUp * lessHelpUp * delta;
            }
        }
        else
        {
            if (yVelocity < -7)
            {
                yVelocity += shipDown * delta;
            }
            else
            {
                yVelocity += shipDown * lessHelpDown * delta;
            }
        }

        if (yVelocity < -80.0 / 3.0)
        {
            yVelocity = -80.0f / 3.0f;
        }
        else if (yVelocity > 64.0 / 3.0)
        {
            yVelocity = 64.0 / 3.0;
        }
        break;
    }
    }

    yVelocity *= gravityMultiplier;

    hazardHitbox.x += xVelocity * delta;
    hazardHitbox.y += yVelocity * delta;
    solidHitbox.x += xVelocity * delta;
    solidHitbox.y += yVelocity * delta;

    yVelocity /= gravityMultiplier;

    handleCollisions(objects);

    if (gamemode == SHIP)
    {
        // i tried to make rotations accurate, but i couldn't get it to work

        if (yVelocity == 0.0)
        {
            if (rotation < 0)
            {
                rotation = std::clamp(rotation + rotationAdder, -360.0, 0.0);
            }
            else if (rotation > 0)
            {
                rotation = std::clamp(rotation - rotationAdder, 0.0, 360.0);
            }
        }
        else
        {
            rotation = yVelocity * 2.0;
            rotation /= std::clamp(5.0 / std::abs(yVelocity), 1.0, 3.0);
        }

        rotation *= gravityMultiplier;
    }

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

        if (pos.y - cameraPos.y > CAMERA_DOWN_SCROLL)
        {
            cameraPos.y = pos.y - CAMERA_DOWN_SCROLL;
        }
        break;
    }

    if (pos.y < -7 * BACKGROUND_SIZE)
    {
        die();
    }

    if (grounded)
    {
        orbBuffered = false;
    }

    previousPos = pos;
}

void Player::handleCollisions(std::vector<GameObject> objects)
{   
    switch (gamemode)
    {
    case CUBE:    
        if (hazardHitbox.y > HEIGHT - 300 - TILE_SIZE)
        {
            hazardHitbox.y = HEIGHT - 300 - TILE_SIZE;
            solidHitbox.y = hazardHitbox.y + TILE_SIZE / 3;
            grounded = true;
            yVelocity = 0;
        }
        break;
    case SHIP:
        if (hazardHitbox.y < bounds.first)
        {
            hazardHitbox.y = bounds.first;
            solidHitbox.y = hazardHitbox.y + TILE_SIZE / 3;
            yVelocity = 0;
        }

        if (hazardHitbox.y + hazardHitbox.h > bounds.second)
        {
            hazardHitbox.y = bounds.second - hazardHitbox.h;
            solidHitbox.y = hazardHitbox.y + TILE_SIZE / 3;
            grounded = true;
            yVelocity = 0;
        }
        break;
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

                    if (gamemode == SHIP)
                    {
                        if (yVelocity < 0 && solidHitbox.y > object.getHitbox()->y + object.getHitbox()->h)
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

                if (gamemode != SHIP)
                {
                    yVelocity /= 2;
                }

                cameraPos.y = std::clamp((float)((((int)(object.getPos().y / TILE_SIZE)) - 3) * TILE_SIZE), -10000.0f, -260.0f);
                bounds.first = cameraPos.y + 40;
                bounds.second = bounds.first + 10 * TILE_SIZE;
                break;
            case CUBE_PORTAL:
                gamemode = CUBE;

                if (gamemode != CUBE)
                {
                    yVelocity /= 2;
                }
                break;
            }
        }
    }

    grounded = yVelocity == 0 && (!mouseHeld || gamemode != SHIP);
    solidHitbox.x = hazardHitbox.x + TILE_SIZE / 3;
    solidHitbox.y = hazardHitbox.y + TILE_SIZE / 3;
    pos.x = hazardHitbox.x;
    pos.y = hazardHitbox.y;
}

void Player::render()
{
    switch (gamemode)
    {
    case CUBE:
    {
        SDL_FRect dst = {pos.x - cameraPos.x, pos.y - cameraPos.y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopyExF(renderer, playerTexture, NULL, &dst, rotation, NULL, SDL_FLIP_NONE);
        break;
    }
    case SHIP:
    {
        if (gravityMultiplier == 1)
        {
            SDL_FRect cubeDST = {pos.x - cameraPos.x + TILE_SIZE / 4, pos.y - cameraPos.y, TILE_SIZE * 3 / 5, TILE_SIZE * 3 / 5};
            SDL_FRect shipDST = {pos.x - cameraPos.x - 14, pos.y - cameraPos.y + 22, 132, 78};
            SDL_FPoint cubeCenter = {shipDST.x + shipDST.w / 2 - cubeDST.x, shipDST.y + shipDST.h / 2 - cubeDST.y};
            SDL_RenderCopyExF(renderer, playerTexture, NULL, &cubeDST, rotation, &cubeCenter, SDL_FLIP_NONE);
            SDL_RenderCopyExF(renderer, shipTexture, NULL, &shipDST, rotation, NULL, SDL_FLIP_NONE);
        }
        else
        {
            SDL_FRect cubeDST = {pos.x - cameraPos.x + TILE_SIZE / 4, pos.y - cameraPos.y + TILE_SIZE * 2 / 5, TILE_SIZE * 3 / 5, TILE_SIZE * 3 / 5};
            SDL_FRect shipDST = {pos.x - cameraPos.x - 14, pos.y - cameraPos.y, 132, 78};
            SDL_FPoint cubeCenter = {shipDST.x + shipDST.w / 2 - cubeDST.x, shipDST.y + shipDST.h / 2 - cubeDST.y};
            SDL_RenderCopyExF(renderer, playerTexture, NULL, &cubeDST, rotation, &cubeCenter, SDL_FLIP_VERTICAL);
            SDL_RenderCopyExF(renderer, shipTexture, NULL, &shipDST, rotation, NULL, SDL_FLIP_VERTICAL);
        }
        break;
    }
    }

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

int Player::getGamemode()
{
    return gamemode;
}

// SDL_FRect Player::getHazardHitbox()
// {
//     return hazardHitbox;
// }

// SDL_FRect Player::getSolidHitbox()
// {
//     return solidHitbox;
// }
