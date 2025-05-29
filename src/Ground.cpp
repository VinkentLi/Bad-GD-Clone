#include "Ground.h"
#include "Game.h"
#include <SDL_image.h>

void Ground::init(Game *game, uint8_t r, uint8_t g, uint8_t b) {
    m_Game = game;
    m_Red = r;
    m_Green = g;
    m_Blue = b;
    m_Renderer = m_Game->getRenderer();
    m_GroundTexture = IMG_LoadTexture(m_Renderer, "res/gfx/ground.png");
    SDL_SetTextureColorMod(m_GroundTexture, r, g, b);
    m_SquareCount = m_Game->getWidth() / GROUND_SIZE + 2;
    m_Source = {0, 0, GROUND_SIZE, GROUND_SIZE};
    m_Position = {0, m_Game->getHeight() - 300.0f};
    m_ShouldRenderOnTop = false;
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

void Ground::update() {
    if (m_Position.x < -GROUND_SIZE) {
        m_Position.x += GROUND_SIZE;
    }
}

void Ground::render() {
    for (int i = 0; i < m_SquareCount; i++) {
        const SDL_FPoint cameraPosition = m_Game->getCameraPosition();
        SDL_FRect dst = {
            m_Position.x + i * GROUND_SIZE - cameraPosition.x,
            m_Position.y - cameraPosition.y, 
            GROUND_SIZE, 
            GROUND_SIZE
        };
        while (dst.x + GROUND_SIZE < 0) {
            dst.x += m_SquareCount * GROUND_SIZE;
        }
        SDL_RenderCopyF(m_Renderer, m_GroundTexture, &m_Source, &dst);
        if (m_ShouldRenderOnTop) {
            SDL_FRect newDST = dst;
            newDST.y = m_Game->getHeight() - dst.y - dst.h;
            SDL_RenderCopyExF(m_Renderer, m_GroundTexture, &m_Source, &newDST, 0.0, NULL, SDL_FLIP_VERTICAL);
        }
    }
}

void Ground::setOnTop(bool value) {
    m_ShouldRenderOnTop = value;
}
