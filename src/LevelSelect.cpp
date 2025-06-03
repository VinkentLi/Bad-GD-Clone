#include "LevelSelect.h"
#include "PlayingState.h"
#include "Game.h"
#include "Text.h"
#include <SDL_image.h>
#include <iostream>

LevelSelect LevelSelect::m_LevelSelect;

void LevelSelect::init(Game *game) {
    GameState::init(game);
    m_Name = "LevelSelect";
    m_Renderer = m_Game->getRenderer();
    m_CornerTexture = IMG_LoadTexture(m_Renderer, "res/gfx/selectCorner.png");
    if (m_CornerTexture == nullptr) {
        std::cerr << "Failed to load selectCorner.png!" << SDL_GetError() << std::endl;
    }
    m_TopTexture = IMG_LoadTexture(m_Renderer, "res/gfx/top.png");
    if (m_TopTexture == nullptr) {
        std::cerr << "Failed to load top.png!" << SDL_GetError() << std::endl;
    }
    m_LevelArrowTexture = IMG_LoadTexture(m_Renderer, "res/gfx/levelArrow.png");
    if (m_LevelArrowTexture == nullptr) {
        std::cerr << "Failed to load levelArrow.png!" << SDL_GetError() << std::endl;
    }
    m_TitleArrowTexture = IMG_LoadTexture(m_Renderer, "res/gfx/toTitleScreen.png");
    if (m_TitleArrowTexture == nullptr) {
        std::cerr << "Failed to load toTitleScreen.png!" << SDL_GetError() << std::endl;
    }
    m_LevelStrings = { "Test Level1", "Test Level2" };
    m_LevelNameTexture = Text::createTexture(m_Renderer, m_LevelStrings[0]);
    m_BestPercentTexture = Text::createTexture(m_Renderer, "Normal: 0%");
    m_BestPracticePercentTexture = Text::createTexture(m_Renderer, "Practice: 0%");
    int cornerWidth = 0;
    int cornerHeight = 0;
    int topWidth = 0;
    int topHeight = 0;
    int levelArrowWidth = 0;
    int levelArrowHeight = 0;
    int titleArrowWidth = 0;
    int titleArrowHeight = 0;
    SDL_QueryTexture(m_CornerTexture, NULL, NULL, &cornerWidth, &cornerHeight);
    SDL_QueryTexture(m_TopTexture, NULL, NULL, &topWidth, &topHeight);
    SDL_QueryTexture(m_LevelArrowTexture, NULL, NULL, &levelArrowWidth, &levelArrowHeight);
    SDL_QueryTexture(m_TitleArrowTexture, NULL, NULL, &titleArrowWidth, &titleArrowHeight);
    const int width = m_Game->getWidth();
    const int height = m_Game->getHeight();
    const int xMargin = 40;
    const int yMargin = 20;
    m_LeftCornerRect = {0, height - cornerHeight, cornerWidth, cornerHeight};
    m_RightCornerRect = {width - cornerWidth, height - cornerHeight, cornerWidth, cornerHeight};
    m_TopRect = {width/2 - topWidth/2, 0, topWidth, topHeight};
    m_LeftLevelArrow.init(
        m_Game, 
        m_LevelArrowTexture, 
        xMargin, 
        height/2, 
        levelArrowWidth, 
        levelArrowHeight, 
        false, 
        true,
        SDL_FLIP_HORIZONTAL
    );
    m_RightLevelArrow.init(
        m_Game, 
        m_LevelArrowTexture, 
        width - xMargin - levelArrowWidth, 
        height/2, 
        levelArrowWidth, 
        levelArrowHeight, 
        false, 
        true
    );
    m_TitleScreenArrow.init(
        m_Game,
        m_TitleArrowTexture,
        xMargin,
        yMargin,
        titleArrowWidth,
        titleArrowHeight
    );
    m_EnterLevel.init(
        m_Game,
        width/2,
        240,
        1200,
        360,
        {.r=0, .g=0, .b=0, .a=100},
        true
    );
    m_BestLevelPercentages.resize(m_Game->LEVEL_COUNT);
    m_BestPracticePercentages.resize(m_Game->LEVEL_COUNT);
    std::fill(m_BestLevelPercentages.begin(), m_BestLevelPercentages.end(), 0);
    std::fill(m_BestPracticePercentages.begin(), m_BestPracticePercentages.end(), 0);
    m_PlaySound = Mix_LoadWAV("res/sfx/playSound.ogg");
}

void LevelSelect::destroy() {
    SDL_DestroyTexture(m_CornerTexture);
    SDL_DestroyTexture(m_TopTexture);
    SDL_DestroyTexture(m_LevelArrowTexture);
    SDL_DestroyTexture(m_TitleArrowTexture);
    SDL_DestroyTexture(m_BestPercentTexture);
    SDL_DestroyTexture(m_BestPracticePercentTexture);
    Mix_FreeChunk(m_PlaySound);
}

