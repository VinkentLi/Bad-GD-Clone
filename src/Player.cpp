#include <algorithm>
#include "Player.hpp"

Player::Player()
{
    m_PlayerTexture = IMG_LoadTexture(renderer, "res/gfx/icon.png");
    m_ShipTexture = IMG_LoadTexture(renderer, "res/gfx/ship.png");
    m_DeathSound = Mix_LoadWAV("res/sfx/deathSound.ogg");
    m_Pos = {-TILE_SIZE, HEIGHT - 300 - TILE_SIZE};
    m_PreviousPos = m_Pos;
    m_XVelocity = 17.31;
    m_YVelocity = 0;
    m_JumpStrength = -37.2671;
    m_PadStrength = -52.61467;
    m_Gravity = 2.874767;
    m_RotationAdder = 6.92308;
    m_ShipUpAdder = 1.4373;
    m_ShipDownAdder = 1.3798;
    m_Rotation = 0;
    m_TargetRotation = 0;
    m_HazardHitbox = {m_Pos.x, m_Pos.y, TILE_SIZE, TILE_SIZE};
    m_SolidHitbox = {m_Pos.x + TILE_SIZE / 3, m_Pos.y + TILE_SIZE / 3, TILE_SIZE / 3, TILE_SIZE / 3};
    m_Grounded = true;
    m_MouseHeld = false;
    m_OrbBuffered = false;
    m_Dead = false;
    m_DeadTimer = 0;
    m_GravityMultiplier = 1;
    m_Gamemode = CUBE;
}

void Player::update(float delta, bool mouseHeld, std::vector<GameObject> objects)
{
    bool mouseClicked = (!this->m_MouseHeld && mouseHeld);
    bool mouseReleased = (this->m_MouseHeld && !mouseHeld);
    this->m_MouseHeld = mouseHeld;

    if (m_Dead)
    {
        m_DeadTimer -= delta;

        if (m_DeadTimer < 0)
        {
            m_Gamemode = CUBE;
            m_PressedOrbs.clear();
            m_Dead = false;
            m_Grounded = true;
            m_HazardHitbox.x = -TILE_SIZE;
            m_HazardHitbox.y = HEIGHT - 300 - TILE_SIZE;
            m_SolidHitbox.x = m_HazardHitbox.x + TILE_SIZE / 3;
            m_SolidHitbox.y = m_HazardHitbox.y + TILE_SIZE / 3;
            m_Pos = {m_HazardHitbox.x, m_HazardHitbox.y};
            cameraPos = {0, 0};
            m_Rotation = 0;
            m_TargetRotation = 0;
            m_GravityMultiplier = 1;
        }
        return;
    }

    switch (m_Gamemode)
    {
    case CUBE:
        if (mouseClicked)
        {
            m_OrbBuffered = true;
        }
        else if (mouseReleased)
        {
            m_OrbBuffered = false;
        }

        if (m_Grounded && this->m_MouseHeld)
        {
            m_YVelocity = m_JumpStrength;
            m_Grounded = false;
        }

        m_YVelocity += m_Gravity * delta;
        m_Rotation += m_GravityMultiplier == 1 ? m_RotationAdder * delta : -m_RotationAdder * delta;

        if (m_GravityMultiplier == 1)
        {
            if (!m_Grounded && m_Rotation > m_TargetRotation)
            {
                m_TargetRotation = ((int)m_Rotation / 90) * 90 + 90;
                // std::cout << targetRotation << ' ';
            }
            else if (m_Rotation > m_TargetRotation)
            {
                m_Rotation = m_TargetRotation;
            }
        }
        else
        {
            if (!m_Grounded && m_Rotation < m_TargetRotation)
            {
                m_TargetRotation = ((int)m_Rotation / 90) * 90 - 90;
                // std::cout << targetRotation << ' ';
            }
            else if (m_Rotation < m_TargetRotation)
            {
                m_Rotation = m_TargetRotation;
            }
        }

        if (m_YVelocity > TILE_SIZE / 2)
        {
            m_YVelocity = TILE_SIZE / 2;
        }
        break;
    case SHIP:
    {
        double shipUp = m_ShipUpAdder;
        double shipDown = m_ShipDownAdder;
        double lessHelpUp = 0.9;
        double lessHelpDown = 0.667;

        if (m_GravityMultiplier == -1)
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
            if (m_YVelocity > -7)
            {
                m_YVelocity -= shipUp * delta;
            }
            else
            {
                m_YVelocity -= shipUp * lessHelpUp * delta;
            }
        }
        else
        {
            if (m_YVelocity < -7)
            {
                m_YVelocity += shipDown * delta;
            }
            else
            {
                m_YVelocity += shipDown * lessHelpDown * delta;
            }
        }

        if (m_YVelocity < -80.0 / 3.0)
        {
            m_YVelocity = -80.0f / 3.0f;
        }
        else if (m_YVelocity > 64.0 / 3.0)
        {
            m_YVelocity = 64.0 / 3.0;
        }
        break;
    }
    }

    m_YVelocity *= m_GravityMultiplier;

    m_HazardHitbox.x += m_XVelocity * delta;
    m_HazardHitbox.y += m_YVelocity * delta;
    m_SolidHitbox.x += m_XVelocity * delta;
    m_SolidHitbox.y += m_YVelocity * delta;

    m_YVelocity /= m_GravityMultiplier;

    handleCollisions(objects);

    if (m_Gamemode == SHIP)
    {
        // i tried to make rotations accurate, but i couldn't get it to work

        if (m_YVelocity == 0.0)
        {
            if (m_Rotation < 0)
            {
                m_Rotation = std::clamp(m_Rotation + m_RotationAdder, -360.0, 0.0);
            }
            else if (m_Rotation > 0)
            {
                m_Rotation = std::clamp(m_Rotation - m_RotationAdder, 0.0, 360.0);
            }
        }
        else
        {
            m_Rotation = m_YVelocity * 2.0;
            m_Rotation /= std::clamp(5.0 / std::abs(m_YVelocity), 1.0, 3.0);
        }

        m_Rotation *= m_GravityMultiplier;
    }

    if (m_Pos.x - cameraPos.x > CAMERA_SCROLL)
    {
        cameraPos.x = m_Pos.x - CAMERA_SCROLL;
    }

    switch (m_Gamemode)
    {
    case CUBE:
        if (m_Pos.y - cameraPos.y < CAMERA_UP_SCROLL)
        {
            cameraPos.y = m_Pos.y - CAMERA_UP_SCROLL;
        }

        if (m_Pos.y - cameraPos.y > CAMERA_DOWN_SCROLL)
        {
            cameraPos.y = m_Pos.y - CAMERA_DOWN_SCROLL;
        }
        break;
    }

    if (m_Pos.y < -7 * BACKGROUND_SIZE)
    {
        die();
    }

    if (m_Grounded)
    {
        m_OrbBuffered = false;
    }

    m_PreviousPos = m_Pos;
}

