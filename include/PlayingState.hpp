#pragma once
#include "Global.hpp"
#include "Player.hpp"

class PlayingState
{
private:
    Player player;
    float timer;
    bool timerFinished, songPlaying;
    std::vector<Mix_Chunk *> songs;
    
public:
    PlayingState();
    void update(float delta, bool mouseHeld);
    void attemptResetTimer();
    void render();
};
