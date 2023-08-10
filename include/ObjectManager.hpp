#pragma once
#include <map>
#include "GameObject.hpp"

class ObjectManager
{
private:
    std::vector<GameObject> objects;
    std::map<std::string, int> stringToType;
    std::map<int, SDL_FRect> typeToHitbox;

public:
    ObjectManager();
    std::vector<GameObject> getObjects();
    void clearObjects();
    void loadLevelData();
    void render();
};

