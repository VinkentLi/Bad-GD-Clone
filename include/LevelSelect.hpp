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
    void update(int &gameState, SDL_Point *mousePos, bool isMouseHeld);
    void render();
    bool getNeedToRecallPlayingStateConstructor();

private:
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    SDL_Rect m_LeftCornerDST, m_RightCornerDST, m_TopDST, m_LeftLevelArrowDST, m_RightLevelArrowDST, m_TitleArrowDST, m_RectWithLevelName;
    TTF_Font *m_Font;
    TTF_Font *m_FontOutline;
    SDL_Texture *m_Corner, *m_Top, *m_LevelArrow, *m_TitleArrow;
    SDL_Point m_MousePos;
    Mix_Chunk *m_PlaySound;
    bool m_IsMouseHeld, m_NeedToRecallPlayingStateConstructor;
    std::vector<std::string> m_LevelStrings;
};

