#include "ObjectManager.h"
#include "Game.h"
#include "LevelSelect.h"
#include <SDL_image.h>
#include <fstream>
#include <iostream>

void ObjectManager::init(Game *game) {
    m_Game = game;
    SDL_Renderer *renderer = m_Game->getRenderer();
    for (int i = 1; i <= 7; i++) {
        m_IDToObjectData[i].type = ObjectType::BLOCK;
        m_IDToObjectData[i].hitbox = { 0, 0, static_cast<float>(m_Game->TILE_SIZE), static_cast<float>(m_Game->TILE_SIZE) };
        m_IDToObjectData[i].offset = {0, 0};
    }
    m_IDToObjectData[8] = {
        ObjectType::HAZARD,
        { 48, 24, 24, 48 },
        { 0, 0 },
        nullptr
    };
    m_IDToObjectData[9] = {
        ObjectType::HAZARD,
        { 42, 30.4, 36, 43.2 },
        { 0, m_Game->TILE_SIZE/2 },
        nullptr
    };
    m_IDToObjectData[10] = {
        ObjectType::NORMAL_PORTAL,
        { 10, -4, 100, 300 },
        { 37, 34 },
        nullptr
    };
    m_IDToObjectData[11] = m_IDToObjectData[10];
    m_IDToObjectData[11].type = ObjectType::UPSIDE_DOWN_PORTAL;
    m_IDToObjectData[12] = {
        ObjectType::CUBE_PORTAL,
        { -4, -4, 136, 344 },
        { -6, 12 },
        nullptr
    };
    m_IDToObjectData[13] = m_IDToObjectData[12];
    m_IDToObjectData[13].type = ObjectType::SHIP_PORTAL;
    m_IDToObjectData[15] = {
        ObjectType::DECO,
        { 0, 0, 0, 0 },
        { 48, 72 },
        nullptr
    };
    m_IDToObjectData[16] = m_IDToObjectData[15];
    m_IDToObjectData[16].offset.y = 16;
    m_IDToObjectData[17] = m_IDToObjectData[15];
    m_IDToObjectData[17].offset.x = 50;
    m_IDToObjectData[18] = m_IDToObjectData[15];
    m_IDToObjectData[18].offset = { -194, -40 };
    m_IDToObjectData[19] = m_IDToObjectData[15];
    m_IDToObjectData[19].offset = { -148, -27 };
    m_IDToObjectData[20] = m_IDToObjectData[15];
    m_IDToObjectData[20].offset = { -84, 12 };
    m_IDToObjectData[21] = m_IDToObjectData[15];
    m_IDToObjectData[21].offset = { -24, 68 };
    m_IDToObjectData[29].type = ObjectType::BG_TRIGGER;
    m_IDToObjectData[30].type = ObjectType::G_TRIGGER;
    m_IDToObjectData[35] = {
        ObjectType::PAD,
        { 10, 104, 100, 16 },
        { 10, 104 },
        nullptr
    };
    m_IDToObjectData[36] = {
        ObjectType::ORB,
        { -12, -12, 144, 144 },
        { 0, 0 },
        nullptr
    };
    m_IDToObjectData[39] = {
        ObjectType::HAZARD,
        { 48, 15.8, 24, 22.4 },
        { 0, 69 },
        nullptr
    };
    m_IDToObjectData[40] = {
        ObjectType::BLOCK,
        { 0, 0, 120, 54 },
        { 0, 0 },
        nullptr
    };
    m_IDToObjectData[41] = m_IDToObjectData[15];
    m_IDToObjectData[41].offset = { 21, 82 };

    loadTextures();
}

void ObjectManager::reset() {
    clearObjects();
    loadLevelData();
}

ObjectManager::~ObjectManager() {
    for (auto &[_, data] : m_IDToObjectData) {
        SDL_DestroyTexture(data.texture);
    }
}

std::vector<GameObject> &ObjectManager::getObjects() {
    return m_Objects;
}

void ObjectManager::clearObjects() {
    m_Objects.clear();
}

SDL_FRect ObjectManager::rotateHitbox(SDL_FRect hitbox, int rotations) {
    if (rotations == 0) {
        return hitbox;
    }
    SDL_FRect rotated = hitbox;
    SDL_FPoint center = {rotated.x + rotated.w/2, rotated.y + rotated.h/2};
    rotated.x -= center.x;
    rotated.y -= center.y;
    for (int i = 0; i < rotations; i++)
    {
        // (x, y) -> (y - h, -x)
        float newX = -rotated.y - rotated.h;
        float newY = rotated.x;
        // std::cout << newX + center.x << ", " << newY + center.y << '\n';
        rotated.x = newX;
        rotated.y = newY;
        // swap w & h
        float temp = rotated.h;
        rotated.h = rotated.w;
        rotated.w = temp;
    }
    rotated.x += center.x;
    rotated.y += center.y;
    return rotated;
}

void ObjectManager::loadTextures() {
    for (auto &[id, data] : m_IDToObjectData) {
        if (data.type == ObjectType::BG_TRIGGER || data.type == ObjectType::G_TRIGGER) {
            continue;
        }
        data.texture = IMG_LoadTexture(m_Game->getRenderer(), ("res/gfx/objects/" + std::to_string(id) + ".png").c_str());
        if (data.texture == nullptr) {
            std::cerr << "Failed to load " << id << ".png! " << SDL_GetError() << '\n';
        }
    }
}

void ObjectManager::loadLevelData() {
    m_FurthestX = 0;
    std::ifstream in;
    in.open("res/leveldata/" + std::to_string(LevelSelect::get()->getLevelSelected()) + ".level");
    int lines;
    in >> lines;

    while (lines-- > 0) {
        std::string objectName;
        SDL_FPoint objectPos;
        int horizontalRepeats, verticalRepeats, rotation;
        in >> objectName >> objectPos.x >> objectPos.y >> horizontalRepeats >> verticalRepeats >> rotation;
        objectPos.x += 15;
        objectPos.y += 15;
        objectPos.x *= 4;
        objectPos.y *= 4;
        objectPos.y = m_Game->getHeight() - 3*m_Game->TILE_SIZE - objectPos.y;
        ObjectType objectType = m_StringToType.at(objectName);
        SDL_FRect hitboxOffset = rotateHitbox(m_TypeToHitbox.at(objectType), rotation);
        for (int h = 0; h < horizontalRepeats; h++) {
            for (int v = 0; v < verticalRepeats; v++) {
                SDL_FPoint pos = {objectPos.x + h*m_Game->TILE_SIZE, objectPos.y + v*m_Game->TILE_SIZE};
                SDL_FRect hitbox = {hitboxOffset.x + pos.x, hitboxOffset.y + pos.y, hitboxOffset.w, hitboxOffset.h};
                m_Objects.emplace_back(m_Game, objectType, rotation, pos, hitbox, m_StringToTexture[objectName]);
                m_FurthestX = std::max(m_FurthestX, static_cast<int>(m_Objects.back().getPos().x + m_Objects.back().getWidth()));
            }
        }
    }
    in.close();
}

void ObjectManager::render() {
    for (GameObject &gameObject : m_Objects) {
        gameObject.render();
    }
}
