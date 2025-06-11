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
    Game() {}
    ~Game() = default;
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    int init();
    void quit();

    void update(float deltaTime);
    void render();
    void handleEvents();
    void incrementFrames();
    void updateFPS();

    void pushState(GameState &state);
    void popState();
    void changeState(GameState &state);
    inline GameState *getState() const { return m_GameStates.top(); }

    inline SDL_Renderer *getRenderer() const { return m_Renderer; }
    inline bool isGameRunning() const { return m_IsGameRunning; }
    inline bool isMouseHeld() const { return m_IsMouseHeld; }
    inline bool isEscapeHeld() const { return m_IsEscapeHeld; }
    inline bool isSpaceHeld() const { return m_IsSpaceHeld; }
    inline bool isUpHeld() const { return m_IsUpHeld; }
    inline bool isZHeld() const { return m_IsZHeld; }
    inline bool isXHeld() const { return m_IsXHeld; }
    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }
    inline int getScreenWidth() const { return m_ScreenWidth; }
    inline int getScreenHeight() const { return m_ScreenHeight; }
    inline SDL_FPoint getCameraPosition() const { return m_CameraPosition; }
    inline void setCameraPosition(SDL_FPoint newPosition) { m_CameraPosition = newPosition; m_TargetY = newPosition.y; } 
    inline void setCameraX(float x) { m_CameraPosition.x = x; }
    inline void setCameraY(float y) { m_CameraPosition.y = y; m_TargetY = y; }
    inline void smoothCameraYScroll(float targetY) { m_TargetY = targetY; }  
    inline SDL_Point getMousePosition() const { return m_MousePosition; }
    inline Ground &getGround() { return m_Ground; }
    inline Background &getBackground() { return m_Background; }
    inline void restartMenuLoop() { Mix_PlayMusic(m_MenuLoop, -1); }

private:
    SDL_Window *m_Window;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_FPSTexture;
    SDL_Point m_MousePosition;
    SDL_FPoint m_CameraPosition = { 0, 0 };
    float m_TargetY = 0;
    Mix_Music *m_MenuLoop;
    Background m_Background;
    Ground m_Ground;
    std::stack<GameState *> m_GameStates;
    bool m_IsGameRunning = true;
    bool m_IsMouseHeld = false;
    bool m_IsEscapeHeld = false;
    bool m_IsSpaceHeld = false;
    bool m_IsUpHeld = false;
    bool m_IsZHeld = false;
    bool m_IsXHeld = false;
    int m_Width;
    int m_Height = Config::HEIGHT;
    int m_ScreenWidth;
    int m_ScreenHeight;
    int m_Frames = 0;
    int m_CurrentFPS = 0;
};
