#include <algorithm>
#include <SDL_image.h>
#include <cmath>
#include <iostream>
#include "Player.h"
#include "PlayingState.h"
#include "Game.h"
#include "GameObject.h"

void Player::init(Game *game) {
    m_Game = game;
    m_Renderer = m_Game->getRenderer();
    m_PlayerTexture = IMG_LoadTexture(m_Renderer, "res/gfx/icon.png");
    if (m_PlayerTexture == nullptr) {
        std::cerr << "Failed to load icon.png! " << SDL_GetError() << std::endl;
    }
    m_ShipTexture = IMG_LoadTexture(m_Renderer, "res/gfx/ship.png");
    if (m_ShipTexture == nullptr) {
        std::cerr << "Failed to load ship.png! " << SDL_GetError() << std::endl;
    }
    SDL_QueryTexture(m_ShipTexture, NULL, NULL, &m_ShipWidth, &m_ShipHeight);
    m_CheckpointTexture = IMG_LoadTexture(m_Renderer, "res/gfx/checkpoint.png");
    if (m_CheckpointTexture == nullptr) {
        std::cerr << "Failed to load checkpoint.png! " << SDL_GetError() << std::endl;
    }
    m_DeathSound = Mix_LoadWAV("res/sfx/deathSound.ogg");
    if (m_DeathSound == nullptr) {
        std::cerr << "Failed to load deathSound.ogg! " << SDL_GetError() << std::endl;
    }
}

void Player::reset() {
    m_Position = {-m_Game->TILE_SIZE, static_cast<float>(m_Game->getHeight() - 4*m_Game->TILE_SIZE)};
    m_PreviousPosition = m_Position;
    m_XVelocity = 20.772;
    m_YVelocity = 0;
    m_JumpStrength = -44.72;
    m_PadStrength = -64;
    m_Gravity = 3.456;
    m_RotationAdder = 6.92308;
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
    m_IsZHeld = false;
    m_IsXHeld = false;
    m_HasBufferedOrb = false;
    m_IsDead = false;
    m_DeadTimer = 0;
    m_GravityMultiplier = 1;
    m_Gamemode = Gamemode::CUBE;
    m_Checkpoints.clear();
    for (GameObject &trigger : PlayingState::get()->getTriggers()) {
        trigger.reset();
    }
}

Player::~Player() {
    SDL_DestroyTexture(m_PlayerTexture);
    SDL_DestroyTexture(m_ShipTexture);
    SDL_DestroyTexture(m_CheckpointTexture);
    Mix_FreeChunk(m_DeathSound);
}

void Player::update(float deltaTime) {
    bool mouseClicked = (!m_IsMouseHeld && m_Game->isMouseHeld());
    bool mouseReleased = (m_IsMouseHeld && !m_Game->isMouseHeld());

    if (PlayingState::get()->isInPractice()) {
        updatePractice();
    }

    m_IsMouseHeld = m_Game->isMouseHeld();
    if (m_IsDead) {
        m_DeadTimer -= deltaTime;
        if (m_DeadTimer < 0) {
            respawn();
        }
        return;
    }

    updatePhysics(deltaTime, mouseClicked, mouseReleased);
    handleCollisions(PlayingState::get()->getObjects());
    activateTriggers(PlayingState::get()->getTriggers());

    switch (m_Gamemode) {
    case Gamemode::CUBE:
        updateCubeRotation(deltaTime);
        break;
    case Gamemode::SHIP:
        updateShipRotation(deltaTime);
        break;
    }
    
    scrollCamera();

    if (m_Position.y < PlayingState::get()->getMinY()) {
        die();
    }
    if (m_IsGrounded) {
        m_HasBufferedOrb = false;
    }
    m_PreviousPosition = m_Position;
}

void Player::updatePractice() {
    bool zReleased = m_IsZHeld && !m_Game->isZHeld();
    bool xReleased = m_IsXHeld && !m_Game->isXHeld();
    m_IsZHeld = m_Game->isZHeld();
    m_IsXHeld = m_Game->isXHeld();

    if (zReleased) {
        Background &background = m_Game->getBackground();
        Ground &ground = m_Game->getGround();
        m_Checkpoints.push_back(Checkpoint {
            m_Position,
            m_Game->getCameraPosition(),
            m_YVelocity,
            m_Rotation,
            m_TargetRotation,
            m_GravityMultiplier,
            m_Gamemode,
            background.getColor(),
            ground.getColor(),
            background.getTargetColor(),
            ground.getTargetColor(),
            background.getFadeTime(),
            ground.getFadeTime()
        });
    }
    if (xReleased && !m_Checkpoints.empty()) {
        m_Checkpoints.pop_back();
    }
}

