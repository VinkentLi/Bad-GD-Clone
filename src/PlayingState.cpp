#include "PlayingState.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "ObjectManager.hpp"
#include <string>
#include <iostream>

PlayingState::PlayingState(Game *game) : m_Game(game) {
    m_Player.init(m_Game);
    m_ObjectManager.init(m_Game);
    m_Timer = 60;
    m_IsTimerFinished = false;
    m_IsSongPlaying = false;
    m_IsPlayerDead = false;
    m_IsEscapeHeld = false;
    m_IsSpaceHeld = false;
    m_Songs.clear();

    for (int i = 0; i < m_Game->LEVEL_COUNT; i++) {
        m_Songs.push_back(Mix_LoadMUS(("res/sfx/" + std::to_string(i) + ".wav").c_str()));
    }
}

PlayingState::~PlayingState() {
    for (int i = 0; i < m_Game->LEVEL_COUNT; i++) {
        Mix_FreeMusic(m_Songs[i]);
    }
}

void PlayingState::update(float deltaTime) {
    int &gameState = m_Game->getGameState();
    
    const bool isEscapeHeld = m_Game->isEscapeHeld();
    const bool isEscapeReleased = m_IsEscapeHeld && !isEscapeHeld;
    m_IsEscapeHeld = isEscapeHeld;

    if (gameState == PAUSED) {
        if (isEscapeReleased) {
            gameState = LEVEL_SELECT;
            resetMusic();
            m_Game->restartMenuLoop();
            m_Game->setCameraPosition({0, 0});
            return;
        }
        const bool isSpaceHeld = m_Game->isSpaceHeld();
        const bool spaceReleased = m_IsSpaceHeld && !isSpaceHeld;
        m_IsSpaceHeld = isSpaceHeld;
        if (spaceReleased) {
            setBackToPlay(gameState);
        }
        return;
    }

    if (isEscapeReleased) {
        setToPause(gameState);
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

void PlayingState::setToPause(int &gameState) {
    gameState = PAUSED;
    Mix_PauseMusic();
}

void PlayingState::setBackToPlay(int &gameState) {
    gameState = PLAYING;
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
