#include <SDL_image.h>
#include <iostream>
#include "Game.hpp"
#include "GameObject.hpp"

GameObject::GameObject(Game *game, int type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, const char *texturePath)
    : m_Game(game), m_Type(type), m_Rotation(rotation), m_Position(pos), m_Hitbox(hitbox) {
    
    m_Renderer = m_Game->getRenderer();
    m_ObjectTexture = IMG_LoadTexture(m_Renderer, texturePath);
    if (m_ObjectTexture == NULL) {
        std::cerr << "Failed to load objectTexture! " << SDL_GetError() << std::endl;
    }
}

SDL_FRect *GameObject::getHitbox() {
    return &m_Hitbox;
}

SDL_FPoint GameObject::getPos() {
    return m_Position;
}

int GameObject::getType() {
    return m_Type;
}

void GameObject::render() {
    SDL_Rect dst;
    dst.x = (int) (m_Position.x) - m_Game->getCameraPosition().x;
    dst.y = (int) (m_Position.y) - m_Game->getCameraPosition().y;
    dst.w = m_Game->TILE_SIZE;
    dst.h = m_Game->TILE_SIZE;

    if (m_Type == SHIP_PORTAL ||
        m_Type == CUBE_PORTAL ||
        m_Type == UPSIDE_DOWN_PORTAL ||
        m_Type == NORMAL_PORTAL) {

        dst.h *= 3;
    }
    if (SDL_RenderCopyEx(m_Renderer, m_ObjectTexture, NULL, &dst, m_Rotation * 90.0, NULL, SDL_FLIP_NONE) != 0) {
        std::cerr << "GameObject failed to render texture! " << SDL_GetError() << std::endl;
    }

    // SDL_FRect temp = hitbox;
    // temp.x -= cameraPos.x;
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
    // SDL_RenderFillRectF(renderer, &temp);
}