void LevelSelect::enter() {
    m_IsMouseHeld = false;
    m_IsEscapeHeld = false;
    m_IsSpaceHeld = false;
    m_LevelSelected = 0;
}

void LevelSelect::exit() {

}

void LevelSelect::update(float deltaTime) {
    const bool isEscapeHeld = m_Game->isEscapeHeld();
    const bool isEscapeReleased = m_IsEscapeHeld && !isEscapeHeld;
    m_IsEscapeHeld = isEscapeHeld;
    if (isEscapeReleased) {
        m_Game->popState();
    }
    Background &background = m_Game->getBackground();
    Ground &ground = m_Game->getGround();
    ground.setPosition({0, static_cast<float>(m_Game->getHeight() - 2*m_Game->TILE_SIZE)});
    ground.setOnTop(false);
    background.setMoving(false);

    const bool isSpaceHeld = m_Game->isSpaceHeld();
    const bool spaceReleased = m_IsSpaceHeld && !isSpaceHeld;
    m_IsSpaceHeld = isSpaceHeld;

    m_TitleScreenArrow.update();
    if (m_TitleScreenArrow.isPressed()) {
        m_Game->popState();
    }
    m_LeftLevelArrow.update();
    if (m_LeftLevelArrow.isPressed()) {
        m_LevelSelected--;
        if (m_LevelSelected < 0) {
            m_LevelSelected = m_Game->LEVEL_COUNT - 1;
        }
        updateLevelNameTexture();
        updateBestPercentTexture();
        updateBestPracticePercentTexture();
    }
    m_RightLevelArrow.update();
    if (m_RightLevelArrow.isPressed()) {
        m_LevelSelected++;
        if (m_LevelSelected == m_Game->LEVEL_COUNT) {
            m_LevelSelected = 0;
        }
        updateLevelNameTexture();
        updateBestPercentTexture();
        updateBestPracticePercentTexture();
    }
    m_EnterLevel.update();
    if (m_EnterLevel.isPressed() || spaceReleased) {
        Mix_PlayChannel(0, m_PlaySound, 0);
        m_Game->pushState(PlayingState::get());
    }
}

void LevelSelect::render() {
    // render textures
    SDL_RenderCopyEx(m_Renderer, m_CornerTexture, NULL, &m_LeftCornerRect, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(m_Renderer, m_CornerTexture, NULL, &m_RightCornerRect, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopyEx(m_Renderer, m_TopTexture, NULL, &m_TopRect, 0, NULL, SDL_FLIP_NONE);
    m_TitleScreenArrow.render();
    m_LeftLevelArrow.render();
    m_RightLevelArrow.render();
    m_EnterLevel.render();

    Text::renderTexture(m_Renderer, m_LevelNameTexture, m_Game->getWidth()/2, m_EnterLevel.getY() + m_EnterLevel.getH()/2, true, true);
    constexpr int MARGIN = 50;
    const int bestPercentY = m_EnterLevel.getY() + m_EnterLevel.getH() + MARGIN;
    Text::renderTexture(m_Renderer, m_BestPercentTexture, m_Game->getWidth()/2, bestPercentY, true, false, 0.5f);
    Text::renderTexture(m_Renderer, m_BestPracticePercentTexture, m_Game->getWidth()/2, bestPercentY + 2*MARGIN, true, false, 0.5f);
}

int LevelSelect::getBestPercentage() {
    return m_BestLevelPercentages[m_LevelSelected];
}

void LevelSelect::setBestPercentage(int value) {
    m_BestLevelPercentages[m_LevelSelected] = value;
    updateBestPercentTexture();
}

int LevelSelect::getBestPracticePercentage() {
    return m_BestPracticePercentages[m_LevelSelected];
}

void LevelSelect::setBestPracticePercentage(int value) {
    m_BestPracticePercentages[m_LevelSelected] = value;
    updateBestPracticePercentTexture();
}

void LevelSelect::updateLevelNameTexture() {
    SDL_DestroyTexture(m_LevelNameTexture);
    m_LevelNameTexture = Text::createTexture(m_Renderer, m_LevelStrings[m_LevelSelected]);
}

void LevelSelect::updateBestPercentTexture() {
    SDL_DestroyTexture(m_BestPercentTexture);
    m_BestPercentTexture = Text::createTexture(m_Renderer, "Normal: " + std::to_string(m_BestLevelPercentages[m_LevelSelected]) + "%");
}

void LevelSelect::updateBestPracticePercentTexture() {
    SDL_DestroyTexture(m_BestPracticePercentTexture);
    m_BestPracticePercentTexture = Text::createTexture(m_Renderer, "Practice: " + std::to_string(m_BestPracticePercentages[m_LevelSelected]) + "%");
}
