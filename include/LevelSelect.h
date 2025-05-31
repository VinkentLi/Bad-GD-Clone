#pragma once
#include "GameState.h"
#include "Button.h"
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

class Game;

class LevelSelect : public GameState {
public:
    void init(Game *game) override;
    void destroy() override;
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render() override;
    inline int getBestPercentage(int levelIndex) { return m_BestLevelPercentages[levelIndex]; }
    inline void setBestPercentage(int levelIndex, int value) { m_BestLevelPercentages[levelIndex] = value; }
    inline static LevelSelect *get() { return &m_LevelSelect; }

private:
    static LevelSelect m_LevelSelect;
    Button m_LeftLevelArrow;
    Button m_RightLevelArrow;
    Button m_TitleScreenArrow;
    Button m_EnterLevel;
    SDL_Rect m_LeftCornerRect;
    SDL_Rect m_RightCornerRect;
    SDL_Rect m_TopRect;
    SDL_Texture *m_CornerTexture;
    SDL_Texture *m_TopTexture;
    SDL_Texture *m_LevelArrowTexture;
    SDL_Texture *m_TitleArrowTexture;
    Mix_Chunk *m_PlaySound;
    std::vector<int> m_BestLevelPercentages;
    bool m_IsMouseHeld;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;
};

