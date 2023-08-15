#include "Ground.hpp"

Ground::Ground(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b)
{
    groundTexture = IMG_LoadTexture(renderer, "res/gfx/ground.png");
    SDL_SetTextureColorMod(groundTexture, r, g, b);
    squareCount = WIDTH / GROUND_SIZE + 2;
    src = {0, 0, GROUND_SIZE, GROUND_SIZE};
    pos = {0, HEIGHT - 300};
    renderOnTop = false;
}

void Ground::setPos(SDL_FPoint pos)
{
    this->pos = pos;
}

SDL_FPoint Ground::getPos()
{
    return pos;
}

void Ground::move(float distance, float delta)
{
    pos.x += distance * delta;
}

void Ground::resetPos()
{
    pos.x = 0;
}

void Ground::update()
{
    if (pos.x < -GROUND_SIZE)
    {
        pos.x += GROUND_SIZE;
    }
}

void Ground::render()
{
    for (int i = 0; i < squareCount; i++)
    {
        SDL_FRect dst = {pos.x + i * GROUND_SIZE - cameraPos.x, pos.y - cameraPos.y, GROUND_SIZE, GROUND_SIZE};

        while (dst.x + GROUND_SIZE < 0)
        {
            dst.x += squareCount * GROUND_SIZE;
        }

        SDL_RenderCopyF(renderer, groundTexture, &src, &dst);

        if (renderOnTop)
        {
            SDL_FRect newDST = dst;
            newDST.y = HEIGHT - dst.y - dst.h;

            SDL_RenderCopyExF(renderer, groundTexture, &src, &newDST, 0.0, NULL, SDL_FLIP_VERTICAL);
        }
    }
}

void Ground::setOnTop(bool value)
{
    renderOnTop = value;
}