void Player::respawn() {
    m_Gamemode = Gamemode::CUBE;
    m_PressedOrbs.clear();
    m_IsDead = false;
    m_IsGrounded = false;
    Background &background = m_Game->getBackground();
    Ground &ground = m_Game->getGround();
    if (!PlayingState::get()->isInPractice() || m_Checkpoints.empty()) {
        m_HazardHitbox.x = -m_Game->TILE_SIZE;
        m_HazardHitbox.y = m_Game->getHeight() - 4 * m_Game->TILE_SIZE;
        m_Game->setCameraPosition({0, 0});
        m_Rotation = 0;
        m_TargetRotation = 0;
        m_GravityMultiplier = 1;
        m_YVelocity = 0;
        for (GameObject &trigger : PlayingState::get()->getTriggers()) {
            trigger.reset();
        }
        SDL_Color initialBackground = PlayingState::get()->getInitialBackground();
        SDL_Color initialGround = PlayingState::get()->getInitialGround();
        background.fade(initialBackground.r, initialBackground.g, initialBackground.b, 0);
        ground.fade(initialGround.r, initialGround.g, initialGround.b, 0);
    } else {
        const Checkpoint &latestCheckpoint = m_Checkpoints.back();
        m_HazardHitbox.x = latestCheckpoint.position.x;
        m_HazardHitbox.y = latestCheckpoint.position.y;
        m_Game->setCameraPosition(latestCheckpoint.cameraPosition);
        m_Rotation = latestCheckpoint.rotation;
        m_TargetRotation = latestCheckpoint.targetRotation;
        m_GravityMultiplier = latestCheckpoint.gravityMultiplier;
        m_YVelocity = latestCheckpoint.yVelocity;
        m_Gamemode = latestCheckpoint.gamemode;
        background.setColor(latestCheckpoint.backgroundColor);
        ground.setColor(latestCheckpoint.groundColor);
        background.setTargetColor(latestCheckpoint.backgroundTargetColor);
        ground.setTargetColor(latestCheckpoint.groundTargetColor);
        background.setFadeTime(latestCheckpoint.backgroundFadeTime);
        ground.setFadeTime(latestCheckpoint.groundFadeTime);
        for (GameObject &trigger : PlayingState::get()->getTriggers()) {
            bool passedBeforeCheckpoint = m_HazardHitbox.x > trigger.getPos().x + m_Game->TILE_SIZE/2;
            if (!passedBeforeCheckpoint) {
                trigger.reset();
            }
        }
    }
    m_SolidHitbox.x = m_HazardHitbox.x + m_Game->TILE_SIZE / 3;
    m_SolidHitbox.y = m_HazardHitbox.y + m_Game->TILE_SIZE / 3;
    m_Position = { m_HazardHitbox.x, m_HazardHitbox.y };
}

void Player::updatePhysics(float deltaTime, bool mouseClicked, bool mouseReleased) {
    switch (m_Gamemode) {
    case Gamemode::CUBE:
        if (mouseClicked) {
            m_HasBufferedOrb = true;
        } else if (mouseReleased) {
            m_HasBufferedOrb = false;
        }
        if (m_IsGrounded && m_IsMouseHeld) {
            m_YVelocity = m_JumpStrength * m_GravityMultiplier;
            m_IsGrounded = false;
        }
        m_YVelocity += m_Gravity * m_GravityMultiplier * deltaTime;
        m_YVelocity = std::clamp(m_YVelocity, static_cast<double>(-m_Game->TILE_SIZE/2), static_cast<double>(m_Game->TILE_SIZE/2));
        break;
    case Gamemode::SHIP: {
        // help from https://github.com/Open-GD/OpenGD
        float shipAccel = m_Gravity * 0.32;
        if (m_IsMouseHeld) {
            shipAccel *= -1.25;
            if (m_YVelocity * m_GravityMultiplier > -5*m_Gravity) {
                shipAccel *= 1.25;
            }
        } else if (m_YVelocity * m_GravityMultiplier <= -5*m_Gravity) {
            shipAccel *= 1.5;
        }
        m_YVelocity += shipAccel * m_GravityMultiplier * deltaTime;
        m_YVelocity = std::clamp(m_YVelocity / m_GravityMultiplier, -32.0, 25.6) * m_GravityMultiplier;
        break;
    }
    }
    m_HazardHitbox.x += m_XVelocity * deltaTime;
    m_HazardHitbox.y += m_YVelocity * deltaTime * 0.9; // why the fuck does gd multiply by 0.9?
    m_SolidHitbox.x += m_XVelocity * deltaTime;
    m_SolidHitbox.y += m_YVelocity * deltaTime * 0.9;
}

