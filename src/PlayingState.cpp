#include "PlayingState.hpp"

PlayingState::PlayingState()
{
    player = Player();
    timer = 60;
    timerFinished = false;
    songPlaying = false;
    songs.clear();

    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        songs.push_back(Mix_LoadWAV(("res/sfx/" + std::to_string(i) + ".mp3").c_str()));
    }
}

void PlayingState::update(float delta, bool mouseHeld)
{
    if (timer > 0)
    {
        timer--;
        return;
    }

    timerFinished = true;

    if (!songPlaying)
    {
        Mix_PlayChannel(1, songs[levelSelected], 0);
        songPlaying = true;
    }

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
