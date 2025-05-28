#include "Background.hpp"
#include "GameStates.hpp"
#include "Game.hpp"
#include "SDL_image.h"
#include <iostream>

void Background::init(Game* game, uint8_t r, uint8_t g, uint8_t b) {
    m_Game = game;
    m_Red = r;
    m_Green = g;
    m_Blue = b;
    m_Renderer = m_Game->getRenderer();
    m_BGTexture = IMG_LoadTexture(m_Renderer, "res/gfx/background.png");
    m_EmptyBG = IMG_LoadTexture(m_Renderer, "res/gfx/emptyBG.png");
    SDL_SetTextureColorMod(m_BGTexture, r, g, b);
    SDL_SetTextureColorMod(m_EmptyBG, r, g, b);
    m_BGCount = m_Game->getWidth() / m_Game->BACKGROUND_SIZE + 2;
    m_Position = {0, 0};
    m_IsMoving = false;
    m_IsFading = false;
    m_TargetRed = r;
    m_TargetGreen = g;
    m_TargetBlue = b;
}

void Background::destroy() {
    SDL_DestroyTexture(m_BGTexture);
    SDL_DestroyTexture(m_EmptyBG);
}

void Background::fade(uint8_t r, uint8_t g, uint8_t b, float time) {
    m_TargetRed = r;
    m_TargetGreen = g;
    m_TargetBlue = b;
    m_FadeTime = time * 60;
    m_IsFading = true;
}

SDL_FPoint Background::getPosition() {
    return m_Position;
}

void Background::resetPosition() {
    m_Position.x = 0;   
}

void Background::setMoving(bool value) {
    m_IsMoving = value;
}

void Background::update(float delta) {
    if (m_IsMoving) {
        m_Position.x -= 2 * delta;
        if (m_Position.x < -m_Game->BACKGROUND_SIZE) {
            m_Position.x += m_Game->BACKGROUND_SIZE;
        }
    }
    if (m_IsFading) {
        m_FadeTime -= delta;
        m_Red += delta * (m_TargetRed - m_Red) / m_FadeTime;
        m_Green += delta * (m_TargetGreen - m_Green) / m_FadeTime;
        m_Blue += delta * (m_TargetBlue - m_Blue) / m_FadeTime;

        if (m_FadeTime <= 0) {
            m_Red = m_TargetRed;
            m_Green = m_TargetGreen;
            m_Blue = m_TargetBlue;
            m_FadeTime = 0;
            m_IsFading = false;
        }
        SDL_SetTextureColorMod(m_BGTexture, m_Red, m_Green, m_Blue);
    }
}

void Background::render(int gameState) {
    for (int i = 0; i < m_BGCount; i++) {
        const int height = m_Game->getHeight();
        const SDL_FPoint cameraPosition = m_Game->getCameraPosition();
        SDL_FRect dst = { 
            m_Position.x + i * m_Game->BACKGROUND_SIZE,
            m_Position.y - (m_Game->BACKGROUND_SIZE - height) - cameraPosition.y/10,
            m_Game->BACKGROUND_SIZE,
            m_Game->BACKGROUND_SIZE
        };
        if (gameState == LEVEL_SELECT) {
            dst.y += (m_Game->BACKGROUND_SIZE - height);
            SDL_RenderCopyExF(m_Renderer, m_EmptyBG, NULL, &dst, 0, NULL, SDL_FLIP_VERTICAL);
            continue;
        }
        if (SDL_RenderCopyF(m_Renderer, m_BGTexture, NULL, &dst) != 0) {
            std::cerr << "Failed to render background! " << SDL_GetError() << std::endl;
        }
    }
}
