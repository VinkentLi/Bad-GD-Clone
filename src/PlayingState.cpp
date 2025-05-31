#include "PlayingState.h"
#include "Game.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Text.h"
#include <SDL_image.h>
#include <string>
#include <algorithm>
#include <iostream>

PlayingState PlayingState::m_PlayingState;

void PlayingState::init(Game *game) {
    GameState::init(game);
    m_Name = "PlayingState";
    m_Player.init(m_Game);
    m_ObjectManager.init(m_Game);
    for (int i = 0; i < m_Game->LEVEL_COUNT; i++) {
        m_Songs.push_back(Mix_LoadMUS(("res/sfx/" + std::to_string(i) + ".wav").c_str()));
    }
    m_LevelEndBlockTexture = IMG_LoadTexture(m_Renderer, "res/gfx/objects/2.png");
    if (m_LevelEndBlockTexture == nullptr) {
        std::cerr << "Failed to load 2.png! " << SDL_GetError() << std::endl;
    }
    m_ResumeTexture = IMG_LoadTexture(m_Renderer, "res/gfx/resume.png");
    if (m_ResumeTexture == nullptr) {
        std::cerr << "Failed to load resume.png! " << SDL_GetError() << std::endl;
    }
    m_ExitTexture = IMG_LoadTexture(m_Renderer, "res/gfx/exitLevel.png");
    if (m_ExitTexture == nullptr) {
        std::cerr << "Failed to load exitLevel.png! " << SDL_GetError() << std::endl;
    }
    m_LevelCompleteSound = Mix_LoadWAV("res/sfx/levelComplete.wav");
    if (m_LevelCompleteSound == nullptr) {
        std::cerr << "Failed to load levelComplete.wav! " << SDL_GetError() << std::endl;
    }
    int resumeWidth = 0;
    int resumeHeight = 0;
    int exitWidth = 0;
    int exitHeight = 0;
    SDL_QueryTexture(m_ResumeTexture, NULL, NULL, &resumeWidth, &resumeHeight);
    SDL_QueryTexture(m_ExitTexture, NULL, NULL, &exitWidth, &exitHeight);
    const int width = m_Game->getWidth();
    const int height = m_Game->getHeight();
    int resumeX = width/2 - (resumeWidth + MARGIN + exitWidth)/2;
    int exitX = width/2 + (resumeWidth + MARGIN - exitWidth)/2;
    m_ResumeButton.init(m_Game, m_ResumeTexture, resumeX, height/2, resumeWidth, resumeHeight, false, true);
    m_ExitButton.init(m_Game, m_ExitTexture, exitX, height/2, exitWidth, exitHeight, false, true);
    m_MinY = m_Game->getHeight() - 50*m_Game->TILE_SIZE; // no idea what the actual game value is lmao
}

void PlayingState::destroy() {
    for (int i = 0; i < m_Game->LEVEL_COUNT; i++) {
        Mix_FreeMusic(m_Songs[i]);
    }
    SDL_DestroyTexture(m_LevelEndBlockTexture);
    SDL_DestroyTexture(m_ResumeTexture);
    SDL_DestroyTexture(m_ExitTexture);
    Mix_FreeChunk(m_LevelCompleteSound);
}

void PlayingState::enter() {
    Mix_HaltMusic();
    m_Player.reset();
    m_ObjectManager.reset();
    int exitX = m_Game->getWidth()/2 + (m_ResumeButton.getW() + MARGIN - m_ExitButton.getW())/2;
    m_ExitButton.setPosition(exitX, m_Game->getHeight()/2);
    m_Timer = 60;
    m_IsTimerFinished = false;
    m_IsSongPlaying = false;
    m_IsPlayerDead = false;
    m_IsEscapeHeld = false;
    m_IsSpaceHeld = false;
    m_IsPaused = false;
    m_ShouldEndLevel = false;
    m_LevelEndBlocksX = m_ObjectManager.getFurthestX() + 12*m_Game->TILE_SIZE;
    m_TimeEndingLevel = 0;
    m_IsLevelComplete = false;
    m_LevelPercent = 0;
}

void PlayingState::exit() {
    resetMusic();
    m_Game->restartMenuLoop();
    m_Game->setCameraPosition({0, 0});
}

