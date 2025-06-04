#include <SDL_image.h>
#include <iostream>
#include "Game.h"
#include "GameObject.h"

GameObject::GameObject(Game *game, ObjectType type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, bool xFlip, bool yFlip, SDL_Texture *objectTexture, SDL_Color color, int duration)
    : m_Game(game), m_Type(type), m_Rotation(rotation), m_Position(pos), m_Hitbox(hitbox), m_FlipX(xFlip), m_FlipY(yFlip), m_ObjectTexture(objectTexture), m_Color(color), m_Duration(duration) {
    
    m_Renderer = m_Game->getRenderer();
    SDL_QueryTexture(m_ObjectTexture, NULL, NULL, &m_Rect.w, &m_Rect.h);
    if (m_FlipX && m_FlipY) {
        m_FlipX = false;
        m_FlipY = false;
        m_Rotation += 2;
    }
}

void GameObject::activate() {
    if (m_Type != ObjectType::BG_TRIGGER && m_Type != ObjectType::G_TRIGGER) {
        std::cout << "You can't activate things that aren't triggers!\n";
        return;
    }
    // can't be activated more than once
    if (m_IsActivated) {
        return;
    }
    m_IsActivated = true;
    if (m_Type == ObjectType::BG_TRIGGER) {
        m_Game->getBackground().fade(m_Color.r, m_Color.g, m_Color.b, m_Duration);
    } else if (m_Type == ObjectType::G_TRIGGER) {
        m_Game->getGround().fade(m_Color.r, m_Color.g, m_Color.b, m_Duration);
    }
}

void GameObject::reset() {
    m_IsActivated = false;
}

SDL_FRect *GameObject::getHitbox() {
    return &m_Hitbox;
}

SDL_FPoint GameObject::getPos() {
    return m_Position;
}

float GameObject::getWidth() {
    return m_Rect.w;
}

float GameObject::getHeight() {
    return m_Rect.h;
}

ObjectType GameObject::getType() {
    return m_Type;
}

void GameObject::render() {
    m_Rect.x = static_cast<int>(m_Position.x) - m_Game->getCameraPosition().x;
    m_Rect.y = static_cast<int>(m_Position.y) - m_Game->getCameraPosition().y;

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (m_FlipX) {
        flip = SDL_FLIP_HORIZONTAL;
    } else if (m_FlipY) {
        flip = SDL_FLIP_VERTICAL;
    }
    SDL_RenderCopyEx(m_Renderer, m_ObjectTexture, NULL, &m_Rect, m_Rotation * 90.0, NULL, flip);

    // SDL_FRect temp = m_Hitbox;
    // temp.x -= m_Game->getCameraPosition().x;
    // temp.y -= m_Game->getCameraPosition().y;
    // SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 125);
    // SDL_RenderFillRectF(m_Renderer, &temp);
}
