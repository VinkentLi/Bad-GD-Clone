#include "GameState.hpp"
#include "Game.hpp"
#include <iostream>

void GameState::init(Game *game) {
    m_Game = game;
    m_Renderer = m_Game->getRenderer();
}
