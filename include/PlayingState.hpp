#pragma once
#include "Player.hpp"
#include "GameStates.hpp"
#include "ObjectManager.hpp"

class PlayingState
{
private:
    Player *m_Player = nullptr;
    float m_Timer;
    bool m_TimerFinished, m_SongPlaying, m_PlayerIsDead;
    std::vector<Mix_Music *> m_Songs;
    ObjectManager *m_ObjectManager = nullptr;
    
public:
    PlayingState();
    ~PlayingState();
    void update(int &gameState, float delta, bool mouseHeld);
    void setToPause(int &gameState);
    void setBackToPlay(int &gameState);
    void resetMusic();
    void attemptResetTimer();
    void render();
    int getPlayerGamemode();
};
