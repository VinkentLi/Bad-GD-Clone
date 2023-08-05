#include "LevelSelect.hpp"

LevelSelect::LevelSelect(SDL_Rect cornerSRC, SDL_Rect topSRC, SDL_Rect levelArrowSRC, SDL_Rect titleArrowSRC)
    : cornerSRC(cornerSRC), topSRC(topSRC), levelArrowSRC(levelArrowSRC), titleArrowSRC(titleArrowSRC)
{
    leftCornerDST = {0, HEIGHT - 282, 285, 282};
    rightCornerDST = {WIDTH - 285, HEIGHT - 282, 285, 282};
    topDST = {WIDTH/2 - 1226/2*3/4, 0, 1226*3/4, 144*3/4};
    leftLevelArrowDST = {40, HEIGHT/2 - 238/2, 106, 238};
    rightLevelArrowDST = {WIDTH - 40 - 106, HEIGHT/2 - 238/2, 106, 238};
    titleArrowDST = {40, 20, 124, 150};
    corner = IMG_LoadTexture(renderer, "res/gfx/selectCorner.png");
    top = IMG_LoadTexture(renderer, "res/gfx/top.png");
    levelArrow = IMG_LoadTexture(renderer, "res/gfx/levelArrow.png");
    titleArrow = IMG_LoadTexture(renderer, "res/gfx/toTitleScreen.png");
    mousePos = {0, 0};
    mouseHeld = false;
    levelSelected = 0;
}

void LevelSelect::update(int &gameState, SDL_Point *mousePos, bool mouseHeld)
{

}

void LevelSelect::render()
{
    SDL_RenderCopyEx(renderer, corner, &cornerSRC, &leftCornerDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, corner, &cornerSRC, &rightCornerDST, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(renderer, top, &topSRC, &topDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, levelArrow, &levelArrowSRC, &leftLevelArrowDST, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(renderer, levelArrow, &levelArrowSRC, &rightLevelArrowDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, titleArrow, &titleArrowSRC, &titleArrowDST, 0, NULL, SDL_FLIP_NONE);
}
