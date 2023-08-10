#pragma once
#include "Player.hpp"
#include "GameStates.hpp"
#include "ObjectManager.hpp"

class PlayingState
{
private:
    Player *player = nullptr;
    float timer;
    bool timerFinished, songPlaying, playerIsDead;
    std::vector<Mix_Music *> songs;
    ObjectManager *objectManager = nullptr;
    
public:
    PlayingState();
    ~PlayingState();
    void update(int &gameState, float delta, bool mouseHeld);
    void setToPause(int &gameState);
    void setBackToPlay(int &gameState);
    void resetMusic();
    void attemptResetTimer();
    void render();
};