void PlayingState::update(float deltaTime) {
    m_LevelPercent = 10000 * m_Player.getPosition().x / m_LevelEndBlocksX;
    m_LevelPercent = std::clamp(m_LevelPercent, 0, 10000);

    const bool isEscapeHeld = m_Game->isEscapeHeld();
    const bool isEscapeReleased = m_IsEscapeHeld && !isEscapeHeld;
    m_IsEscapeHeld = isEscapeHeld;

    if (m_IsLevelComplete) {
        updateLevelComplete(isEscapeReleased);
        return;
    }
    if (m_ShouldEndLevel) {
        updateEndLevel(deltaTime);
        return;
    }

    if (m_IsPaused) {
        updatePause(isEscapeReleased);
        return;
    }
    if (isEscapeReleased) {
        pause();
    }

    Background &background = m_Game->getBackground();
    Ground &ground = m_Game->getGround();
    ground.setPosition({ground.getPosition().x, m_Game->getHeight() - 300.0f});
    background.update(deltaTime);

    if (m_Game->getCameraPosition().x != 0) {
        background.setMoving(true);
    }
    if (m_Player.getGamemode() == Gamemode::SHIP) {
        ground.setOnTop(true);
    } else {
        ground.setOnTop(false);
    }

    if (m_Timer > 0) {
        m_Timer--;
        return;
    }
    m_IsTimerFinished = true;
    bool playerJustRevived = m_IsPlayerDead && !m_Player.isDead();
    if (!m_IsSongPlaying || playerJustRevived)
    {
        Mix_PlayMusic(m_Songs[m_Game->getLevelSelected()], 0);
        m_IsSongPlaying = true;
    }
    m_IsPlayerDead = m_Player.isDead();
    m_Player.update(deltaTime, m_Game->isMouseHeld(), m_ObjectManager.getObjects());
    
    if (m_Player.getPosition().x > m_ObjectManager.getFurthestX()) {
        m_ShouldEndLevel = true;
    }
}

void PlayingState::updatePause(bool isEscapeReleased) {
    m_ExitButton.update();
    if (m_ExitButton.isPressed() || isEscapeReleased) {
        m_Game->popState();
        return;
    }
    const bool isSpaceHeld = m_Game->isSpaceHeld();
    const bool spaceReleased = m_IsSpaceHeld && !isSpaceHeld;
    m_IsSpaceHeld = isSpaceHeld;

    m_ResumeButton.update();
    if (m_ResumeButton.isPressed() || spaceReleased) {
        resume();
    }
}

void PlayingState::updateEndLevel(float deltaTime) {
    m_TimeEndingLevel += deltaTime;
    // i just made this up lmao
    constexpr float CAM_SPEED = 1.8f;
    float xAdder = m_TimeEndingLevel / 35.0f;
    m_Player.moveX(xAdder);
    m_Game->setCameraX(m_Game->getCameraPosition().x + CAM_SPEED * deltaTime);
    if (m_Player.getPosition().x > m_LevelEndBlocksX) {
        m_IsLevelComplete = true;
        m_ShouldEndLevel = false;
        m_ExitButton.setPosition(m_Game->getWidth()/2, m_Game->getHeight()/2, true, true);
        Mix_PlayChannel(-1, m_LevelCompleteSound, 0);
    }
}

void PlayingState::updateLevelComplete(bool isEscapeReleased) {
    m_ExitButton.update();
    if (m_ExitButton.isPressed() || isEscapeReleased) {
        m_Game->popState();
    }
}

void PlayingState::render() {
    m_Player.render();
    m_ObjectManager.render();
    renderEndBlocks();
    Text::renderText(
        m_Renderer, 
        std::to_string(m_LevelPercent/100) + "." + std::to_string(m_LevelPercent % 100), 
        m_Game->getWidth()/2, 
        10, 
        true,
        false,
        0.5f
    );
    if (m_IsPaused) {
        renderPause();
    }
    if (m_IsLevelComplete) {
        renderLevelCompleteMenu();
    }
}

void PlayingState::renderPause() {
    SDL_Rect screen = { 0, 0, m_Game->getWidth(), m_Game->getHeight() };
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 100);
    SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(m_Renderer, &screen);
    const std::string levelName = m_Game->getLevelStrings()[m_Game->getLevelSelected()];
    Text::renderText(m_Renderer, levelName, m_Game->getWidth()/2, MARGIN, true, true);
    m_ResumeButton.render();
    m_ExitButton.render();
}

void PlayingState::renderEndBlocks() {
    SDL_Rect dst = { 
        m_LevelEndBlocksX - static_cast<int>(m_Game->getCameraPosition().x),
        m_MinY - static_cast<int>(m_Game->getCameraPosition().y),
        m_Game->TILE_SIZE,
        m_Game->TILE_SIZE
    };
    for (int i = 0; i < 47; i++) {
        SDL_RenderCopyEx(m_Renderer, m_LevelEndBlockTexture, NULL, &dst, -90.0, NULL, SDL_FLIP_NONE);
        dst.y += m_Game->TILE_SIZE;
    }
}

void PlayingState::renderLevelCompleteMenu() {
    Text::renderText(m_Renderer, "Your did it!", m_Game->getWidth()/2, m_Game->getHeight()/4, true, true);
    m_ExitButton.render();
}

void PlayingState::pause() {
    m_IsPaused = true;
    Mix_PauseMusic();
}

void PlayingState::resume() {
    m_IsPaused = false;
    Mix_ResumeMusic();
}

void PlayingState::resetMusic() {
    Mix_HaltMusic();
}

void PlayingState::attemptResetTimer() {
    if (m_IsTimerFinished) {
        return;
    }
    m_Timer = 60;
}
