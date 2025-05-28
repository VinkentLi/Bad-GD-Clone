#include "GameState.hpp"
#include "Game.hpp"

GameState::GameState(Game *game) : m_Game(game) {
    m_Renderer = m_Game->getRenderer();
}
