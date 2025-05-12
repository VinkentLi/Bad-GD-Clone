#pragma once
#include <map>
#include "GameObject.hpp"

class ObjectManager
{
private:
    std::vector<GameObject> m_Objects;
    std::map<std::string, int> m_StringToType;
    std::map<int, SDL_FRect> m_TypeToHitbox;

public:
    ObjectManager();
    std::vector<GameObject> getObjects();
    SDL_FRect rotateHitbox(SDL_FRect hitbox, int rotations);
    void clearObjects();
    void loadLevelData();
    void render();
};

