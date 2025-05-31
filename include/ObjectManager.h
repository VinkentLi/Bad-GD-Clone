#pragma once
#include <map>
#include <vector>
#include <string>
#include <SDL.h>
#include "GameObject.h"

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
    std::map<std::string, ObjectType> m_StringToType;
    std::map<std::string, SDL_Texture *> m_StringToTexture;
    std::map<ObjectType, SDL_FRect> m_TypeToHitbox;
    int m_FurthestX;
};

