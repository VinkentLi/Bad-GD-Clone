#pragma once
#include <optional>
#include "Player.hpp"
#include "GameStates.hpp"
#include "ObjectManager.hpp"

class PlayingState
{
private:
    Player player;
    float timer;
    bool timerFinished, songPlaying;
    std::vector<Mix_Music *> songs;
    std::optional<ObjectManager> ObjectManager;
    
public:
    PlayingState();
    void update(int &gameState, float delta, bool mouseHeld);
    void setToPause(int &gameState);
    void setBackToPlay(int &gameState);
    void resetMusic();
    void attemptResetTimer();
    void render();
};
