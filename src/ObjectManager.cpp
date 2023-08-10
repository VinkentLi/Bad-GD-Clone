#include "ObjectManager.hpp"
#include <fstream>

ObjectManager::ObjectManager()
{
    stringToType = { {"BLOCK", BLOCK},
                     {"SPIKE", HAZARD},
                     {"ywORB", ORB},
                     {"ywPAD", PAD},
                     {"pSHIP", SHIP_PORTAL},
                     {"pCUBE", CUBE_PORTAL},
                     {"pUPSD", UPSIDE_DOWN_PORTAL},
                     {"pRGLR", NORMAL_PORTAL} };
    
    typeToHitbox = { {BLOCK,              {0,  0,  TILE_SIZE, TILE_SIZE}},
                     {HAZARD,             {36, 31, 27, 50}},
                     {ORB,                {0,  0,  TILE_SIZE, TILE_SIZE}},
                     {PAD,                {10, 90, 77, 10}},
                     {SHIP_PORTAL,        {0,  0,  TILE_SIZE, TILE_SIZE*3}},
                     {CUBE_PORTAL,        {0,  0,  TILE_SIZE, TILE_SIZE*3}},
                     {UPSIDE_DOWN_PORTAL, {0,  0,  TILE_SIZE, TILE_SIZE*3}},
                     {NORMAL_PORTAL,      {0,  0,  TILE_SIZE, TILE_SIZE*3}} };

    loadLevelData();
}

void ObjectManager::clearObjects()
{
    objects.clear();
}

void ObjectManager::loadLevelData()
{
    std::ifstream in;
    in.open("res/leveldata/" + std::to_string(levelSelected) + ".level", std::ios::in);
    int lines;
    in >> lines;

    for ( ; lines > 0; lines--)
    {
        std::string objectName;
        SDL_FPoint objectPos;
        int horizontalRepeats, verticalRepeats;
        in >> objectName >> objectPos.x >> objectPos.y >> horizontalRepeats >> verticalRepeats;
        int objectType = stringToType.at(objectName);
        SDL_FRect hitboxOffset = typeToHitbox.at(objectType);

        for (int h = 0; h < horizontalRepeats; h++)
        {
            for (int v = 0; v < verticalRepeats; v++)
            {
                SDL_FPoint pos = {objectPos.x + h*TILE_SIZE, objectPos.y + v*TILE_SIZE};
                SDL_FRect hitbox = {hitboxOffset.x + pos.x, hitboxOffset.y + pos.y, hitboxOffset.w, hitboxOffset.h};
                objects.push_back(GameObject(objectType, pos, hitbox, ("res/gfx/" + objectName + ".png").c_str()));
            }
        }
    }

    std::cout << objects.size() << '\n';
}

void ObjectManager::render()
{
    for (GameObject &gameObject : objects)
    {
        gameObject.render();
    }
}
