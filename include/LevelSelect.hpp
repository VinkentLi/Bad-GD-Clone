#pragma once
#include "GameStates.hpp"
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

class Game;

class LevelSelect {
public:
    LevelSelect(Game *game);
    ~LevelSelect();
    LevelSelect(const LevelSelect &) = delete;
    LevelSelect &operator=(const LevelSelect &) = delete;
    void update(float deltaTime);
    void render();
    bool getNeedToRecallPlayingStateConstructor();

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
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
    bool m_NeedToRecallPlayingStateConstructor;
    std::vector<std::string> m_LevelStrings;
};

