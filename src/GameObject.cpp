#include "GameObject.hpp"

GameObject::GameObject(int type, SDL_FPoint pos, SDL_FRect hitbox, const char *texturePath)
    : type(type), pos(pos), hitbox(hitbox)
{
    objectTexture = IMG_LoadTexture(renderer, texturePath);
    
    if (objectTexture == NULL)
    {
        std::cerr << "Failed to load objectTexture! " << SDL_GetError() << std::endl;
    }
}

SDL_FRect *GameObject::getHitbox()
{
    return &hitbox;
}

SDL_FPoint GameObject::getPos()
{
    return pos;
}

int GameObject::getType()
{
    return type;
}

void GameObject::render()
{
    SDL_Rect dst;
    dst.x = (int) (pos.x) - cameraPos.x;
    dst.y = (int) (pos.y) - cameraPos.y;
    dst.w = TILE_SIZE;
    dst.h = TILE_SIZE;

    if (SDL_RenderCopy(renderer, objectTexture, NULL, &dst) != 0)
    {
        std::cerr << "GameObject failed to render texture! " << SDL_GetError() << std::endl;
    }

    SDL_FRect temp = hitbox;
    temp.x -= cameraPos.x;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
    SDL_RenderFillRectF(renderer, &temp);
}