void Player::handleCollisions(std::vector<GameObject> objects)
{   
    switch (m_Gamemode)
    {
    case CUBE:    
        if (m_HazardHitbox.y > HEIGHT - 300 - TILE_SIZE)
        {
            m_HazardHitbox.y = HEIGHT - 300 - TILE_SIZE;
            m_SolidHitbox.y = m_HazardHitbox.y + TILE_SIZE / 3;
            m_Grounded = true;
            m_YVelocity = 0;
        }
        break;
    case SHIP:
        if (m_HazardHitbox.y < m_Bounds.first)
        {
            m_HazardHitbox.y = m_Bounds.first;
            m_SolidHitbox.y = m_HazardHitbox.y + TILE_SIZE / 3;
            m_YVelocity = 0;
        }

        if (m_HazardHitbox.y + m_HazardHitbox.h > m_Bounds.second)
        {
            m_HazardHitbox.y = m_Bounds.second - m_HazardHitbox.h;
            m_SolidHitbox.y = m_HazardHitbox.y + TILE_SIZE / 3;
            m_Grounded = true;
            m_YVelocity = 0;
        }
        break;
    }

    for (GameObject &object : objects)
    {
        SDL_FRect intersect; // this is useless but im too lazy to remove it lmfao

        if (SDL_IntersectFRect(&m_HazardHitbox, object.getHitbox(), &intersect))
        {
            switch (object.getType())
            {
            case HAZARD:
                die();
                break;
            case BLOCK:
                if (!SDL_HasIntersectionF(&m_SolidHitbox, object.getHitbox()))
                {
                    if (m_GravityMultiplier == 1)
                    {
                        if (m_YVelocity > 0 && m_SolidHitbox.y + m_SolidHitbox.h < object.getHitbox()->y)
                        {
                            m_HazardHitbox.y = object.getPos().y - TILE_SIZE;
                            m_YVelocity = 0;
                        }
                    }
                    else
                    {
                        if (m_YVelocity > 0 && m_SolidHitbox.y > object.getHitbox()->y + object.getHitbox()->h)
                        {
                            m_HazardHitbox.y = object.getPos().y + TILE_SIZE;
                            m_YVelocity = 0;
                        }
                    }

                    if (m_Gamemode == SHIP)
                    {
                        if (m_YVelocity < 0 && m_SolidHitbox.y > object.getHitbox()->y + object.getHitbox()->h)
                        {
                            m_HazardHitbox.y = object.getPos().y + TILE_SIZE;
                            m_YVelocity = 0;
                        }
                    }
                    break;
                }
                die();
                break;
            case PAD:
                m_YVelocity = m_PadStrength;
                break;
            case ORB:
                if (m_OrbBuffered && (m_PressedOrbs.empty() || std::find(m_PressedOrbs.begin(), m_PressedOrbs.end(), object.getHitbox()) == m_PressedOrbs.end()))
                {
                    m_YVelocity = m_JumpStrength;
                    m_PressedOrbs.push_back(object.getHitbox());
                }
                break;
            case UPSIDE_DOWN_PORTAL:
                if (m_GravityMultiplier == -1)
                {
                    break;
                }

                m_YVelocity /= 2;
                m_GravityMultiplier = -1;

                if (m_YVelocity < 0)
                {
                    m_YVelocity *= -1;
                }
                break;
            case NORMAL_PORTAL:
                if (m_GravityMultiplier == 1)
                {
                    break;
                }

                m_YVelocity /= 2;
                m_GravityMultiplier = 1;

                if (m_YVelocity < 0)
                {
                    m_YVelocity *= -1;
                }
                break;
            case SHIP_PORTAL:
                m_Gamemode = SHIP;
                m_Rotation = 0;

                if (m_Gamemode != SHIP)
                {
                    m_YVelocity /= 2;
                }

                cameraPos.y = std::clamp((float)((((int)(object.getPos().y / TILE_SIZE)) - 3) * TILE_SIZE), -10000.0f, -260.0f);
                m_Bounds.first = cameraPos.y + 40;
                m_Bounds.second = m_Bounds.first + 10 * TILE_SIZE;
                break;
            case CUBE_PORTAL:
                m_Gamemode = CUBE;

                if (m_Gamemode != CUBE)
                {
                    m_YVelocity /= 2;
                }
                break;
            }
        }
    }

    m_Grounded = m_YVelocity == 0 && (!m_MouseHeld || m_Gamemode != SHIP);
    m_SolidHitbox.x = m_HazardHitbox.x + TILE_SIZE / 3;
    m_SolidHitbox.y = m_HazardHitbox.y + TILE_SIZE / 3;
    m_Pos.x = m_HazardHitbox.x;
    m_Pos.y = m_HazardHitbox.y;
}

