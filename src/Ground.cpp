#include "Ground.h"
#include "Game.h"
#include <iostream>
#include <SDL_image.h>

void Ground::init(Game *game, uint8_t r, uint8_t g, uint8_t b) {
    m_Game = game;
    m_Red = r;
    m_Green = g;
    m_Blue = b;
    m_Renderer = m_Game->getRenderer();
    m_GroundTexture = IMG_LoadTexture(m_Renderer, "res/gfx/ground.png");
    if (m_GroundTexture == nullptr) {
        std::cerr << "Failed to load ground.png! " << SDL_GetError() << std::endl;
    }
    SDL_SetTextureColorMod(m_GroundTexture, r, g, b);
    SDL_QueryTexture(m_GroundTexture, NULL, NULL, &m_GroundSize, NULL);
    m_SquareCount = m_Game->getWidth() / m_GroundSize + 2;
    m_Position = {0, static_cast<float>(m_Game->getHeight() - 3*m_Game->TILE_SIZE)};
    m_ShouldRenderOnTop = false;
}

void Ground::fade(uint8_t r, uint8_t g, uint8_t b, float time) {
    m_TargetRed = r;
    m_TargetGreen = g;
    m_TargetBlue = b;
    m_FadeTime = time * 60;
    m_IsFading = true;
}

void Ground::destroy() {
    SDL_DestroyTexture(m_GroundTexture);
}

void Ground::setPosition(SDL_FPoint pos) {
    this->m_Position = pos;
}

SDL_FPoint Ground::getPosition() {
    return m_Position;
}

void Ground::move(float distance, float delta) {
    m_Position.x += distance * delta;
}

void Ground::resetPosition() {
    m_Position.x = 0;
}

void Ground::update(float delta) {
    if (m_Position.x < -m_GroundSize) {
        m_Position.x += m_GroundSize;
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
        SDL_SetTextureColorMod(m_GroundTexture, static_cast<uint8_t>(m_Red), static_cast<uint8_t>(m_Green), static_cast<uint8_t>(m_Blue));
    }
}

void Ground::render() {
    for (int i = 0; i < m_SquareCount; i++) {
        const SDL_FPoint cameraPosition = m_Game->getCameraPosition();
        SDL_FRect dst = {
            m_Position.x + i * m_GroundSize - cameraPosition.x,
            m_Position.y - cameraPosition.y, 
            static_cast<float>(m_GroundSize), 
            static_cast<float>(m_GroundSize)
        };
        while (dst.x + m_GroundSize < 0) {
            dst.x += m_SquareCount * m_GroundSize;
        }
        if (m_ShouldRenderOnTop) {
            const int BOUNDS_HEIGHT = (m_Game->getHeight() - 10*m_Game->TILE_SIZE)/2;
            dst.y = static_cast<float>(m_Game->getHeight() - BOUNDS_HEIGHT);
            SDL_RenderCopyF(m_Renderer, m_GroundTexture, NULL, &dst);
            SDL_FRect newDST = dst;
            newDST.y = m_Game->getHeight() - dst.y - dst.h;
            SDL_RenderCopyExF(m_Renderer, m_GroundTexture, NULL, &newDST, 0.0, NULL, SDL_FLIP_VERTICAL);
        } else {
            SDL_RenderCopyF(m_Renderer, m_GroundTexture, NULL, &dst);
        }
    }
}

void Ground::setOnTop(bool value) {
    m_ShouldRenderOnTop = value;
}

std::array<float, 3> Ground::getColor() {
    return { m_Red, m_Green, m_Blue };
}

void Ground::setColor(std::array<float, 3> color) {
    m_Red = color[0];
    m_Green = color[1];
    m_Blue = color[2];
    SDL_SetTextureColorMod(m_GroundTexture, static_cast<uint8_t>(m_Red), static_cast<uint8_t>(m_Green), static_cast<uint8_t>(m_Blue));
}

SDL_Color Ground::getTargetColor() {
    return { m_TargetRed, m_TargetGreen, m_TargetBlue };
}

void Ground::setTargetColor(SDL_Color color) {
    m_TargetRed = color.r;
    m_TargetGreen = color.g;
    m_TargetBlue = color.b;
}

float Ground::getFadeTime() {
    return m_FadeTime;
}

void Ground::setFadeTime(float fadeTime) {
    m_FadeTime = fadeTime;
}
