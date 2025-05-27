#include "LevelSelect.hpp"
#include "Game.hpp"
#include <SDL_image.h>
#include <iostream>

LevelSelect::LevelSelect(Game *game) : m_Game(game) {
    m_Renderer = m_Game->getRenderer();
    const int WIDTH = m_Game->getWidth();
    const int HEIGHT = m_Game->getHeight();
    // TODO: fix magic numbers
    m_LeftCornerDST = {0, HEIGHT - 282, 285, 282};
    m_RightCornerDST = {WIDTH - 285, HEIGHT - 282, 285, 282};
    m_TopDST = {WIDTH/2 - 1226/2*3/4, 0, 1226*3/4, 144*3/4};
    m_LeftLevelArrowDST = {40, HEIGHT/2 - 238/2, 106, 238};
    m_RightLevelArrowDST = {WIDTH - 40 - 106, HEIGHT/2 - 238/2, 106, 238};
    m_TitleArrowDST = {40, 20, 124, 150};
    m_Corner = IMG_LoadTexture(m_Renderer, "res/gfx/selectCorner.png");
    m_Top = IMG_LoadTexture(m_Renderer, "res/gfx/top.png");
    m_LevelArrow = IMG_LoadTexture(m_Renderer, "res/gfx/levelArrow.png");
    m_TitleArrow = IMG_LoadTexture(m_Renderer, "res/gfx/toTitleScreen.png");
    m_IsMouseHeld = false;
    m_NeedToRecallPlayingStateConstructor = false;
    m_Game->setLevelSelected(0);
    m_LevelStrings.push_back("Test level1");
    m_LevelStrings.push_back("Test Level2");
    m_Font = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    m_FontOutline = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    TTF_SetFontOutline(m_FontOutline, 4);
    m_RectWithLevelName.w = 1000;
    m_RectWithLevelName.h = 300;
    m_RectWithLevelName.x = WIDTH / 2 - 1000/2;
    m_RectWithLevelName.y = HEIGHT / 2 - 350;
    m_PlaySound = Mix_LoadWAV("res/sfx/playSound.ogg");
}

void LevelSelect::update(float deltaTime) {
    const bool isMouseHeld = m_Game->isMouseHeld();
    const bool isMouseReleased = m_IsMouseHeld && !isMouseHeld;
    if (isMouseReleased) {  
        SDL_Point mousePosition = m_Game->getMousePosition();
        float wScale = m_Game->getScreenWidth() / (float) m_Game->getWidth();
        float hScale = m_Game->getScreenHeight() / (float) m_Game->getHeight();
        SDL_Rect scaledTitleArrow = m_TitleArrowDST;
        SDL_Rect scaledLeft = m_LeftLevelArrowDST;
        SDL_Rect scaledRight = m_RightLevelArrowDST;
        SDL_Rect scaledLevelRect = m_RectWithLevelName;
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

        int &gameState = m_Game->getGameState();

        if (SDL_PointInRect(&mousePosition, &scaledTitleArrow)) {
            gameState = TITLE_SCREEN;
        } else if (SDL_PointInRect(&mousePosition, &scaledLeft)) {
            m_Game->decreaseLevelSelected();
            if (m_Game->getLevelSelected() < 0) {
                m_Game->setLevelSelected(m_Game->LEVEL_COUNT - 1);
            }
        } else if (SDL_PointInRect(&mousePosition, &scaledRight)) {
            m_Game->increaseLevelSelected();
            if (m_Game->getLevelSelected() == m_Game->LEVEL_COUNT) {
                m_Game->setLevelSelected(0);
            }
        } else if (SDL_PointInRect(&mousePosition, &scaledLevelRect)) {
            gameState = PLAYING;
            m_NeedToRecallPlayingStateConstructor = true;
            Mix_HaltMusic();
            Mix_PlayChannel(0, m_PlaySound, 0);
        }
    }
    m_IsMouseHeld = isMouseHeld;
}

void LevelSelect::render() {
    // render textures
    SDL_RenderCopyEx(m_Renderer, m_Corner, NULL, &m_LeftCornerDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(m_Renderer, m_Corner, NULL, &m_RightCornerDST, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(m_Renderer, m_Top, NULL, &m_TopDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(m_Renderer, m_LevelArrow, NULL, &m_LeftLevelArrowDST, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(m_Renderer, m_LevelArrow, NULL, &m_RightLevelArrowDST, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(m_Renderer, m_TitleArrow, NULL, &m_TitleArrowDST, 0, NULL, SDL_FLIP_NONE);

    // render the rectangle with the level name
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 100);
    SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(m_Renderer, &m_RectWithLevelName);

    // render level name
    std::string levelName = m_LevelStrings[m_Game->getLevelSelected()];
    SDL_Surface *levelShadowSurface = TTF_RenderText_Blended(m_Font, levelName.c_str(), {0, 0, 0, 100});
    SDL_Surface *levelStringSurface = TTF_RenderText_Blended(m_Font, levelName.c_str(), {255, 255, 255});
    SDL_Surface *levelStringOutline = TTF_RenderText_Blended(
        m_FontOutline,
        m_LevelStrings[m_Game->getLevelSelected()].c_str(),
        {0, 0, 0}
    );
    SDL_SetSurfaceBlendMode(levelStringSurface, SDL_BLENDMODE_BLEND);
    SDL_Rect rect = {4, 4, levelStringSurface->w, levelStringSurface->h};
    SDL_BlitSurface(levelStringSurface, NULL, levelStringOutline, &rect);
    SDL_FreeSurface(levelStringSurface);
    SDL_Texture *levelShadowTexture = SDL_CreateTextureFromSurface(m_Renderer, levelShadowSurface);
    SDL_Texture *levelStringTexture = SDL_CreateTextureFromSurface(m_Renderer, levelStringOutline);
    SDL_Rect stringDST = {
        m_Game->getWidth()/2 - levelStringOutline->w/2,
        m_RectWithLevelName.y + m_RectWithLevelName.h/2 - levelStringOutline->h/2, 
        levelStringOutline->w, 
        levelStringOutline->h
    };
    SDL_Rect shadowDST = stringDST;
    shadowDST.x += 5;
    shadowDST.y += 5;
    SDL_RenderCopy(m_Renderer, levelShadowTexture, NULL, &shadowDST);
    SDL_RenderCopy(m_Renderer, levelStringTexture, NULL, &stringDST);
    SDL_FreeSurface(levelShadowSurface);
    SDL_DestroyTexture(levelShadowTexture);
    SDL_FreeSurface(levelStringOutline);
    SDL_DestroyTexture(levelStringTexture);
}

bool LevelSelect::getNeedToRecallPlayingStateConstructor() {
    if (m_NeedToRecallPlayingStateConstructor) {
        m_NeedToRecallPlayingStateConstructor = false;
        return true;
    }
    return false;
}
