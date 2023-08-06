#pragma once
#include "GameStates.hpp"
#include "Global.hpp"

class LevelSelect
{
private:
    constexpr static int LEVEL_COUNT = 2;
    SDL_Rect leftCornerDST, rightCornerDST, topDST, leftLevelArrowDST, rightLevelArrowDST, titleArrowDST;
    TTF_Font *font;
    TTF_Font *fontOutline;
    SDL_Texture *corner, *top, *levelArrow, *titleArrow;
    SDL_Point mousePos;
    bool mouseHeld;
    int levelSelected;
    std::vector<std::string> levelStrings;

public:
    LevelSelect();
    void update(int &gameState, SDL_Point *mousePos, bool mouseHeld);
    void render();
};

