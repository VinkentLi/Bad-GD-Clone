#pragma once
#include "Global.hpp"
#include "Player.hpp"

class PlayingState
{
private:
    Player player;
    float timer;
    bool timerFinished;
    
public:
    PlayingState();
    void update(float delta, bool mouseHeld);
    void attemptResetTimer();
    void render();
};
