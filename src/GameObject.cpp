#include "GameObject.hpp"

GameObject::GameObject(SDL_Rect src, int type, SDL_FPoint pos, SDL_FRect hitbox)
    : src(src), type(type), pos(pos), hitbox(hitbox)
{
}

void GameObject::render()
{
    SDL_Rect dst;
    dst.x = (int) (pos.x);
    dst.y = (int) (pos.y);
    dst.w = TILE_SIZE;
    dst.h = TILE_SIZE;

    if (SDL_RenderCopy(renderer, tileSheet, &src, &dst) != 0)
    {
        std::cerr << "GameObject failed to render texture! " << SDL_GetError() << std::endl;
    }
}
