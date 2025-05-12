#include "Ground.hpp"

Ground::Ground(uint8_t r, uint8_t g, uint8_t b) : m_Red(r), m_Green(g), m_Blue(b)
{
    m_GroundTexture = IMG_LoadTexture(renderer, "res/gfx/ground.png");
    SDL_SetTextureColorMod(m_GroundTexture, r, g, b);
    m_SquareCount = WIDTH / GROUND_SIZE + 2;
    m_Source = {0, 0, GROUND_SIZE, GROUND_SIZE};
    m_Pos = {0, HEIGHT - 300};
    m_RenderOnTop = false;
}

void Ground::setPos(SDL_FPoint pos)
{
    this->m_Pos = pos;
}

SDL_FPoint Ground::getPos()
{
    return m_Pos;
}

void Ground::move(float distance, float delta)
{
    m_Pos.x += distance * delta;
}

void Ground::resetPos()
{
    m_Pos.x = 0;
}

void Ground::update()
{
    if (m_Pos.x < -GROUND_SIZE)
    {
        m_Pos.x += GROUND_SIZE;
    }
}

void Ground::render()
{
    for (int i = 0; i < m_SquareCount; i++)
    {
        SDL_FRect dst = {m_Pos.x + i * GROUND_SIZE - cameraPos.x, m_Pos.y - cameraPos.y, GROUND_SIZE, GROUND_SIZE};

        while (dst.x + GROUND_SIZE < 0)
        {
            dst.x += m_SquareCount * GROUND_SIZE;
        }

        SDL_RenderCopyF(renderer, m_GroundTexture, &m_Source, &dst);

        if (m_RenderOnTop)
        {
            SDL_FRect newDST = dst;
            newDST.y = HEIGHT - dst.y - dst.h;

            SDL_RenderCopyExF(renderer, m_GroundTexture, &m_Source, &newDST, 0.0, NULL, SDL_FLIP_VERTICAL);
        }
    }
}

void Ground::setOnTop(bool value)
{
    m_RenderOnTop = value;
}
