#include "GameObject.hpp"

GameObject::GameObject(int type, int rotation, SDL_FPoint pos, SDL_FRect hitbox, const char *texturePath)
    : m_Type(type), m_Rotation(rotation), m_Pos(pos), m_Hitbox(hitbox)
{
    m_ObjectTexture = IMG_LoadTexture(renderer, texturePath);
    
    if (m_ObjectTexture == NULL)
    {
        std::cerr << "Failed to load objectTexture! " << SDL_GetError() << std::endl;
    }
}

SDL_FRect *GameObject::getHitbox()
{
    return &m_Hitbox;
}

SDL_FPoint GameObject::getPos()
{
    return m_Pos;
}

int GameObject::getType()
{
    return m_Type;
}

void GameObject::render()
{
    SDL_Rect dst;
    dst.x = (int) (m_Pos.x) - cameraPos.x;
    dst.y = (int) (m_Pos.y) - cameraPos.y;
    dst.w = TILE_SIZE;
    dst.h = TILE_SIZE;

    if (m_Type == SHIP_PORTAL ||
        m_Type == CUBE_PORTAL ||
        m_Type == UPSIDE_DOWN_PORTAL ||
        m_Type == NORMAL_PORTAL)
    {
        dst.h *= 3;
    }

    if (SDL_RenderCopyEx(renderer, m_ObjectTexture, NULL, &dst, m_Rotation * 90.0, NULL, SDL_FLIP_NONE) != 0)
    {
        std::cerr << "GameObject failed to render texture! " << SDL_GetError() << std::endl;
    }

    // SDL_FRect temp = hitbox;
    // temp.x -= cameraPos.x;
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
    // SDL_RenderFillRectF(renderer, &temp);
}
