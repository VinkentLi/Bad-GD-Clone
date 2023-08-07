#include "PlayingState.hpp"

PlayingState::PlayingState()
{
    player = Player();
    timer = 60;
    timerFinished = false;
}

void PlayingState::update(float delta, bool mouseHeld)
{
    if (timer > 0)
    {
        timer--;
        return;
    }

    timerFinished = true;
    player.update(delta, mouseHeld);
}

void PlayingState::attemptResetTimer()
{
    if (timerFinished)
    {
        return;
    }

    timer = 60;
}

void PlayingState::render()
{
    player.render();
}
