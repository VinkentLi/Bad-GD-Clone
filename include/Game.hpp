#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "GameStates.hpp"

class Background;
class Ground;
class TitleScreen;
class LevelSelect;
class PlayingState;

class Game {
public:
    constexpr static int LEVEL_COUNT = 2;
    constexpr static int TILE_SIZE = 100;
    constexpr static int BACKGROUND_SIZE = 2048;
    Game() {}
    int init();
    void run();
    void quit();
    bool isMouseHeld() { return m_IsMouseHeld; }
    SDL_Renderer *getRenderer() { return m_Renderer; }
    int getWidth() { return m_Width; }
    int getHeight() { return m_Height; }
    int getScreenWidth() { return m_ScreenWidth; }
    int getScreenHeight() { return m_ScreenHeight; }
    SDL_FPoint getCameraPosition() { return m_CameraPosition; }
    void setCameraPosition(SDL_FPoint newPosition) { m_CameraPosition = newPosition; } 
    void setCameraX(float x) { m_CameraPosition.x = x; }
    void setCameraY(float y) { m_CameraPosition.y = y; }
    int getLevelSelected() { return m_LevelSelected; }
    void setLevelSelected(int num) { m_LevelSelected = num; }
    void increaseLevelSelected() { m_LevelSelected++; }
    void decreaseLevelSelected() { m_LevelSelected--; }

private:
    SDL_Window *m_Window;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_TileSheet;
    SDL_Point m_MousePos;
    SDL_FPoint m_CameraPosition;
    TTF_Font *m_Font;
    TTF_Font *m_FontOutline;
    Mix_Music *m_MenuLoop;
    Background *m_Background = nullptr;
    Ground *m_Ground = nullptr;
    TitleScreen *m_TitleScreen = nullptr;
    LevelSelect *m_LevelSelect = nullptr;
    PlayingState *m_PlayingState = nullptr;
    bool m_IsGameRunning = true;
    bool m_IsMouseHeld = false;
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
