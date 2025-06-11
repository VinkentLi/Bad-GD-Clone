#pragma once
#include <SDL.h>
#include <string>

enum class StateType {
    TITLE_SCREEN,
    LEVEL_SELECT,
    PLAYING_STATE
};

class Game;

class GameState {
public:
    ~GameState() = default;
    virtual void init(Game *game);
    virtual void destroy() = 0;
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() const = 0;
    inline StateType getType() { return m_Type; } 

protected:
    GameState() {}
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    StateType m_Type;
};
