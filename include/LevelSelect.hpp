#pragma once
#include "GameStates.hpp"
#include "Global.hpp"

class LevelSelect
{
private:
    SDL_Rect m_LeftCornerDST, m_RightCornerDST, m_TopDST, m_LeftLevelArrowDST, m_RightLevelArrowDST, m_TitleArrowDST, m_RectWithLevelName;
    TTF_Font *m_Font;
    TTF_Font *m_FontOutline;
    SDL_Texture *m_Corner, *m_Top, *m_LevelArrow, *m_TitleArrow;
    SDL_Point m_MousePos;
    Mix_Chunk *m_PlaySound;
    bool m_MouseHeld, m_NeedToRecallPlayingStateConstructor;
    std::vector<std::string> levelStrings;

public:
    LevelSelect();
    void update(int &gameState, SDL_Point *mousePos, bool mouseHeld);
    void render();
    bool getNeedToRecallPlayingStateConstructor();
};

