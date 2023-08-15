#include "PlayingState.hpp"

PlayingState::PlayingState()
{
    player = new Player();
    objectManager = new ObjectManager();
    timer = 60;
    timerFinished = false;
    songPlaying = false;
    playerIsDead = false;
    songs.clear();

    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        songs.push_back(Mix_LoadMUS(("res/sfx/" + std::to_string(i) + ".mp3").c_str()));
    }
}

PlayingState::~PlayingState()
{
    delete player;
    delete objectManager;
}

void PlayingState::update(int &gameState, float delta, bool mouseHeld)
{
    if (gameState == PAUSED)
    {
        return;
    }

    if (timer > 0)
    {
        timer--;
        return;
    }

    timerFinished = true;

    if (!songPlaying || (playerIsDead && !player->isDead()))
    {
        Mix_PlayMusic(songs[levelSelected], 0);
        songPlaying = true;
    }

    playerIsDead = player->isDead();
    player->update(delta, mouseHeld, objectManager->getObjects());
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
    if (timerFinished)
    {
        return;
    }

    timer = 60;
}

void PlayingState::render()
{
    objectManager->render();
    player->render();
}

int PlayingState::getPlayerGamemode()
{
    return player->getGamemode();
}
