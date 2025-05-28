#pragma once
#include <vector>
#include <SDL_mixer.h>
#include "Player.hpp"
#include "ObjectManager.hpp"

class Game;

class PlayingState {
public:
    PlayingState(Game *game);
    ~PlayingState();
    PlayingState(const PlayingState &) = delete;
    PlayingState &operator=(const PlayingState &) = delete;
    void update(float deltaTime);
    void setToPause(int &gameState);
    void setBackToPlay(int &gameState);
    void resetMusic();
    void attemptResetTimer();
    void render();
    int getPlayerGamemode();

private:
    Game *m_Game;
    Player m_Player;
    ObjectManager m_ObjectManager;
    float m_Timer;
    bool m_IsTimerFinished;
    bool m_IsSongPlaying;
    bool m_IsPlayerDead;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;
    std::vector<Mix_Music *> m_Songs;
};
