#pragma once
#include <map>
#include <vector>
#include <string>
#include <SDL.h>
#include "GameObject.hpp"

class Game;

class ObjectManager {
public:
    ObjectManager(Game *game);
    std::vector<GameObject> getObjects();
    SDL_FRect rotateHitbox(SDL_FRect hitbox, int rotations);
    void clearObjects();
    void loadLevelData();
    void render();

private:
    Game *m_Game;
    std::vector<GameObject> m_Objects;
    std::map<std::string, int> m_StringToType;
    std::map<int, SDL_FRect> m_TypeToHitbox;
};