void Player::handleCollisions(std::vector<GameObject> &objects) {   
    collideWithGround();

    for (GameObject &object : objects) {
        SDL_FRect intersect; // this is useless but im too lazy to remove it lmfao
        if (SDL_IntersectFRect(&m_HazardHitbox, object.getHitbox(), &intersect)) {
            collideWithObject(object);
        }
    }
    // update some stuff
    m_IsGrounded = m_YVelocity == 0 && (!m_IsMouseHeld || m_Gamemode != Gamemode::SHIP);
    m_SolidHitbox.x = m_HazardHitbox.x + m_Game->TILE_SIZE / 3;
    m_SolidHitbox.y = m_HazardHitbox.y + m_Game->TILE_SIZE / 3;
    m_Position.x = m_HazardHitbox.x;
    m_Position.y = m_HazardHitbox.y;
}

void Player::collideWithObject(GameObject &object) {
    switch (object.getType()) {
    case ObjectType::HAZARD:
        die();
        break;
    case ObjectType::BLOCK:
        if (SDL_HasIntersectionF(&m_SolidHitbox, object.getHitbox())) {
            die();
        } else {
            snapToObject(object);
        }
        break;
    case ObjectType::PAD:
        m_YVelocity = m_PadStrength * m_GravityMultiplier;
        break;
    case ObjectType::ORB: {
        const bool orbNotInPressedOrbs = std::find(m_PressedOrbs.begin(), m_PressedOrbs.end(), object.getHitbox()) == m_PressedOrbs.end();
        const bool orbHasNotBeenPressed = m_PressedOrbs.empty() || orbNotInPressedOrbs;
        if (m_HasBufferedOrb && orbHasNotBeenPressed) {
            m_YVelocity = m_JumpStrength * m_GravityMultiplier;
            m_PressedOrbs.push_back(object.getHitbox());
        }
        break;
    }
    case ObjectType::UPSIDE_DOWN_PORTAL:
        if (m_GravityMultiplier == -1) {
            break;
        }
        m_YVelocity /= 2;
        m_GravityMultiplier = -1;
        break;
    case ObjectType::NORMAL_PORTAL:
        if (m_GravityMultiplier == 1) {
            break;
        }
        m_YVelocity /= 2;
        m_GravityMultiplier = 1;
        break;
    case ObjectType::SHIP_PORTAL: {
        if (m_Gamemode != Gamemode::SHIP) {
            m_YVelocity /= 2;
            m_Rotation = 0;
        }
        m_Gamemode = Gamemode::SHIP;
        setShipBounds(object);
        break;
    }
    case ObjectType::CUBE_PORTAL:
        m_Gamemode = Gamemode::CUBE;
        if (m_Gamemode != Gamemode::CUBE) {
            m_YVelocity /= 2;
        }
        break;
    default:
        break;
    }
}

