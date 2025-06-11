#pragma once
#include "GameState.h"
#include "Button.h"
#include <array>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string_view>
#include "Config.h"

class Game;

class LevelSelect : public GameState {
public:
    void init(Game *game) override;
    void destroy() override;
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render() const override;
    inline int getLevelSelected() const { return m_LevelSelected; }
    int getBestPercentage() const;
    void setBestPercentage(int value);
    int getBestPracticePercentage() const;
    void setBestPracticePercentage(int value);
    inline SDL_Texture *getLevelNameTexture() const { return m_LevelNameTexture; }
    inline static LevelSelect &get() { return m_LevelSelect; }

private:
    static LevelSelect m_LevelSelect;
    int m_LevelSelected = 0;
    constexpr static std::array<std::string_view, Config::LEVEL_COUNT> m_LevelStrings = {
        "Stereo Madness", "Back on Track", "Polargeist", "Dry Out", "Base After Base", "Cant Let Go", "Jumper"
    };
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
    std::array<int, Config::LEVEL_COUNT> m_BestLevelPercentages;
    std::array<int, Config::LEVEL_COUNT> m_BestPracticePercentages;
    bool m_IsMouseHeld;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;

    void updateLevelNameTexture();
    void updateBestPercentTexture();
    void updateBestPracticePercentTexture();
};

