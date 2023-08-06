#include "LevelSelect.hpp"

LevelSelect::LevelSelect()
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
    levelStrings.push_back("Test level1");
    levelStrings.push_back("Test Level2");
    font = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    fontOutline = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    TTF_SetFontOutline(fontOutline, 4);
}

void LevelSelect::update(int &gameState, SDL_Point *mousePos, bool mouseHeld)
{
    if (this->mouseHeld && !mouseHeld)
    {  
        float wScale = SCREEN_WIDTH / (float) WIDTH;
        float hScale = SCREEN_HEIGHT / (float) HEIGHT;
        SDL_Rect scaledTitleArrow = titleArrowDST, scaledLeft = leftLevelArrowDST, scaledRight = rightLevelArrowDST;
        scaledTitleArrow.x *= wScale;
        scaledTitleArrow.y *= hScale;
        scaledTitleArrow.w *= wScale;
        scaledTitleArrow.h *= hScale;
        scaledLeft.x *= wScale;
        scaledLeft.y *= hScale;
        scaledLeft.w *= wScale;
        scaledLeft.h *= hScale;
        scaledRight.x *= wScale;
        scaledRight.y *= hScale;
        scaledRight.w *= wScale;
        scaledRight.h *= hScale;

        if (SDL_PointInRect(mousePos, &scaledTitleArrow))
        {
            gameState = TITLE_SCREEN;
        }
        else if (SDL_PointInRect(mousePos, &scaledLeft))
        {
            levelSelected--;
            
            if (levelSelected < 0)
            {
                levelSelected = LEVEL_COUNT - 1;
            }
        }
        else if (SDL_PointInRect(mousePos, &scaledRight))
        {
            levelSelected++;

            if (levelSelected == LEVEL_COUNT)
            {
                levelSelected = 0;
            }
        }
    }

    this->mouseHeld = mouseHeld;
    this->mousePos = *mousePos;
}

void LevelSelect::render()
{
    SDL_RenderCopyEx(renderer, corner, NULL, &leftCornerDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, corner, NULL, &rightCornerDST, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(renderer, top, NULL, &topDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, levelArrow, NULL, &leftLevelArrowDST, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(renderer, levelArrow, NULL, &rightLevelArrowDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, titleArrow, NULL, &titleArrowDST, 0, NULL, SDL_FLIP_NONE);
    SDL_Rect rectWithLevelName;
    rectWithLevelName.w = 1000;
    rectWithLevelName.h = 300;
    rectWithLevelName.x = WIDTH / 2 - 1000/2;
    rectWithLevelName.y = HEIGHT / 2 - 350;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &rectWithLevelName);

    // render level name

    SDL_Surface *levelShadowSurface = TTF_RenderText_Blended(font, levelStrings[levelSelected].c_str(), {0, 0, 0, 100});
    SDL_Surface *levelStringSurface = TTF_RenderText_Blended(font, levelStrings[levelSelected].c_str(), {255, 255, 255});
    SDL_Surface *levelStringOutline = TTF_RenderText_Blended(fontOutline, levelStrings[levelSelected].c_str(), {0, 0, 0});
    SDL_SetSurfaceBlendMode(levelStringSurface, SDL_BLENDMODE_BLEND);
    SDL_Rect rect = {4, 4, levelStringSurface->w, levelStringSurface->h};
    SDL_BlitSurface(levelStringSurface, NULL, levelStringOutline, &rect);
    SDL_FreeSurface(levelStringSurface);
    SDL_Texture *levelShadowTexture = SDL_CreateTextureFromSurface(renderer, levelShadowSurface);
    SDL_Texture *levelStringTexture = SDL_CreateTextureFromSurface(renderer, levelStringOutline);
    SDL_Rect stringDST = {WIDTH/2 - levelStringOutline->w/2, rectWithLevelName.y + rectWithLevelName.h/2 - levelStringOutline->h/2, levelStringOutline->w, levelStringOutline->h};
    SDL_Rect shadowDST = stringDST;
    shadowDST.x += 5;
    shadowDST.y += 5;
    SDL_RenderCopy(renderer, levelShadowTexture, NULL, &shadowDST);
    SDL_RenderCopy(renderer, levelStringTexture, NULL, &stringDST);
    SDL_FreeSurface(levelShadowSurface);
    SDL_DestroyTexture(levelShadowTexture);
    SDL_FreeSurface(levelStringOutline);
    SDL_DestroyTexture(levelStringTexture);
}
