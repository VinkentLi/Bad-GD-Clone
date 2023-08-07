#pragma once
#include "GameStates.hpp"
#include "Global.hpp"

class LevelSelect
{
private:
    SDL_Rect leftCornerDST, rightCornerDST, topDST, leftLevelArrowDST, rightLevelArrowDST, titleArrowDST, rectWithLevelName;
    TTF_Font *font;
    TTF_Font *fontOutline;
    SDL_Texture *corner, *top, *levelArrow, *titleArrow;
    SDL_Point mousePos;
    Mix_Chunk *playSound;
    bool mouseHeld;
    std::vector<std::string> levelStrings;

public:
    LevelSelect();
    void update(int &gameState, SDL_Point *mousePos, bool mouseHeld);
    void render();
};

