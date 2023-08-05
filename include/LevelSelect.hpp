#pragma once
#include "GameStates.hpp"
#include "Global.hpp"

inline constexpr int LEVEL_COUNT = 1;

class LevelSelect
{
private:
    SDL_Rect cornerSRC, topSRC, levelArrowSRC, titleArrowSRC, leftCornerDST, rightCornerDST, topDST, leftLevelArrowDST, rightLevelArrowDST, titleArrowDST;
    SDL_Texture *corner, *top, *levelArrow, *titleArrow;
    SDL_Point mousePos;
    bool mouseHeld;
    int levelSelected;

public:
    LevelSelect() = default;
    LevelSelect(SDL_Rect cornerSRC, SDL_Rect topSRC, SDL_Rect levelArrowSRC, SDL_Rect titleArrowSRC);
    void update(int &gameState, SDL_Point *mousePos, bool mouseHeld);
    void render();
};

