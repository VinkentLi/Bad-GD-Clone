#pragma once
#include <vector>
#include <SDL_mixer.h>

class Player;
class Game;
class ObjectManager;

class PlayingState {
public:
    PlayingState(Game *game);
    ~PlayingState();
    void update(float deltaTime);
    void setToPause(int &gameState);
    void setBackToPlay(int &gameState);
    void resetMusic();
    void attemptResetTimer();
    void render();
    int getPlayerGamemode();

private:
    Game *m_Game;
    Player *m_Player = nullptr;
    float m_Timer;
    bool m_IsTimerFinished;
    bool m_IsSongPlaying;
    bool m_IsPlayerDead;
    std::vector<Mix_Music *> m_Songs;
    ObjectManager *m_ObjectManager = nullptr;
};
