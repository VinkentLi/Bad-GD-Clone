#include "LevelSelect.hpp"

LevelSelect::LevelSelect()
{
    m_LeftCornerDST = {0, HEIGHT - 282, 285, 282};
    m_RightCornerDST = {WIDTH - 285, HEIGHT - 282, 285, 282};
    m_TopDST = {WIDTH/2 - 1226/2*3/4, 0, 1226*3/4, 144*3/4};
    m_LeftLevelArrowDST = {40, HEIGHT/2 - 238/2, 106, 238};
    m_RightLevelArrowDST = {WIDTH - 40 - 106, HEIGHT/2 - 238/2, 106, 238};
    m_TitleArrowDST = {40, 20, 124, 150};
    m_Corner = IMG_LoadTexture(renderer, "res/gfx/selectCorner.png");
    m_Top = IMG_LoadTexture(renderer, "res/gfx/top.png");
    m_LevelArrow = IMG_LoadTexture(renderer, "res/gfx/levelArrow.png");
    m_TitleArrow = IMG_LoadTexture(renderer, "res/gfx/toTitleScreen.png");
    m_MousePos = {0, 0};
    m_MouseHeld = false;
    m_NeedToRecallPlayingStateConstructor = false;
    levelSelected = 0;
    levelStrings.push_back("Test level1");
    levelStrings.push_back("Test Level2");
    m_Font = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    m_FontOutline = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    TTF_SetFontOutline(m_FontOutline, 4);
    m_RectWithLevelName.w = 1000;
    m_RectWithLevelName.h = 300;
    m_RectWithLevelName.x = WIDTH / 2 - 1000/2;
    m_RectWithLevelName.y = HEIGHT / 2 - 350;
    m_PlaySound = Mix_LoadWAV("res/sfx/playSound.ogg");
}

void LevelSelect::update(int &gameState, SDL_Point *mousePos, bool mouseHeld)
{
    if (this->m_MouseHeld && !mouseHeld)
    {  
        float wScale = SCREEN_WIDTH / (float) WIDTH;
        float hScale = SCREEN_HEIGHT / (float) HEIGHT;
        SDL_Rect scaledTitleArrow = m_TitleArrowDST, scaledLeft = m_LeftLevelArrowDST, scaledRight = m_RightLevelArrowDST, scaledLevelRect = m_RectWithLevelName;
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
        scaledLevelRect.x *= wScale;
        scaledLevelRect.y *= hScale;
        scaledLevelRect.w *= wScale;
        scaledLevelRect.h *= hScale;

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
        else if (SDL_PointInRect(mousePos, &scaledLevelRect))
        {
            gameState = PLAYING;
            m_NeedToRecallPlayingStateConstructor = true;
            Mix_HaltMusic();
            Mix_PlayChannel(0, m_PlaySound, 0);
        }
    }

    this->m_MouseHeld = mouseHeld;
    this->m_MousePos = *mousePos;
}

void LevelSelect::render()
{
    // render textures

    SDL_RenderCopyEx(renderer, m_Corner, NULL, &m_LeftCornerDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, m_Corner, NULL, &m_RightCornerDST, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(renderer, m_Top, NULL, &m_TopDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, m_LevelArrow, NULL, &m_LeftLevelArrowDST, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(renderer, m_LevelArrow, NULL, &m_RightLevelArrowDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, m_TitleArrow, NULL, &m_TitleArrowDST, 0, NULL, SDL_FLIP_NONE);

    // render the rectangle with the level name

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &m_RectWithLevelName);

    // render level name

    std::string levelName = levelStrings[levelSelected];

    SDL_Surface *levelShadowSurface = TTF_RenderText_Blended(m_Font, levelName.c_str(), {0, 0, 0, 100});
    SDL_Surface *levelStringSurface = TTF_RenderText_Blended(m_Font, levelName.c_str(), {255, 255, 255});
    SDL_Surface *levelStringOutline = TTF_RenderText_Blended(m_FontOutline, levelStrings[levelSelected].c_str(), {0, 0, 0});
    SDL_SetSurfaceBlendMode(levelStringSurface, SDL_BLENDMODE_BLEND);
    SDL_Rect rect = {4, 4, levelStringSurface->w, levelStringSurface->h};
    SDL_BlitSurface(levelStringSurface, NULL, levelStringOutline, &rect);
    SDL_FreeSurface(levelStringSurface);
    SDL_Texture *levelShadowTexture = SDL_CreateTextureFromSurface(renderer, levelShadowSurface);
    SDL_Texture *levelStringTexture = SDL_CreateTextureFromSurface(renderer, levelStringOutline);
    SDL_Rect stringDST = {WIDTH/2 - levelStringOutline->w/2, m_RectWithLevelName.y + m_RectWithLevelName.h/2 - levelStringOutline->h/2, levelStringOutline->w, levelStringOutline->h};
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

bool LevelSelect::getNeedToRecallPlayingStateConstructor()
{
    if (m_NeedToRecallPlayingStateConstructor)
    {
        m_NeedToRecallPlayingStateConstructor = false;
        return true;
    }

    return false;
}
