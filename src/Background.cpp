#include "Global.hpp"
#include "Background.hpp"
#include "GameStates.hpp"

Background::Background(uint8_t r, uint8_t g, uint8_t b) : m_Red(r), m_Green(g), m_Blue(b)
{
    m_BGTexture = IMG_LoadTexture(renderer, "res/gfx/background.png");
    m_EmptyBG = IMG_LoadTexture(renderer, "res/gfx/emptyBG.png");
    SDL_SetTextureColorMod(m_BGTexture, r, g, b);
    SDL_SetTextureColorMod(m_EmptyBG, r, g, b);
    m_BGCount = WIDTH / BACKGROUND_SIZE + 2;
    m_Pos = {0, 0};
    m_Moving = false;
    m_Fading = false;
    m_TargetRed = r;
    m_TargetGreen = g;
    m_TargetBlue = b;
}

void Background::fade(uint8_t r, uint8_t g, uint8_t b, float time)
{
    m_TargetRed = r;
    m_TargetGreen = g;
    m_TargetBlue = b;
    m_FadeTime = time * 60;
    m_Fading = true;
}

SDL_FPoint Background::getPos()
{
    return m_Pos;
}

void Background::resetPos()
{
    m_Pos.x = 0;   
}

void Background::setMoving(bool value)
{
    m_Moving = value;
}

void Background::update(float delta)
{
    if (m_Moving)
    {
        m_Pos.x -= 2 * delta;

        if (m_Pos.x < -BACKGROUND_SIZE)
        {
            m_Pos.x += BACKGROUND_SIZE;
        }
    }

    if (m_Fading)
    {
        m_FadeTime -= delta;
        m_Red += delta * (m_TargetRed - m_Red) / m_FadeTime;
        m_Green += delta * (m_TargetGreen - m_Green) / m_FadeTime;
        m_Blue += delta * (m_TargetBlue - m_Blue) / m_FadeTime;

        if (m_FadeTime <= 0)
        {
            m_Red = m_TargetRed;
            m_Green = m_TargetGreen;
            m_Blue = m_TargetBlue;
            m_FadeTime = 0;
            m_Fading = false;
        }

        SDL_SetTextureColorMod(m_BGTexture, m_Red, m_Green, m_Blue);
    }
}

void Background::render(int gameState)
{
    for (int i = 0; i < m_BGCount; i++)
    {
        SDL_FRect dst = {m_Pos.x + i * BACKGROUND_SIZE, m_Pos.y - (BACKGROUND_SIZE - HEIGHT) - cameraPos.y/10, BACKGROUND_SIZE, BACKGROUND_SIZE};

        if (gameState == LEVEL_SELECT)
        {
            dst.y += (BACKGROUND_SIZE - HEIGHT);
            SDL_RenderCopyExF(renderer, m_EmptyBG, NULL, &dst, 0, NULL, SDL_FLIP_VERTICAL);
            continue;
        }


        if (SDL_RenderCopyF(renderer, m_BGTexture, NULL, &dst) != 0)
        {
            std::cerr << "Failed to render background! " << SDL_GetError() << std::endl;
        }
    }
}