void Player::collideWithGround() {
    switch (m_Gamemode) {
    case Gamemode::CUBE:    
        if (m_HazardHitbox.y > m_Game->getHeight() - 4 * m_Game->TILE_SIZE) {
            m_HazardHitbox.y = m_Game->getHeight() - 4 * m_Game->TILE_SIZE;
            m_SolidHitbox.y = m_HazardHitbox.y + m_Game->TILE_SIZE / 3;
            m_IsGrounded = true;
            m_YVelocity = 0;
        }
        break;
    case Gamemode::SHIP:
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
}

void Player::snapToObject(GameObject &object) {
    if (m_GravityMultiplier == 1 || m_Gamemode == Gamemode::SHIP) {
        if (m_YVelocity > 0 && m_SolidHitbox.y + m_SolidHitbox.h < object.getHitbox()->y) {
            m_HazardHitbox.y = object.getPos().y - m_Game->TILE_SIZE;
            m_YVelocity = 0;
        }
    }
    if (m_GravityMultiplier == -1 || m_Gamemode == Gamemode::SHIP) {
        if (m_YVelocity < 0 && m_SolidHitbox.y > object.getHitbox()->y + object.getHitbox()->h) {
            m_HazardHitbox.y = object.getPos().y + object.getHitbox()->h;
            m_YVelocity = 0;
        }
    }
}

void Player::setShipBounds(GameObject &shipPortal) {
    const int BOUNDS_HEIGHT = (m_Game->getHeight() - 10*m_Game->TILE_SIZE)/2;
    m_Game->setCameraY(std::clamp(
        // why does gd do this lmao
        (std::round(shipPortal.getPos().y/m_Game->TILE_SIZE)-3)*m_Game->TILE_SIZE - BOUNDS_HEIGHT,
        -10000.0f, 
        static_cast<float>(-3*m_Game->TILE_SIZE + BOUNDS_HEIGHT)
    ));
    m_Bounds.first = m_Game->getCameraPosition().y + BOUNDS_HEIGHT;
    m_Bounds.second = m_Bounds.first + 10 * m_Game->TILE_SIZE;
}

void Player::activateTriggers(std::vector<GameObject> &triggers) {
    for (GameObject &trigger : triggers) {
        bool passedTrigger = m_Position.x > trigger.getPos().x + m_Game->TILE_SIZE/2;
        if (passedTrigger) {
            trigger.activate();
        }
    }
}

void Player::updateCubeRotation(float deltaTime) {
    m_Rotation += m_GravityMultiplier == 1 ? m_RotationAdder * deltaTime : -m_RotationAdder * deltaTime;
    m_Rotation = fmod(m_Rotation, 360.0);
    if (m_GravityMultiplier == 1) {
        if (!m_IsGrounded && m_Rotation > m_TargetRotation) {
            m_TargetRotation = std::trunc(m_Rotation / 90) * 90 + 90;
        } else if (m_Rotation > m_TargetRotation) {
            m_Rotation = m_TargetRotation;
        }
    } else {
        if (!m_IsGrounded && m_Rotation < m_TargetRotation) {
            m_TargetRotation = std::trunc(m_Rotation / 90) * 90 - 90;
            // std::cout << targetRotation << ' ';
        } else if (m_Rotation < m_TargetRotation) {
            m_Rotation = m_TargetRotation;
        }
    }
    m_TargetRotation = fmod(m_TargetRotation, 360.0);
}

void Player::updateShipRotation(float deltaTime) {
    if (m_Gamemode == Gamemode::SHIP) {
        // special thanks to https://github.com/Open-GD/OpenGD for this
        if (pow(m_YVelocity, 2) + pow(m_XVelocity, 2) >= 1.2) {
            float target = std::atan2(m_YVelocity, m_XVelocity) * 180.0 / M_PI;
            // exponential interpolation
            m_Rotation += (target - m_Rotation) * (1.0 - pow(0.85, deltaTime));
        }
    }
}

void Player::scrollCamera() {
    const static int CAMERA_SCROLL = m_Game->TILE_SIZE * 6;
    const static int CAMERA_UP_SCROLL = m_Game->getHeight() / 4;
    const static int CAMERA_DOWN_SCROLL = m_Game->getHeight() - 4*m_Game->TILE_SIZE;
    
    if (m_Position.x - m_Game->getCameraPosition().x > CAMERA_SCROLL) {
        m_Game->setCameraX(m_Position.x - CAMERA_SCROLL);
    }

    if (m_Gamemode == Gamemode::CUBE) {
        if (m_Position.y - m_Game->getCameraPosition().y < CAMERA_UP_SCROLL) {
            m_Game->setCameraY(m_Position.y - CAMERA_UP_SCROLL);
        }
        if (m_Position.y - m_Game->getCameraPosition().y > CAMERA_DOWN_SCROLL) {
            m_Game->setCameraY(m_Position.y - CAMERA_DOWN_SCROLL);
        }
    }
}

void Player::render() {
    for (Checkpoint &checkpoint : m_Checkpoints) {
        SDL_Rect rect = { 
            static_cast<int>(checkpoint.position.x - m_Game->getCameraPosition().x), 
            static_cast<int>(checkpoint.position.y - m_Game->getCameraPosition().y),
            m_Game->TILE_SIZE, 
            m_Game->TILE_SIZE 
        };
        SDL_RenderCopy(m_Renderer, m_CheckpointTexture, NULL, &rect);
    }

    switch (m_Gamemode) {
    case Gamemode::CUBE: {
        renderCube();
        break;
    }
    case Gamemode::SHIP: 
        renderShip();
        break;
    }

    // render hitboxes
    // SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 125);
    // SDL_FRect temp = m_HazardHitbox;
    // temp.x -= m_Game->getCameraPosition().x;
    // temp.y -= m_Game->getCameraPosition().y;
    // SDL_RenderFillRectF(m_Renderer, &temp);
    // SDL_SetRenderDrawColor(m_Renderer, 0, 0, 255, 125);
    // SDL_FRect temp2 = m_SolidHitbox;
    // temp2.x -= m_Game->getCameraPosition().x;
    // temp2.y -= m_Game->getCameraPosition().y;
    // SDL_RenderFillRectF(m_Renderer, &temp2);
}

void Player::renderCube() {
    SDL_FRect dst = {
        m_Position.x - m_Game->getCameraPosition().x, 
        m_Position.y - m_Game->getCameraPosition().y,
        m_Game->TILE_SIZE, 
        m_Game->TILE_SIZE
    };
    SDL_RenderCopyExF(m_Renderer, m_PlayerTexture, NULL, &dst, m_Rotation, NULL, SDL_FLIP_NONE);
}

void Player::renderShip() {
    int xDisplacement = (m_ShipWidth - m_Game->TILE_SIZE)/2;
    SDL_FPoint cameraPosition = m_Game->getCameraPosition();
    float cubeX = m_Position.x - cameraPosition.x + m_Game->TILE_SIZE*6/25;
    float cubeY = m_Position.y - cameraPosition.y + m_Game->TILE_SIZE/20;
    float shipX = m_Position.x - cameraPosition.x - xDisplacement;
    float shipY = m_Position.y - cameraPosition.y + m_Game->TILE_SIZE - m_ShipHeight;
    if (m_GravityMultiplier == -1) {
        cubeY = m_Position.y - cameraPosition.y + m_Game->TILE_SIZE*2/5;
        shipY = m_Position.y - cameraPosition.y;
    }
    SDL_FRect cubeDST = {
        cubeX,
        cubeY, 
        m_Game->TILE_SIZE*14/25, 
        m_Game->TILE_SIZE*14/25
    };
    SDL_FRect shipDST = {
        shipX, 
        shipY, 
        static_cast<float>(m_ShipWidth), 
        static_cast<float>(m_ShipHeight)
    };
    SDL_FPoint cubeCenter = {shipDST.x + shipDST.w / 2 - cubeDST.x, shipDST.y + shipDST.h / 2 - cubeDST.y};
    if (m_GravityMultiplier == 1) {
        SDL_RenderCopyExF(m_Renderer, m_PlayerTexture, NULL, &cubeDST, m_Rotation, &cubeCenter, SDL_FLIP_NONE);
        SDL_RenderCopyExF(m_Renderer, m_ShipTexture, NULL, &shipDST, m_Rotation, NULL, SDL_FLIP_NONE);
    } else {
        SDL_RenderCopyExF(m_Renderer, m_PlayerTexture, NULL, &cubeDST, m_Rotation, &cubeCenter, SDL_FLIP_VERTICAL);
        SDL_RenderCopyExF(m_Renderer, m_ShipTexture, NULL, &shipDST, m_Rotation, NULL, SDL_FLIP_VERTICAL);
    }
}

void Player::die() {
    m_IsDead = true;
    m_DeadTimer = 90;
    if (!PlayingState::get()->isInPractice()) {
        Mix_HaltMusic();
    }
    Mix_PlayChannel(-1, m_DeathSound, 0);
    m_YVelocity = 0;
}

bool Player::isDead() {
    return m_IsDead;
}

Gamemode Player::getGamemode() {
    return m_Gamemode;
}
