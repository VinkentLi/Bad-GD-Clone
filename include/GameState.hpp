#pragma once
#include <SDL.h>
#include <string>

class Game;

class GameState {
public:
    ~GameState() = default;
    virtual void init(Game *game);
    virtual void destroy() = 0;
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    inline std::string getName() { return m_Name; } 

protected:
    GameState() {}
    Game *m_Game;
    SDL_Renderer *m_Renderer;
    std::string m_Name;
};
