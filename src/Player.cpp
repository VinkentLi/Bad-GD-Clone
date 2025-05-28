#include <algorithm>
#include <SDL_image.h>
#include "Player.hpp"
#include "Game.hpp"
#include "GameObject.hpp"

void Player::init(Game *game) {
    m_Game = game;
    m_Renderer = m_Game->getRenderer();
    m_PlayerTexture = IMG_LoadTexture(m_Renderer, "res/gfx/icon.png");
    m_ShipTexture = IMG_LoadTexture(m_Renderer, "res/gfx/ship.png");
    m_DeathSound = Mix_LoadWAV("res/sfx/deathSound.ogg");
}

void Player::reset() {
    m_Position = {-m_Game->TILE_SIZE, m_Game->getHeight() - 300.0f - m_Game->TILE_SIZE};
    m_PreviousPosition = m_Position;
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
    m_HazardHitbox = {
        m_Position.x, 
        m_Position.y, 
        static_cast<float>(m_Game->TILE_SIZE), 
        static_cast<float>(m_Game->TILE_SIZE)
    };
    m_SolidHitbox = {
        m_Position.x + m_Game->TILE_SIZE / 3, 
        m_Position.y + m_Game->TILE_SIZE / 3, 
        m_Game->TILE_SIZE / 3, 
        m_Game->TILE_SIZE / 3
    };
    m_IsGrounded = true;
    m_IsMouseHeld = false;
    m_HasBufferedOrb = false;
    m_IsDead = false;
    m_DeadTimer = 0;
    m_GravityMultiplier = 1;
    m_Gamemode = CUBE;
}

Player::~Player() {
    SDL_DestroyTexture(m_PlayerTexture);
    SDL_DestroyTexture(m_ShipTexture);
    Mix_FreeChunk(m_DeathSound);
}

