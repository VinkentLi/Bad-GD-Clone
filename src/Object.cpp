#include <SDL_image.h>
#include <iostream>
#include "Game.h"
#include "Object.h"

Object::Object(Game *game, ObjectType type, int rotation, SDL_FPoint pos, std::optional<SDL_FRect> hitbox, bool xFlip, bool yFlip, SDL_Texture *objectTexture)
    : m_Game(game), m_Type(type), m_Rotation(rotation), m_Position(pos), m_Hitbox(hitbox), m_FlipX(xFlip), m_FlipY(yFlip), m_ObjectTexture(objectTexture) {
    
    m_Renderer = m_Game->getRenderer();
    if (m_ObjectTexture != nullptr) {
        SDL_QueryTexture(m_ObjectTexture, NULL, NULL, &m_Width, &m_Height);
    } else {
        m_Width = 0;
        m_Height = 0;
    }
    // in sdl you can't do both flips at the same time ¯\_(ツ)_/¯
    if (m_FlipX && m_FlipY) {
        m_FlipX = false;
        m_FlipY = false;
        m_Rotation += 2;
    }
}

std::optional<SDL_FRect> Object::getHitbox() const {
    return m_Hitbox;
}

SDL_FPoint Object::getPos() const {
    return m_Position;
}

int Object::getWidth() const {
    return m_Width;
}

int Object::getHeight() const {
    return m_Height;
}

ObjectType Object::getType() const {
    return m_Type;
}

void Object::render() const {
    // some objects don't have textures
    if (m_ObjectTexture == nullptr) {
        return;
    }
    SDL_Rect dst = {
        .x = static_cast<int>(m_Position.x - m_Game->getCameraPosition().x),
        .y = static_cast<int>(m_Position.y - m_Game->getCameraPosition().y),
        .w = m_Width,
        .h = m_Height
    };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (m_FlipX) {
        flip = SDL_FLIP_HORIZONTAL;
    } else if (m_FlipY) {
        flip = SDL_FLIP_VERTICAL;
    }
    SDL_RenderCopyEx(m_Renderer, m_ObjectTexture, NULL, &dst, m_Rotation * 90.0, NULL, flip);
    
    // this just draws hitboxes for debugging
    // SDL_FRect temp = m_Hitbox;
    // temp.x -= m_Game->getCameraPosition().x;
    // temp.y -= m_Game->getCameraPosition().y;
    // SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 125);
    // SDL_RenderFillRectF(m_Renderer, &temp);
}
