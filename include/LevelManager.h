#pragma once
#include <map>
#include <vector>
#include <string>
#include <SDL.h>
#include "Trigger.h"

struct ObjectData {
    ObjectType type;
    std::optional<SDL_FRect> hitbox = { };
    SDL_FPoint offset = { 0, 0 };
    SDL_Texture *texture = nullptr;
    int width = 0;
    int height = 0;
};

class Game;

class LevelManager {
public:
    LevelManager() = default;
    ~LevelManager();
    LevelManager(const LevelManager &) = delete;
    LevelManager &operator=(const LevelManager &) = delete;
    void init(Game *game);
    void reset();
    std::vector<GameObject> &getObjects();
    std::vector<Trigger> &getTriggers();
    SDL_Color getInitialBackground();
    SDL_Color getInitialGround();
    SDL_FRect rotateHitbox(SDL_FRect hitbox, int rotations);
    void render();
    inline int getFurthestX() { return m_FurthestX; }

private:
    Game *m_Game;
    std::vector<GameObject> m_Objects;
    std::vector<Trigger> m_Triggers;
    std::map<int, ObjectData> m_IDToObjectData;
    SDL_Color m_InitialBackgroundColor;
    SDL_Color m_InitialGroundColor;
    int m_FurthestX = 0;

    void initBlocks();
    void clearObjects();
    void loadTextures();
    void loadLevelData();
    void setInitialProperties(const std::string &initialProperties);
    void addObject(const std::string &properties);
};

