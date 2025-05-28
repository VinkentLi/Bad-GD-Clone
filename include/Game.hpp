#pragma once
#include <stack>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "GameStates.hpp"
#include "GameState.hpp"
#include "Background.hpp"
#include "Ground.hpp"
#include <memory>

class TitleScreen;
class LevelSelect;
class PlayingState;

class Game {
public:
    constexpr static int LEVEL_COUNT = 2;
    constexpr static int TILE_SIZE = 100;
    constexpr static int BACKGROUND_SIZE = 2048;
    Game() {}
    ~Game() = default;
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    int init();
    void run();
    void quit();
    inline bool isMouseHeld() { return m_IsMouseHeld; }
    inline bool isEscapeHeld() { return m_IsEscapeHeld; }
    inline bool isSpaceHeld() { return m_IsSpaceHeld; }
    inline SDL_Renderer *getRenderer() { return m_Renderer; }
    inline int getWidth() { return m_Width; }
    inline int getHeight() { return m_Height; }
    inline int getScreenWidth() { return m_ScreenWidth; }
    inline int getScreenHeight() { return m_ScreenHeight; }
    inline SDL_FPoint getCameraPosition() { return m_CameraPosition; }
    inline void setCameraPosition(SDL_FPoint newPosition) { m_CameraPosition = newPosition; } 
    inline void setCameraX(float x) { m_CameraPosition.x = x; }
    inline void setCameraY(float y) { m_CameraPosition.y = y; }
    inline int getLevelSelected() { return m_LevelSelected; }
    inline void setLevelSelected(int num) { m_LevelSelected = num; }
    inline void increaseLevelSelected() { m_LevelSelected++; }
    inline void decreaseLevelSelected() { m_LevelSelected--; }
    inline SDL_Point getMousePosition() { return m_MousePosition; }
    inline int &getGameState() { return m_GameState; } // TODO: get rid of this
    inline Ground &getGround() { return m_Ground; }
    inline Background &getBackground() { return m_Background; }
    inline void restartMenuLoop() { Mix_PlayMusic(m_MenuLoop, -1); } // will get rid of this after gsm is added

private:
    SDL_Window *m_Window;
    SDL_Renderer *m_Renderer;
    SDL_Point m_MousePosition;
    SDL_FPoint m_CameraPosition;
    TTF_Font *m_Font;
    Mix_Music *m_MenuLoop;
    Background m_Background;
    Ground m_Ground;
    // std::stack<std::unique_ptr<GameState>> m_GameStates;
    TitleScreen *m_TitleScreen = nullptr;
    LevelSelect *m_LevelSelect = nullptr;
    PlayingState *m_PlayingState = nullptr;
    bool m_IsGameRunning = true;
    bool m_IsMouseHeld = false;
    bool m_IsEscapeHeld = false;
    bool m_IsSpaceHeld = false;
    int m_Width;
    int m_Height = 1080;
    int m_ScreenWidth;
    int m_ScreenHeight;
    int m_Frames = 0;
    int m_CurrentFPS = 0;
    int m_GameState = TITLE_SCREEN;
    int m_LevelSelected = 0;
    int m_Timer = 0;
#ifdef __EMSCRIPTEN__
    void mainLoop();
#else
    void mainLoop(float deltaTime);
#endif
    void update(float deltaTime);
    void render();
    void handleEvents();
};
