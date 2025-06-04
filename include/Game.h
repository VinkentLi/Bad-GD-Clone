#pragma once
#include <stack>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "GameState.h"
#include "Background.h"
#include "Ground.h"
#include <memory>
#include <vector>

class Game {
public:
    constexpr static int LEVEL_COUNT = 7;
    constexpr static int TILE_SIZE = 120;
    Game() {}
    ~Game() = default;
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    int init();
    void run();
    void quit();

    void update(float deltaTime);
    void render();
    void handleEvents();

    void pushState(GameState *state);
    void popState();
    void changeState(GameState *state);
    inline GameState *getState() { return m_GameStates.top(); }
    
    inline bool isGameRunning() { return m_IsGameRunning; }
    inline bool isMouseHeld() { return m_IsMouseHeld; }
    inline bool isEscapeHeld() { return m_IsEscapeHeld; }
    inline bool isSpaceHeld() { return m_IsSpaceHeld; }
    inline bool isZHeld() { return m_IsZHeld; }
    inline bool isXHeld() { return m_IsXHeld; }
    inline SDL_Renderer *getRenderer() { return m_Renderer; }
    inline int getWidth() { return m_Width; }
    inline int getHeight() { return m_Height; }
    inline int getScreenWidth() { return m_ScreenWidth; }
    inline int getScreenHeight() { return m_ScreenHeight; }
    inline SDL_FPoint getCameraPosition() { return m_CameraPosition; }
    inline void setCameraPosition(SDL_FPoint newPosition) { m_CameraPosition = newPosition; } 
    inline void setCameraX(float x) { m_CameraPosition.x = x; }
    inline void setCameraY(float y) { m_CameraPosition.y = y; }
    inline SDL_Point getMousePosition() { return m_MousePosition; }
    inline Ground &getGround() { return m_Ground; }
    inline Background &getBackground() { return m_Background; }
    inline void restartMenuLoop() { Mix_PlayMusic(m_MenuLoop, -1); }

private:
    SDL_Window *m_Window;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_FPSTexture;

// TODO: FIX BAD CODE
#ifdef __EMSCRIPTEN__
public:
#endif
    uint64_t m_CurrentTime;
    uint64_t m_NewTime;
    SDL_Point m_MousePosition;
    SDL_FPoint m_CameraPosition;
    Mix_Music *m_MenuLoop;
    Background m_Background;
    Ground m_Ground;
    std::stack<GameState *> m_GameStates;
    bool m_IsGameRunning = true;
    bool m_IsMouseHeld = false;
    bool m_IsEscapeHeld = false;
    bool m_IsSpaceHeld = false;
    bool m_IsZHeld = false;
    bool m_IsXHeld = false;
    int m_Width;
    int m_Height = 1296;
    int m_ScreenWidth;
    int m_ScreenHeight;
    int m_Frames = 0;
    int m_CurrentFPS = 0;
    int m_Timer = 0;
#ifndef __EMSCRIPTEN__
    void mainLoop();
#endif
};

#ifdef __EMSCRIPTEN__
extern Game emGame;
#endif
