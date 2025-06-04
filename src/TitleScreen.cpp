#include "TitleScreen.h"
#include "LevelSelect.h"
#include <SDL_image.h>
#include <iostream>
#include "Game.h"

TitleScreen TitleScreen::m_TitleScreen;

void TitleScreen::init(Game *game) {
    GameState::init(game);
    m_Name = "TitleScreen";
    m_TitleTexture = IMG_LoadTexture(m_Renderer, "res/gfx/title.png");
    if (m_TitleTexture == NULL) {
        std::cerr << "Failed to load title.png! " << SDL_GetError() << std::endl;
    }
    m_TitlePlayTexture = IMG_LoadTexture(m_Renderer, "res/gfx/titlePlay.png");
    if (m_TitlePlayTexture == NULL) {
        std::cerr << "Failed to load titlePlay.png! " << SDL_GetError() << std::endl;
    }
    int titleWidth = 0;
    int titleHeight = 0;
    int titlePlayWidth = 0;
    int titlePlayHeight = 0;
    SDL_QueryTexture(m_TitleTexture, NULL, NULL, &titleWidth, &titleHeight);
    SDL_QueryTexture(m_TitlePlayTexture, NULL, NULL, &titlePlayWidth, &titlePlayHeight);
    const int width = m_Game->getWidth();
    const int height = m_Game->getHeight();
    m_TitleRect = { width/2 - titleWidth/2, 100, titleWidth, titleHeight };
    m_TitlePlay.init(
        m_Game,
        m_TitlePlayTexture,
        width/2,
        height/2,
        titlePlayWidth,
        titlePlayHeight,
        true,
        true
    );
}

void TitleScreen::destroy() {
    SDL_DestroyTexture(m_TitleTexture);
    SDL_DestroyTexture(m_TitlePlayTexture);
}

void TitleScreen::enter() {
    m_IsMouseHeld = false;
    m_IsEscapeHeld = false;
    m_IsSpaceHeld = false;
}

void TitleScreen::exit() {

}

void TitleScreen::update(float deltaTime) {
    const bool isEscapeHeld = m_Game->isEscapeHeld();
    const bool isEscapeReleased = m_IsEscapeHeld && !isEscapeHeld;
    m_IsEscapeHeld = isEscapeHeld;
    if (isEscapeReleased) {
#ifndef __EMSCRIPTEN__
        m_Game->popState();
#endif
    }
    Ground &ground = m_Game->getGround();
    Background &background = m_Game->getBackground();
    ground.setPosition({ground.getPosition().x, static_cast<float>(m_Game->getHeight() - 3*m_Game->TILE_SIZE)});
    background.setMoving(true);
    background.update(deltaTime);
    ground.update(deltaTime);
    ground.move(-20.772f, deltaTime);

    const bool isSpaceHeld = m_Game->isSpaceHeld();
    const bool spaceReleased = m_IsSpaceHeld && !isSpaceHeld;
    m_IsSpaceHeld = isSpaceHeld;

    m_TitlePlay.update();
    if (m_TitlePlay.isPressed() || spaceReleased) {
        m_Game->pushState(LevelSelect::get());
    }
}

void TitleScreen::render() {
    SDL_RenderCopy(m_Renderer, m_TitleTexture, NULL, &m_TitleRect);
    m_TitlePlay.render();
}
