#include "PlayingState.h"
#include "Game.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Text.h"
#include <SDL_image.h>
#include <string>
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
    m_ResumeTexture = IMG_LoadTexture(m_Renderer, "res/gfx/resume.png");
    if (m_ResumeTexture == nullptr) {
        std::cerr << "Failed to load resume.png! " << SDL_GetError() << std::endl;
    }
    m_ExitTexture = IMG_LoadTexture(m_Renderer, "res/gfx/exitLevel.png");
    if (m_ExitTexture == nullptr) {
        std::cerr << "Failed to load exitLevel.png! " << SDL_GetError() << std::endl;
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
}

void PlayingState::destroy() {
    for (int i = 0; i < m_Game->LEVEL_COUNT; i++) {
        Mix_FreeMusic(m_Songs[i]);
    }
}

void PlayingState::enter() {
    Mix_HaltMusic();
    m_Player.reset();
    m_ObjectManager.reset();
    m_Timer = 60;
    m_IsTimerFinished = false;
    m_IsSongPlaying = false;
    m_IsPlayerDead = false;
    m_IsEscapeHeld = false;
    m_IsSpaceHeld = false;
    m_IsPaused = false;
}

void PlayingState::exit() {
    resetMusic();
    m_Game->restartMenuLoop();
    m_Game->setCameraPosition({0, 0});
}

void PlayingState::update(float deltaTime) {
    const bool isEscapeHeld = m_Game->isEscapeHeld();
    const bool isEscapeReleased = m_IsEscapeHeld && !isEscapeHeld;
    m_IsEscapeHeld = isEscapeHeld;

    if (m_IsPaused) {
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
}

void PlayingState::render() {
    m_ObjectManager.render();
    m_Player.render();
    if (m_IsPaused) {
        renderPause();
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