void Player::render()
{
    switch (m_Gamemode)
    {
    case CUBE:
    {
        SDL_FRect dst = {m_Pos.x - cameraPos.x, m_Pos.y - cameraPos.y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopyExF(renderer, m_PlayerTexture, NULL, &dst, m_Rotation, NULL, SDL_FLIP_NONE);
        break;
    }
    case SHIP:
    {
        if (m_GravityMultiplier == 1)
        {
            SDL_FRect cubeDST = {m_Pos.x - cameraPos.x + TILE_SIZE / 4, m_Pos.y - cameraPos.y, TILE_SIZE * 3 / 5, TILE_SIZE * 3 / 5};
            SDL_FRect shipDST = {m_Pos.x - cameraPos.x - 14, m_Pos.y - cameraPos.y + 22, 132, 78};
            SDL_FPoint cubeCenter = {shipDST.x + shipDST.w / 2 - cubeDST.x, shipDST.y + shipDST.h / 2 - cubeDST.y};
            SDL_RenderCopyExF(renderer, m_PlayerTexture, NULL, &cubeDST, m_Rotation, &cubeCenter, SDL_FLIP_NONE);
            SDL_RenderCopyExF(renderer, m_ShipTexture, NULL, &shipDST, m_Rotation, NULL, SDL_FLIP_NONE);
        }
        else
        {
            SDL_FRect cubeDST = {m_Pos.x - cameraPos.x + TILE_SIZE / 4, m_Pos.y - cameraPos.y + TILE_SIZE * 2 / 5, TILE_SIZE * 3 / 5, TILE_SIZE * 3 / 5};
            SDL_FRect shipDST = {m_Pos.x - cameraPos.x - 14, m_Pos.y - cameraPos.y, 132, 78};
            SDL_FPoint cubeCenter = {shipDST.x + shipDST.w / 2 - cubeDST.x, shipDST.y + shipDST.h / 2 - cubeDST.y};
            SDL_RenderCopyExF(renderer, m_PlayerTexture, NULL, &cubeDST, m_Rotation, &cubeCenter, SDL_FLIP_VERTICAL);
            SDL_RenderCopyExF(renderer, m_ShipTexture, NULL, &shipDST, m_Rotation, NULL, SDL_FLIP_VERTICAL);
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
    m_Dead = true;
    m_DeadTimer = 90;
    Mix_HaltMusic();
    Mix_PlayChannel(-1, m_DeathSound, 0);
    m_YVelocity = 0;
}

bool Player::isDead()
{
    return m_Dead;
}

int Player::getGamemode()
{
    return m_Gamemode;
}

// SDL_FRect Player::getHazardHitbox()
// {
//     return hazardHitbox;
// }

// SDL_FRect Player::getSolidHitbox()
// {
//     return solidHitbox;
// }
