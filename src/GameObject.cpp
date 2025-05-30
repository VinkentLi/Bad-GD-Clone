#include <SDL_image.h>
#include <iostream>
#include "Game.h"
#include "GameObject.h"

GameObject::GameObject(Game *game, ObjectType type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, SDL_Texture *objectTexture)
    : m_Game(game), m_Type(type), m_Rotation(rotation), m_Position(pos), m_Hitbox(hitbox), m_ObjectTexture(objectTexture) {
    
    m_Renderer = m_Game->getRenderer();
}

SDL_FRect *GameObject::getHitbox() {
    return &m_Hitbox;
}

SDL_FPoint GameObject::getPos() {
    return m_Position;
}

ObjectType GameObject::getType() {
    return m_Type;
}

void GameObject::render() {
    SDL_Rect dst;
    dst.x = static_cast<int>(m_Position.x) - m_Game->getCameraPosition().x;
    dst.y = static_cast<int>(m_Position.y) - m_Game->getCameraPosition().y;
    SDL_QueryTexture(m_ObjectTexture, NULL, NULL, &dst.w, &dst.h);

    if (SDL_RenderCopyEx(m_Renderer, m_ObjectTexture, NULL, &dst, m_Rotation * 90.0, NULL, SDL_FLIP_NONE) != 0) {
        std::cerr << "GameObject failed to render texture! " << SDL_GetError() << std::endl;
    }

    // SDL_FRect temp = hitbox;
    // temp.x -= cameraPos.x;
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
    // SDL_RenderFillRectF(renderer, &temp);
}