void Player::update(float delta, bool isMouseHeld, std::vector<GameObject> &objects) {
    bool mouseClicked = (!m_IsMouseHeld && isMouseHeld);
    bool mouseReleased = (m_IsMouseHeld && !isMouseHeld);
    m_IsMouseHeld = isMouseHeld;
    if (m_IsDead) {
        m_DeadTimer -= delta;
        if (m_DeadTimer < 0) {
            m_Gamemode = CUBE;
            m_PressedOrbs.clear();
            m_IsDead = false;
            m_IsGrounded = true;
            m_HazardHitbox.x = -m_Game->TILE_SIZE;
            m_HazardHitbox.y = m_Game->getHeight() - 300.0f - m_Game->TILE_SIZE;
            m_SolidHitbox.x = m_HazardHitbox.x + m_Game->TILE_SIZE / 3;
            m_SolidHitbox.y = m_HazardHitbox.y + m_Game->TILE_SIZE / 3;
            m_Position = {m_HazardHitbox.x, m_HazardHitbox.y};
            m_Game->setCameraPosition({0, 0});
            m_Rotation = 0;
            m_TargetRotation = 0;
            m_GravityMultiplier = 1;
        }
        return;
    }
    switch (m_Gamemode) {
    case CUBE:
        if (mouseClicked) {
            m_HasBufferedOrb = true;
        } else if (mouseReleased) {
            m_HasBufferedOrb = false;
        }
        if (m_IsGrounded && m_IsMouseHeld) {
            m_YVelocity = m_JumpStrength;
            m_IsGrounded = false;
        }
        m_YVelocity += m_Gravity * delta;
        m_Rotation += m_GravityMultiplier == 1 ? m_RotationAdder * delta : -m_RotationAdder * delta;

        if (m_GravityMultiplier == 1) {
            if (!m_IsGrounded && m_Rotation > m_TargetRotation) {
                m_TargetRotation = ((int)m_Rotation / 90) * 90 + 90;
                // std::cout << targetRotation << ' ';
            } else if (m_Rotation > m_TargetRotation) {
                m_Rotation = m_TargetRotation;
            }
        } else {
            if (!m_IsGrounded && m_Rotation < m_TargetRotation) {
                m_TargetRotation = ((int)m_Rotation / 90) * 90 - 90;
                // std::cout << targetRotation << ' ';
            } else if (m_Rotation < m_TargetRotation) {
                m_Rotation = m_TargetRotation;
            }
        }

        if (m_YVelocity > m_Game->TILE_SIZE / 2) {
            m_YVelocity = m_Game->TILE_SIZE / 2;
        }
        break;
    case SHIP: {
        double shipUp = m_ShipUpAdder;
        double shipDown = m_ShipDownAdder;
        double lessHelpUp = 0.9;
        double lessHelpDown = 0.667;

        if (m_GravityMultiplier == -1) {
            double t1 = shipUp;
            shipUp = shipDown;
            shipDown = t1;
            double t2 = lessHelpUp;
            lessHelpUp = lessHelpDown;
            lessHelpDown = t2;
        }

        if (isMouseHeld) {
            if (m_YVelocity > -7) {
                m_YVelocity -= shipUp * delta;
            } else {
                m_YVelocity -= shipUp * lessHelpUp * delta;
            }
        } else {
            if (m_YVelocity < -7) {
                m_YVelocity += shipDown * delta;
            } else {
                m_YVelocity += shipDown * lessHelpDown * delta;
            }
        }

        if (m_YVelocity < -80.0 / 3.0) {
            m_YVelocity = -80.0f / 3.0f;
        } else if (m_YVelocity > 64.0 / 3.0) {
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

    if (m_Gamemode == SHIP) {
        // i tried to make rotations accurate, but i couldn't get it to work
        if (m_YVelocity == 0.0) {
            if (m_Rotation < 0) {
                m_Rotation = std::clamp(m_Rotation + m_RotationAdder, -360.0, 0.0);
            } else if (m_Rotation > 0) {
                m_Rotation = std::clamp(m_Rotation - m_RotationAdder, 0.0, 360.0);
            }
        } else {
            m_Rotation = m_YVelocity * 2.0;
            m_Rotation /= std::clamp(5.0 / std::abs(m_YVelocity), 1.0, 3.0);
        }
        m_Rotation *= m_GravityMultiplier;
    }
    constexpr int CAMERA_SCROLL = m_Game->TILE_SIZE * 6;
    const static int CAMERA_UP_SCROLL = m_Game->getHeight() / 4;
    const static int CAMERA_DOWN_SCROLL = m_Game->getHeight() - 400;
    
    if (m_Position.x - m_Game->getCameraPosition().x > CAMERA_SCROLL) {
        m_Game->setCameraX(m_Position.x - CAMERA_SCROLL);
    }

    if (m_Gamemode == CUBE) {
        if (m_Position.y - m_Game->getCameraPosition().y < CAMERA_UP_SCROLL) {
            m_Game->setCameraY(m_Position.y - CAMERA_UP_SCROLL);
        }
        if (m_Position.y - m_Game->getCameraPosition().y > CAMERA_DOWN_SCROLL) {
            m_Game->setCameraY(m_Position.y - CAMERA_DOWN_SCROLL);
        }
    }
    if (m_Position.y < -7 * m_Game->BACKGROUND_SIZE) {
        die();
    }
    if (m_IsGrounded) {
        m_HasBufferedOrb = false;
    }
    m_PreviousPosition = m_Position;
}

void Player::handleCollisions(std::vector<GameObject> &objects) {   
    switch (m_Gamemode) {
    case CUBE:    
        if (m_HazardHitbox.y > m_Game->getHeight() - 300 - m_Game->TILE_SIZE) {
            m_HazardHitbox.y = m_Game->getHeight() - 300 - m_Game->TILE_SIZE;
            m_SolidHitbox.y = m_HazardHitbox.y + m_Game->TILE_SIZE / 3;
            m_IsGrounded = true;
            m_YVelocity = 0;
        }
        break;
    case SHIP:
        if (m_HazardHitbox.y < m_Bounds.first) {
            m_HazardHitbox.y = m_Bounds.first;
            m_SolidHitbox.y = m_HazardHitbox.y + m_Game->TILE_SIZE / 3;
            m_YVelocity = 0;
        }
        if (m_HazardHitbox.y + m_HazardHitbox.h > m_Bounds.second) {
            m_HazardHitbox.y = m_Bounds.second - m_HazardHitbox.h;
            m_SolidHitbox.y = m_HazardHitbox.y + m_Game->TILE_SIZE / 3;
            m_IsGrounded = true;
            m_YVelocity = 0;
        }
        break;
    }

    for (GameObject &object : objects) {
        SDL_FRect intersect; // this is useless but im too lazy to remove it lmfao
        if (SDL_IntersectFRect(&m_HazardHitbox, object.getHitbox(), &intersect)) {
            switch (object.getType()) {
            case HAZARD:
                die();
                break;
            case BLOCK:
                if (!SDL_HasIntersectionF(&m_SolidHitbox, object.getHitbox())) {
                    if (m_GravityMultiplier == 1) {
                        if (m_YVelocity > 0 && m_SolidHitbox.y + m_SolidHitbox.h < object.getHitbox()->y) {
                            m_HazardHitbox.y = object.getPos().y - m_Game->TILE_SIZE;
                            m_YVelocity = 0;
                        }
                    } else {
                        if (m_YVelocity > 0 && m_SolidHitbox.y > object.getHitbox()->y + object.getHitbox()->h) {
                            m_HazardHitbox.y = object.getPos().y + m_Game->TILE_SIZE;
                            m_YVelocity = 0;
                        }
                    }
                    if (m_Gamemode == SHIP) {
                        if (m_YVelocity < 0 && m_SolidHitbox.y > object.getHitbox()->y + object.getHitbox()->h) {
                            m_HazardHitbox.y = object.getPos().y + m_Game->TILE_SIZE;
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
            case ORB: {
                const bool orbNotInPressedOrbs = std::find(m_PressedOrbs.begin(), m_PressedOrbs.end(), object.getHitbox()) == m_PressedOrbs.end();
                const bool orbHasNotBeenPressed = m_PressedOrbs.empty() || orbNotInPressedOrbs;
                if (m_HasBufferedOrb && orbHasNotBeenPressed) {
                    m_YVelocity = m_JumpStrength;
                    m_PressedOrbs.push_back(object.getHitbox());
                }
                break;
            }
            case UPSIDE_DOWN_PORTAL:
                if (m_GravityMultiplier == -1) {
                    break;
                }
                m_YVelocity /= 2;
                m_GravityMultiplier = -1;
                if (m_YVelocity < 0) {
                    m_YVelocity *= -1;
                }
                break;
            case NORMAL_PORTAL:
                if (m_GravityMultiplier == 1) {
                    break;
                }
                m_YVelocity /= 2;
                m_GravityMultiplier = 1;
                if (m_YVelocity < 0) {
                    m_YVelocity *= -1;
                }
                break;
            case SHIP_PORTAL:
                m_Gamemode = SHIP;
                m_Rotation = 0;

                if (m_Gamemode != SHIP) {
                    m_YVelocity /= 2;
                }
                m_Game->setCameraY(std::clamp(
                    // I forgot what this does lmfao
                    static_cast<float>(((static_cast<int>(object.getPos().y / m_Game->TILE_SIZE)) - 3) * m_Game->TILE_SIZE),
                    -10000.0f, 
                    -260.0f
                ));
                m_Bounds.first = m_Game->getCameraPosition().y + 40;
                m_Bounds.second = m_Bounds.first + 10 * m_Game->TILE_SIZE;
                break;
            case CUBE_PORTAL:
                m_Gamemode = CUBE;
                if (m_Gamemode != CUBE) {
                    m_YVelocity /= 2;
                }
                break;
            }
        }
    }

    m_IsGrounded = m_YVelocity == 0 && (!m_IsMouseHeld || m_Gamemode != SHIP);
    m_SolidHitbox.x = m_HazardHitbox.x + m_Game->TILE_SIZE / 3;
    m_SolidHitbox.y = m_HazardHitbox.y + m_Game->TILE_SIZE / 3;
    m_Position.x = m_HazardHitbox.x;
    m_Position.y = m_HazardHitbox.y;
}

void Player::render() {
    switch (m_Gamemode) {
    case CUBE: {
        SDL_FRect dst = {
            m_Position.x - m_Game->getCameraPosition().x, 
            m_Position.y - m_Game->getCameraPosition().y,
            m_Game->TILE_SIZE, 
            m_Game->TILE_SIZE
        };
        SDL_RenderCopyExF(m_Renderer, m_PlayerTexture, NULL, &dst, m_Rotation, NULL, SDL_FLIP_NONE);
        break;
    }
    case SHIP: {
        if (m_GravityMultiplier == 1) {
            SDL_FRect cubeDST = {
                m_Position.x - m_Game->getCameraPosition().x + m_Game->TILE_SIZE / 4,
                m_Position.y - m_Game->getCameraPosition().y, 
                m_Game->TILE_SIZE * 3 / 5, 
                m_Game->TILE_SIZE * 3 / 5
            };
            SDL_FRect shipDST = {
                m_Position.x - m_Game->getCameraPosition().x - 14, 
                m_Position.y - m_Game->getCameraPosition().y + 22, 
                132, 
                78
            };
            SDL_FPoint cubeCenter = {shipDST.x + shipDST.w / 2 - cubeDST.x, shipDST.y + shipDST.h / 2 - cubeDST.y};
            SDL_RenderCopyExF(m_Renderer, m_PlayerTexture, NULL, &cubeDST, m_Rotation, &cubeCenter, SDL_FLIP_NONE);
            SDL_RenderCopyExF(m_Renderer, m_ShipTexture, NULL, &shipDST, m_Rotation, NULL, SDL_FLIP_NONE);
        } else {
            SDL_FRect cubeDST = {
                m_Position.x - m_Game->getCameraPosition().x + m_Game->TILE_SIZE / 4,
                m_Position.y - m_Game->getCameraPosition().y + m_Game->TILE_SIZE * 2 / 5, 
                m_Game->TILE_SIZE * 3 / 5, 
                m_Game->TILE_SIZE * 3 / 5
            };
            SDL_FRect shipDST = {
                m_Position.x - m_Game->getCameraPosition().x - 14, 
                m_Position.y - m_Game->getCameraPosition().y, 
                132, 
                78
            };
            SDL_FPoint cubeCenter = {shipDST.x + shipDST.w / 2 - cubeDST.x, shipDST.y + shipDST.h / 2 - cubeDST.y};
            SDL_RenderCopyExF(m_Renderer, m_PlayerTexture, NULL, &cubeDST, m_Rotation, &cubeCenter, SDL_FLIP_VERTICAL);
            SDL_RenderCopyExF(m_Renderer, m_ShipTexture, NULL, &shipDST, m_Rotation, NULL, SDL_FLIP_VERTICAL);
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

void Player::die() {
    m_IsDead = true;
    m_DeadTimer = 90;
    Mix_HaltMusic();
    Mix_PlayChannel(-1, m_DeathSound, 0);
    m_YVelocity = 0;
}

bool Player::isDead() {
    return m_IsDead;
}

int Player::getGamemode() {
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
