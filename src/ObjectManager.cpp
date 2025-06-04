#include "ObjectManager.h"
#include "Game.h"
#include "LevelSelect.h"
#include "PlayingState.h"
#include <SDL_image.h>
#include <fstream>
#include "Util.h"
#include <iostream>

void ObjectManager::init(Game *game) {
    m_Game = game;
    // i found hitbox values in the gd programming discord
    for (int i = 1; i <= 7; i++) {
        if (i == 5) continue;
        m_IDToObjectData[i].type = ObjectType::BLOCK;
        m_IDToObjectData[i].hitbox = { 0, 0, static_cast<float>(m_Game->TILE_SIZE), static_cast<float>(m_Game->TILE_SIZE) };
    }
    m_IDToObjectData[5].type = ObjectType::DECO;
    m_IDToObjectData[8] = {
        ObjectType::HAZARD,
        { 48, 36, 24, 48 },
    };
    m_IDToObjectData[9] = {
        ObjectType::HAZARD,
        { 42, 30.4, 36, 43.2 }
    };
    m_IDToObjectData[10] = {
        ObjectType::NORMAL_PORTAL,
        { 10, -4, 100, 300 }
    };
    m_IDToObjectData[11] = m_IDToObjectData[10];
    m_IDToObjectData[11].type = ObjectType::UPSIDE_DOWN_PORTAL;
    m_IDToObjectData[12] = {
        ObjectType::CUBE_PORTAL,
        { -4, -4, 136, 344 }
    };
    m_IDToObjectData[13] = m_IDToObjectData[12];
    m_IDToObjectData[13].type = ObjectType::SHIP_PORTAL;
    for (int i = 15; i <= 21; i++) {
        m_IDToObjectData[i].type = ObjectType::DECO;
    }
    m_IDToObjectData[29].type = ObjectType::BG_TRIGGER;
    m_IDToObjectData[30].type = ObjectType::G_TRIGGER;
    m_IDToObjectData[35] = {
        ObjectType::PAD,
        { -10, 0, 100, 16 },
    };
    m_IDToObjectData[36] = {
        ObjectType::ORB,
        { -12, -12, 144, 144 },
    };
    m_IDToObjectData[39] = {
        ObjectType::HAZARD,
        { 48, 15.8, 24, 22.4 },
    };
    m_IDToObjectData[40] = {
        ObjectType::BLOCK,
        { 0, 0, 120, 54 },
    };
    m_IDToObjectData[41].type = ObjectType::DECO;

    loadTextures();
}

void ObjectManager::reset() {
    m_FurthestX = 0;
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

std::vector<GameObject> &ObjectManager::getTriggers() {
    return m_Triggers;
}

void ObjectManager::clearObjects() {
    m_Objects.clear();
}

SDL_FRect ObjectManager::rotateHitbox(SDL_FRect hitbox, int rotations) {
    if (rotations == 0) {
        return hitbox;
    }
    while (rotations < 0) {
        rotations += 4;
    }
    SDL_FRect rotated = hitbox;
    SDL_FPoint center = {rotated.x + rotated.w/2, rotated.y + rotated.h/2};
    rotated.x -= center.x;
    rotated.y -= center.y;
    for (int i = 0; i < rotations; i++)
    {
        float newX = -rotated.y - rotated.h;
        float newY = rotated.x;
        rotated.x = newX;
        rotated.y = newY;
        std::swap(rotated.w, rotated.h);
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
            continue;
        }
        SDL_QueryTexture(data.texture, NULL, NULL, &data.width, &data.height);
    }
}

