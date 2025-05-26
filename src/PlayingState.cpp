#include "PlayingState.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "ObjectManager.hpp"
#include <string>
#include <iostream>

PlayingState::PlayingState(Game *game) : m_Game(game) {
    m_Player = new Player(m_Game);
    m_ObjectManager = new ObjectManager(m_Game);
    m_Timer = 60;
    m_IsTimerFinished = false;
    m_IsSongPlaying = false;
    m_IsPlayerDead = false;
    m_Songs.clear();

    for (int i = 0; i < m_Game->LEVEL_COUNT; i++) {
        m_Songs.push_back(Mix_LoadMUS(("res/sfx/" + std::to_string(i) + ".wav").c_str()));
    }
}

PlayingState::~PlayingState() {
    // RAII at its finest
    delete m_Player;
    delete m_ObjectManager;
}

void PlayingState::update(int &gameState, float delta, bool isMouseHeld) {
    if (gameState == PAUSED) {
        return;
    }
    if (m_Timer > 0) {
        m_Timer--;
        return;
    }
    m_IsTimerFinished = true;
    bool playerJustRevived = m_IsPlayerDead && !m_Player->isDead();
    if (!m_IsSongPlaying || playerJustRevived)
    {
        Mix_PlayMusic(m_Songs[m_Game->getLevelSelected()], 0);
        m_IsSongPlaying = true;
    }
    m_IsPlayerDead = m_Player->isDead();
    m_Player->update(delta, isMouseHeld, m_ObjectManager->getObjects());
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
    m_ObjectManager->render();
    m_Player->render();
}

int PlayingState::getPlayerGamemode() {
    return m_Player->getGamemode();
}
