#include "PlayingState.hpp"

PlayingState::PlayingState()
{
    m_Player = new Player();
    m_ObjectManager = new ObjectManager();
    m_Timer = 60;
    m_TimerFinished = false;
    m_SongPlaying = false;
    m_PlayerIsDead = false;
    m_Songs.clear();

    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        m_Songs.push_back(Mix_LoadMUS(("res/sfx/" + std::to_string(i) + ".mp3").c_str()));
    }
}

PlayingState::~PlayingState()
{
    delete m_Player;
    delete m_ObjectManager;
}

void PlayingState::update(int &gameState, float delta, bool mouseHeld)
{
    if (gameState == PAUSED)
    {
        return;
    }

    if (m_Timer > 0)
    {
        m_Timer--;
        return;
    }

    m_TimerFinished = true;

    if (!m_SongPlaying || (m_PlayerIsDead && !m_Player->isDead()))
    {
        Mix_PlayMusic(m_Songs[levelSelected], 0);
        m_SongPlaying = true;
    }

    m_PlayerIsDead = m_Player->isDead();
    m_Player->update(delta, mouseHeld, m_ObjectManager->getObjects());
}

void PlayingState::setToPause(int &gameState)
{
    gameState = PAUSED;
    Mix_PauseMusic();
}

void PlayingState::setBackToPlay(int &gameState)
{
    gameState = PLAYING;
    Mix_ResumeMusic();
}

void PlayingState::resetMusic()
{
    Mix_HaltMusic();
}

void PlayingState::attemptResetTimer()
{
    if (m_TimerFinished)
    {
        return;
    }

    m_Timer = 60;
}

void PlayingState::render()
{
    m_ObjectManager->render();
    m_Player->render();
}

int PlayingState::getPlayerGamemode()
{
    return m_Player->getGamemode();
}
