#include "Button.h"
#include "Game.h"

void Button::init(
    Game *game,
    int x, 
    int y, 
    int w, 
    int h, 
    SDL_Color color, 
    bool xCentered,
    bool yCentered,
    SDL_RendererFlip flip
) {
    m_Game = game;
    m_IsTextured = false;
    m_Color = color;
    m_Flip = flip;
    m_Renderer = m_Game->getRenderer();
    m_Rect = { .x=x, .y=y, .w=w, .h=h};
    if (xCentered) {
        m_Rect.x -= w/2;
    }
    if (yCentered) {
        m_Rect.y -= h/2;
    }
}

void Button::init(
    Game *game,
    SDL_Texture *texture, 
    int x, 
    int y, 
    int w,
    int h,
    bool xCentered, 
    bool yCentered, 
    SDL_RendererFlip flip
) {
    m_Game = game;
    m_Texture = texture;
    m_IsTextured = true;
    m_Flip = flip;
    m_Renderer = m_Game->getRenderer();
    m_Rect = { .x=x, .y=y, .w=w, .h=h };
    if (xCentered) {
        m_Rect.x -= w/2;
    }
    if (yCentered) {
        m_Rect.y -= h/2;
    }
}

void Button::update() {
    m_IsPressed = false;
    const bool isMouseHeld = m_Game->isMouseHeld();
    const bool isMouseReleased = m_IsMouseHeld && !isMouseHeld;
    m_IsMouseHeld = isMouseHeld;
    if (isMouseReleased) {
        SDL_Point mousePosition = m_Game->getMousePosition();
        float wScale = m_Game->getScreenWidth() / static_cast<float>(m_Game->getWidth());
        float hScale = m_Game->getScreenHeight() / static_cast<float>(m_Game->getHeight());
        SDL_Point scaledMousePosition;
        scaledMousePosition.x = static_cast<int>(mousePosition.x / wScale);
        scaledMousePosition.y = static_cast<int>(mousePosition.y / hScale);
        if (SDL_PointInRect(&scaledMousePosition, &m_Rect)) {
            m_IsPressed = true;
        }
    }
}

void Button::render() {
    if (m_IsTextured) {
        SDL_RenderCopyEx(m_Renderer, m_Texture, NULL, &m_Rect, 0, NULL, m_Flip);
    } else {
        SDL_SetRenderDrawColor(m_Renderer, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
        SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(m_Renderer, &m_Rect);
    }
}

void Button::setPosition(int x, int y, bool xCentered, bool yCentered) {
    m_Rect.x = x;
    m_Rect.y = y;
    if (xCentered) {
        m_Rect.x -= m_Rect.w/2;
    }
    if (yCentered) {
        m_Rect.y -= m_Rect.h/2;
    }
}
