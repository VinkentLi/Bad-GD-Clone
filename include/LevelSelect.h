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
    inline std::string getLevelName() { return m_LevelStrings[m_LevelSelected]; }
    inline int getLevelSelected() { return m_LevelSelected; }
    int getBestPercentage();
    void setBestPercentage(int value);
    int getBestPracticePercentage();
    void setBestPracticePercentage(int value);
    inline SDL_Texture *getLevelNameTexture() { return m_LevelNameTexture; }
    inline static LevelSelect *get() { return &m_LevelSelect; }

private:
    static LevelSelect m_LevelSelect;
    int m_LevelSelected = 0;
    std::vector<std::string> m_LevelStrings;
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
    SDL_Texture *m_LevelNameTexture;
    SDL_Texture *m_BestPercentTexture;
    SDL_Texture *m_BestPracticePercentTexture;
    Mix_Chunk *m_PlaySound;
    std::vector<int> m_BestLevelPercentages;
    std::vector<int> m_BestPracticePercentages;
    bool m_IsMouseHeld;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;

    void updateLevelNameTexture();
    void updateBestPercentTexture();
    void updateBestPracticePercentTexture();
};

