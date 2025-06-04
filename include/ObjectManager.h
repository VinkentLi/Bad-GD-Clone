#pragma once
#include <map>
#include <vector>
#include <string>
#include <SDL.h>
#include "GameObject.h"

struct ObjectData {
    ObjectType type;
    SDL_FRect hitbox = { 0, 0, 0, 0 };
    SDL_FPoint offset = { 0, 0 };
    SDL_Texture *texture = nullptr;
    int width = 0;
    int height = 0;
};

class Game;

class ObjectManager {
public:
    ObjectManager() = default;
    ~ObjectManager();
    ObjectManager(const ObjectManager &) = delete;
    ObjectManager &operator=(const ObjectManager &) = delete;
    void init(Game *game);
    void reset();
    std::vector<GameObject> &getObjects();
    SDL_FRect rotateHitbox(SDL_FRect hitbox, int rotations);
    void clearObjects();
    void loadTextures();
    void loadLevelData();
    void render();
    inline int getFurthestX() { return m_FurthestX; }

private:
    Game *m_Game;
    std::vector<GameObject> m_Objects;
    std::map<int, ObjectData> m_IDToObjectData;
    int m_FurthestX = 0;
};

