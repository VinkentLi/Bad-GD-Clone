#pragma once
#include <SDL.h>

class Game;

class Button {
public:
    Button() {}
    void init(
        Game *game, 
        int x, 
        int y, 
        int w, 
        int h, 
        SDL_Color color, 
        bool xCentered = false,
        bool yCentered = false,
        SDL_RendererFlip flip=SDL_FLIP_NONE
    );
    // Button does not own texture!
    void init(
        Game *game,
        SDL_Texture *texture, 
        int x, 
        int y, 
        int w,
        int h,
        bool xCentered = false, 
        bool yCentered = false, 
        SDL_RendererFlip flip=SDL_FLIP_NONE
    );
    void update();
    void render();
    bool mouseIntersects();
    inline bool isPressed() { return m_IsPressed; }
    inline int getX() { return m_Rect.x; }
    inline int getY() { return m_Rect.y; }
    inline int getW() { return m_Rect.w; }
    inline int getH() { return m_Rect.h; }
    void setPosition(int x, int y, bool xCentered = false, bool yCentered = false);

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_Texture;
    SDL_Rect m_Rect;
    SDL_RendererFlip m_Flip;
    SDL_Color m_Color;
    bool m_IsMouseHeld = false;
    bool m_IsTextured;
    bool m_IsPressed = false;
};
