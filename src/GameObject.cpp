#include "GameObject.hpp"

GameObject::GameObject(int id, int type, SDL_FPoint pos, SDL_FRect hitbox)
    : id(id), type(type), pos(pos), hitbox(hitbox)
{
    this->src.x = (id % 4) * TILE_SIZE;
    this->src.y = (id / 4) * TILE_SIZE;
    this->src.w = TILE_SIZE;
    this->src.h = TILE_SIZE;
}

void GameObject::render()
{
    SDL_Rect dst;
    dst.x = (int) (pos.x);
    dst.y = (int) (pos.y);
    dst.w = TILE_SIZE;
    dst.h = TILE_SIZE;

    if (SDL_RenderCopy(renderer, tileSheet, &this->src, &dst) != 0)
    {
        std::cerr << "GameObject failed to render texture! " << SDL_GetError() << std::endl;
    }
}
