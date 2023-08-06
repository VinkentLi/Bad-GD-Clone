#include "TitleScreen.hpp"

TitleScreen::TitleScreen()
{
    titleTexture = IMG_LoadTexture(renderer, "res/gfx/title.png");

    if (titleTexture == NULL)
    {
        std::cerr << "Failed to load title.png! " << SDL_GetError() << std::endl;
    }

    titlePlay = IMG_LoadTexture(renderer, "res/gfx/titlePlay.png");

    if (titlePlay == NULL)
    {
        std::cerr << "Failed to load titlePlay.png! " << SDL_GetError() << std::endl;
    }

    titleDST = {WIDTH / 2 - (1699*3/4) / 2, 100, 1699*3/4, 206*3/4}; // lol
    titlePlayDST = {WIDTH/2 - 415/2*3/4, HEIGHT/2 - 415/2*3/4 - 40, 415*3/4, 415*3/4}; // super readable btw
    mouseHeld = false;
}

void TitleScreen::update(int &gameState, SDL_Point *mousePos, bool mouseHeld)
{
    if (this->mouseHeld && !mouseHeld)
    {
        SDL_Rect scaledTitlePlay;
        float wScale = SCREEN_WIDTH / (float) WIDTH;
        float hScale = SCREEN_HEIGHT / (float) HEIGHT;
        scaledTitlePlay.x = titlePlayDST.x * wScale;
        scaledTitlePlay.y = titlePlayDST.y * hScale;
        scaledTitlePlay.w = titlePlayDST.w * wScale;
        scaledTitlePlay.h = titlePlayDST.h * hScale;
        
        if (SDL_PointInRect(mousePos, &scaledTitlePlay))
        {
            gameState = LEVEL_SELECT;
        }
    }

    this->mouseHeld = mouseHeld;
    this->mousePos = *mousePos;
}

void TitleScreen::render()
{
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleDST);
    SDL_RenderCopy(renderer, titlePlay, NULL, &titlePlayDST);
}
