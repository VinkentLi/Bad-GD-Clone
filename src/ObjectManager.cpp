#include "ObjectManager.hpp"
#include "Game.hpp"
#include <SDL_image.h>
#include <fstream>
#include <iostream>

void ObjectManager::init(Game *game) {
    m_Game = game;
    m_StringToType = { {"BLOCK", BLOCK},
                       {"SPIKE", HAZARD},
                       {"ywORB", ORB},
                       {"ywPAD", PAD},
                       {"pSHIP", SHIP_PORTAL},
                       {"pCUBE", CUBE_PORTAL},
                       {"pUPSD", UPSIDE_DOWN_PORTAL},
                       {"pRGLR", NORMAL_PORTAL} };
    
    m_TypeToHitbox = { {BLOCK,              {0 , 0 , m_Game->TILE_SIZE, m_Game->TILE_SIZE  }},
                       {HAZARD,             {36, 31, 27               , 50                 }},
                       {ORB,                {0 , 0 , m_Game->TILE_SIZE, m_Game->TILE_SIZE  }},
                       {PAD,                {10, 90, 77               , 10                 }},
                       {SHIP_PORTAL,        {0 , 0 , m_Game->TILE_SIZE, m_Game->TILE_SIZE*3}},
                       {CUBE_PORTAL,        {0 , 0 , m_Game->TILE_SIZE, m_Game->TILE_SIZE*3}},
                       {UPSIDE_DOWN_PORTAL, {0 , 0 , m_Game->TILE_SIZE, m_Game->TILE_SIZE*3}},
                       {NORMAL_PORTAL,      {0 , 0 , m_Game->TILE_SIZE, m_Game->TILE_SIZE*3}} };

    loadTextures();
}

void ObjectManager::reset() {
    clearObjects();
    loadLevelData();
}

ObjectManager::~ObjectManager() {
    for (auto &[_, texture] : m_StringToTexture) {
        SDL_DestroyTexture(texture);
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
    for (auto &[blockName, _] : m_StringToType) {
        m_StringToTexture[blockName] = IMG_LoadTexture(m_Game->getRenderer(), ("res/gfx/" + blockName + ".png").c_str());
        if (m_StringToTexture[blockName] == nullptr) {
            std::cerr << "Failed to load " << blockName << "! " << SDL_GetError() << std::endl;
        }
    }
}

void ObjectManager::loadLevelData() {
    std::ifstream in;
    in.open("res/leveldata/" + std::to_string(m_Game->getLevelSelected()) + ".level");
    int lines;
    in >> lines;

    while (lines-- > 0) {
        std::string objectName;
        SDL_FPoint objectPos;
        int horizontalRepeats, verticalRepeats, rotation;
        in >> objectName >> objectPos.x >> objectPos.y >> horizontalRepeats >> verticalRepeats >> rotation;
        int objectType = m_StringToType.at(objectName);
        SDL_FRect hitboxOffset = rotateHitbox(m_TypeToHitbox.at(objectType), rotation);
        for (int h = 0; h < horizontalRepeats; h++) {
            for (int v = 0; v < verticalRepeats; v++) {
                SDL_FPoint pos = {objectPos.x + h*m_Game->TILE_SIZE, objectPos.y + v*m_Game->TILE_SIZE};
                SDL_FRect hitbox = {hitboxOffset.x + pos.x, hitboxOffset.y + pos.y, hitboxOffset.w, hitboxOffset.h};
                m_Objects.emplace_back(m_Game, objectType, rotation, pos, hitbox, m_StringToTexture[objectName]);
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