void ObjectManager::loadLevelData() {
    m_FurthestX = 0;
    std::ifstream in;
    in.open("res/leveldata/" + std::to_string(LevelSelect::get()->getLevelSelected()) + ".level");
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string levelData = buffer.str();
    in.close();
    std::vector<std::string> objectProperties = Util::splitString(levelData, ';');
    
    // read initial properties from before the level starts
    std::string initialProperties = objectProperties[0];
    std::vector<std::string> splitIntialProperties = Util::splitString(initialProperties, ',');
    for (std::size_t i = 0; i+1 < splitIntialProperties.size(); i += 2) {
        std::string property = splitIntialProperties[i];
        std::string propertyValue = splitIntialProperties[i+1];
        // i love gd cologne https://github.com/GDColon/GDBrowser/blob/master/misc/analysis/initialProperties.json
        if (property == "kS29") {
            std::vector<std::string> splitPropertyValue = Util::splitString(propertyValue, '_');
            uint8_t red = static_cast<uint8_t>(std::stoi(splitPropertyValue[1]));
            uint8_t blue = static_cast<uint8_t>(std::stoi(splitPropertyValue[3]));
            uint8_t green = static_cast<uint8_t>(std::stoi(splitPropertyValue[5]));
            SDL_Color color = { red, blue, green };
            PlayingState::get()->setInitialBackground(color);
        } else if (property == "kS30") {
            std::vector<std::string> splitPropertyValue = Util::splitString(propertyValue, '_');
            uint8_t red = static_cast<uint8_t>(std::stoi(splitPropertyValue[1]));
            uint8_t blue = static_cast<uint8_t>(std::stoi(splitPropertyValue[3]));
            uint8_t green = static_cast<uint8_t>(std::stoi(splitPropertyValue[5]));
            SDL_Color color = { red, blue, green };
            PlayingState::get()->setInitialGround(color);
        }
        // i'm not doing any more initial properties because i'm lazy as hell
    }

    objectProperties.erase(objectProperties.begin());

    for (std::string &properties : objectProperties) {
        std::vector<std::string> splitProperties = Util::splitString(properties, ',');
        int objectID = 1;
        SDL_FPoint objectPos = { 0, 0 };
        ObjectData data = m_IDToObjectData[objectID];
        bool flipX = false;
        bool flipY = false;
        int rotation = 0;
        SDL_Color color = { 0, 0, 0 };
        float duration = 0;
        // i'm just gonna assume id is always listed first cuz otherwise there's gonna be some issues
        // also i love gd cologne https://github.com/GDColon/GDBrowser/blob/master/misc/analysis/objectProperties.json
        for (std::size_t i = 0; i+1 < splitProperties.size(); i += 2) {
            int propertyID = std::stoi(splitProperties[i]);
            std::string propertyValue = splitProperties[i+1];
            switch (propertyID) {
            case 1: // ID
                objectID = std::stoi(propertyValue);
                // if the object actually exists
                if (m_IDToObjectData.count(objectID) > 0) {
                    data = m_IDToObjectData[objectID];
                } else {
                    continue;
                }
                break;
            case 2: // x
                objectPos.x = std::stof(propertyValue)*4 - data.width/2;
                break;
            case 3: // y
                objectPos.y = m_Game->getHeight() - 3*m_Game->TILE_SIZE - (std::stof(propertyValue)*4 + data.height/2) + data.offset.y;
                break;
            case 4: // flipX
                flipX = std::stoi(propertyValue) == 1;
                break;
            case 5: // flipY
                flipY = std::stoi(propertyValue) == 1;
                break;
            case 6: // rotation
                rotation = std::stoi(propertyValue) / 90;
                break;
            case 7: // red
                color.r = std::stoi(propertyValue);
                break;
            case 8: // green
                color.g = std::stoi(propertyValue);
                break;
            case 9: // blue
                color.b = std::stoi(propertyValue);
                break;
            case 10: // duration
                duration = std::stof(propertyValue);
                break;
            default:
                break;
            }
        }
        // if the object doesn't exist
        if (m_IDToObjectData.count(objectID) == 0) {
            continue;
        }
        SDL_FRect hitboxOffset = rotateHitbox(data.hitbox, rotation);
        if (flipX) {
            hitboxOffset.x = data.width - hitboxOffset.x - hitboxOffset.w;
        }
        if (flipY) {
            hitboxOffset.y = data.height - hitboxOffset.y - hitboxOffset.h;
        }
        SDL_FRect hitbox = {hitboxOffset.x + objectPos.x, hitboxOffset.y + objectPos.y, hitboxOffset.w, hitboxOffset.h};
        // separate triggers
        if (data.type == ObjectType::BG_TRIGGER || data.type == ObjectType::G_TRIGGER) {
            m_Triggers.emplace_back(m_Game, data.type, rotation, objectPos, hitbox, flipX, flipY, data.texture, color, duration);
        } else {
            m_Objects.emplace_back(m_Game, data.type, rotation, objectPos, hitbox, flipX, flipY, data.texture, color, duration);
        }
        m_FurthestX = std::max(m_FurthestX, static_cast<int>(objectPos.x + data.width));
    }
}

void ObjectManager::render() {
    for (GameObject &gameObject : m_Objects) {
        // don't render stuff you don't see
        if (gameObject.getPos().x + gameObject.getWidth() < m_Game->getCameraPosition().x ||
            gameObject.getPos().x > m_Game->getCameraPosition().x + m_Game->getWidth() ||
            gameObject.getPos().y + gameObject.getHeight() < m_Game->getCameraPosition().y ||
            gameObject.getPos().y > m_Game->getCameraPosition().y + m_Game->getHeight()) {

            continue;
        }
        gameObject.render();
    }
}
