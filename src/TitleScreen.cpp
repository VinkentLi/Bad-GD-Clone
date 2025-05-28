#include "TitleScreen.hpp"
#include <SDL_image.h>
#include <iostream>
#include "Game.hpp"

TitleScreen::TitleScreen(Game *game) : m_Game(game) {
    m_Renderer = m_Game->getRenderer();
    m_TitleTexture = IMG_LoadTexture(m_Renderer, "res/gfx/title.png");
    if (m_TitleTexture == NULL) {
        std::cerr << "Failed to load title.png! " << SDL_GetError() << std::endl;
    }
    m_TitlePlay = IMG_LoadTexture(m_Renderer, "res/gfx/titlePlay.png");
    if (m_TitlePlay == NULL) {
        std::cerr << "Failed to load titlePlay.png! " << SDL_GetError() << std::endl;
    }
    m_TitleDST = {m_Game->getWidth()/ 2 - (1699*3/4) / 2, 100, 1699*3/4, 206*3/4}; // lol
    m_TitlePlayDST = { // most readable code of all time
        m_Game->getWidth()/2 - 415/2*3/4,
        m_Game->getHeight()/2 - 415/2*3/4 - 40, 
        415*3/4, 
        415*3/4
    };
    m_IsMouseHeld = false;
}

TitleScreen::~TitleScreen() {
    SDL_DestroyTexture(m_TitleTexture);
    SDL_DestroyTexture(m_TitlePlay);
}

void TitleScreen::update(int &gameState) {
    const bool isMouseHeld = m_Game->isMouseHeld();
    const bool mouseReleased = m_IsMouseHeld && !isMouseHeld;
    if (mouseReleased) {
        SDL_Rect scaledTitlePlay;
        float wScale = m_Game->getScreenWidth() / static_cast<float>(m_Game->getWidth());
        float hScale = m_Game->getScreenHeight() / static_cast<float>((float) m_Game->getHeight());
        scaledTitlePlay.x = m_TitlePlayDST.x * wScale;
        scaledTitlePlay.y = m_TitlePlayDST.y * hScale;
        scaledTitlePlay.w = m_TitlePlayDST.w * wScale;
        scaledTitlePlay.h = m_TitlePlayDST.h * hScale;
        const SDL_Point mousePosition = m_Game->getMousePosition();
        if (SDL_PointInRect(&mousePosition, &scaledTitlePlay)) {
            gameState = LEVEL_SELECT;
        }
    }
    m_IsMouseHeld = isMouseHeld;
}

void TitleScreen::render() {
    SDL_RenderCopy(m_Renderer, m_TitleTexture, NULL, &m_TitleDST);
    SDL_RenderCopy(m_Renderer, m_TitlePlay, NULL, &m_TitlePlayDST);
}
