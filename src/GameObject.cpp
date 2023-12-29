#include "GameObject.hpp"

GameObject::GameObject(int type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, const char *texturePath)
    : m_type(type), m_rotation(rotation), m_pos(pos), m_hitbox(hitbox) {
    m_object_texture = IMG_LoadTexture(renderer, texturePath);
    
    if (m_object_texture == NULL) {
        std::cerr << "Failed to load m_object_texture! " << SDL_GetError() << std::endl;
    }
}

SDL_FRect *GameObject::get_hitbox() {
    return &m_hitbox;
}

SDL_FPoint GameObject::get_pos() {
    return m_pos;
}

int GameObject::get_type() {
    return m_type;
}

void GameObject::render() {
    SDL_Rect dst;
    dst.x = (int) (m_pos.x) - camera_pos.x;
    dst.y = (int) (m_pos.y) - camera_pos.y;
    dst.w = TILE_SIZE;
    dst.h = TILE_SIZE;

    if (m_type == SHIP_PORTAL ||
        m_type == CUBE_PORTAL ||
        m_type == UPSIDE_DOWN_PORTAL ||
        m_type == NORMAL_PORTAL) {
        dst.h *= 3;
    }

    if (SDL_RenderCopyEx(renderer, m_object_texture, NULL, &dst, m_rotation * 90.0, NULL, SDL_FLIP_NONE) != 0) {
        std::cerr << "GameObject failed to render texture! " << SDL_GetError() << std::endl;
    }

    // SDL_FRect temp = m_hitbox;
    // temp.x -= cameraPos.x;
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
    // SDL_RenderFillRectF(renderer, &temp);
}
