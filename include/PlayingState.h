#pragma once
#include <vector>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Player.h"
#include "ObjectManager.h"
#include "GameState.h"
#include "Button.h"

class Game;

class PlayingState : public GameState {
public:
    void init(Game *game) override;
    void destroy() override;
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render() override;
    inline int getMinY() { return m_MinY; }
    inline bool isInPractice() { return m_IsInPractice; }
    inline static PlayingState *get() { return &m_PlayingState; } 

private:
    static PlayingState m_PlayingState;
    Player m_Player;
    ObjectManager m_ObjectManager;
    SDL_Texture *m_LevelEndBlockTexture;
    SDL_Texture *m_ResumeTexture;
    SDL_Texture *m_ExitTexture;
    SDL_Texture *m_EnterPracticeTexture;
    SDL_Texture *m_ExitPracticeTexture;
    Mix_Chunk *m_LevelCompleteSound;
    Mix_Music *m_PracticeMusic;
    static constexpr int MARGIN = 50;
    int m_MinY;
    Button m_ResumeButton;
    Button m_ExitButton;
    Button m_EnterPracticeButton;
    Button m_ExitPracticeButton;
    float m_Timer;
    bool m_IsTimerFinished;
    bool m_IsSongPlaying;
    bool m_IsPlayerDead;
    bool m_IsEscapeHeld;
    bool m_IsSpaceHeld;
    bool m_IsPaused;
    bool m_ShouldEndLevel;
    bool m_JustSetNewBest;
    int m_LevelEndBlocksX;
    float m_TimeEndingLevel;
    bool m_IsLevelComplete;
    int m_LevelPercent; // ex: 3692 = 36.92%
    bool m_IsInPractice;
    std::vector<Mix_Music *> m_Songs;

    void updatePause(bool isEscapeReleased);
    void updateEndLevel(float deltaTime);
    void updateLevelComplete(bool isEscapeReleased);
    void renderPause();
    void renderEndBlocks();
    void renderLevelCompleteMenu();
    void pause();
    void resume();
    void resetMusic();
    void attemptResetTimer();
};
