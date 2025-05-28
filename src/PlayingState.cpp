#include "PlayingState.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "ObjectManager.hpp"
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
        if (isEscapeReleased) {
            m_Game->popState();
            return;
        }
        const bool isSpaceHeld = m_Game->isSpaceHeld();
        const bool spaceReleased = m_IsSpaceHeld && !isSpaceHeld;
        m_IsSpaceHeld = isSpaceHeld;
        if (spaceReleased) {
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
    if (m_Player.getGamemode() == SHIP) {
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

void PlayingState::render() {
    m_ObjectManager.render();
    m_Player.render();
}

int PlayingState::getPlayerGamemode() {
    return m_Player.getGamemode();
}
