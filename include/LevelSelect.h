#pragma once
#include "GameState.h"
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
    inline static LevelSelect *get() { return &m_LevelSelect; }

private:
    static LevelSelect m_LevelSelect;
    SDL_Rect m_LeftCornerDST;
    SDL_Rect m_RightCornerDST;
    SDL_Rect m_TopDST;
    SDL_Rect m_LeftLevelArrowDST;
    SDL_Rect m_RightLevelArrowDST;
    SDL_Rect m_TitleArrowDST;
    SDL_Rect m_RectWithLevelName;
    TTF_Font *m_Font;
    TTF_Font *m_FontOutline;
    SDL_Texture *m_Corner;
    SDL_Texture *m_Top;
    SDL_Texture *m_LevelArrow;
    SDL_Texture *m_TitleArrow;
    Mix_Chunk *m_PlaySound;
    bool m_IsMouseHeld;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;
    std::vector<std::string> m_LevelStrings;
};

