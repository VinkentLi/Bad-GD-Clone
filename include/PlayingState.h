#pragma once
#include <vector>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Player.h"
#include "ObjectManager.h"
#include "GameState.h"

class Game;

class PlayingState : public GameState {
public:
    void init(Game *game) override;
    void destroy() override;
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render() override;
    inline static PlayingState *get() { return &m_PlayingState; } 

private:
    static PlayingState m_PlayingState;
    Player m_Player;
    ObjectManager m_ObjectManager;
    float m_Timer;
    bool m_IsTimerFinished;
    bool m_IsSongPlaying;
    bool m_IsPlayerDead;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;
    bool m_IsPaused;
    std::vector<Mix_Music *> m_Songs;

    void pause();
    void resume();
    void resetMusic();
    void attemptResetTimer();
    void renderPause();
};
