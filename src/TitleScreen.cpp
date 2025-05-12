#include "TitleScreen.hpp"

TitleScreen::TitleScreen()
{
    m_TitleTexture = IMG_LoadTexture(renderer, "res/gfx/title.png");

    if (m_TitleTexture == NULL)
    {
        std::cerr << "Failed to load title.png! " << SDL_GetError() << std::endl;
    }

    m_TitlePlay = IMG_LoadTexture(renderer, "res/gfx/titlePlay.png");

    if (m_TitlePlay == NULL)
    {
        std::cerr << "Failed to load titlePlay.png! " << SDL_GetError() << std::endl;
    }

    m_TitleDST = {WIDTH / 2 - (1699*3/4) / 2, 100, 1699*3/4, 206*3/4}; // lol
    m_TitlePlayDST = {WIDTH/2 - 415/2*3/4, HEIGHT/2 - 415/2*3/4 - 40, 415*3/4, 415*3/4}; // super readable btw
    m_MouseHeld = false;
}

void TitleScreen::update(int &gameState, SDL_Point *mousePos, bool mouseHeld)
{
    if (this->m_MouseHeld && !mouseHeld)
    {
        SDL_Rect scaledTitlePlay;
        float wScale = SCREEN_WIDTH / (float) WIDTH;
        float hScale = SCREEN_HEIGHT / (float) HEIGHT;
        scaledTitlePlay.x = m_TitlePlayDST.x * wScale;
        scaledTitlePlay.y = m_TitlePlayDST.y * hScale;
        scaledTitlePlay.w = m_TitlePlayDST.w * wScale;
        scaledTitlePlay.h = m_TitlePlayDST.h * hScale;
        
        if (SDL_PointInRect(mousePos, &scaledTitlePlay))
        {
            gameState = LEVEL_SELECT;
        }
    }

    this->m_MouseHeld = mouseHeld;
    this->m_MousePos = *mousePos;
}

void TitleScreen::render()
{
    SDL_RenderCopy(renderer, m_TitleTexture, NULL, &m_TitleDST);
    SDL_RenderCopy(renderer, m_TitlePlay, NULL, &m_TitlePlayDST);
}
