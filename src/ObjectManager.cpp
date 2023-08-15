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

std::vector<GameObject> ObjectManager::getObjects()
{
    return objects;
}

void ObjectManager::clearObjects()
{
    objects.clear();
}

SDL_FRect ObjectManager::rotateHitbox(SDL_FRect hitbox, int rotations)
{
    if (rotations == 0)
    {
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

void ObjectManager::loadLevelData()
{
    std::ifstream in;
    in.open("res/leveldata/" + std::to_string(levelSelected) + ".level");
    int lines;
    in >> lines;

    while (lines --> 0)
    {
        std::string objectName;
        SDL_FPoint objectPos;
        int horizontalRepeats, verticalRepeats, rotation;
        in >> objectName >> objectPos.x >> objectPos.y >> horizontalRepeats >> verticalRepeats >> rotation;
        int objectType = stringToType.at(objectName);
        SDL_FRect hitboxOffset = rotateHitbox(typeToHitbox.at(objectType), rotation);

        for (int h = 0; h < horizontalRepeats; h++)
        {
            for (int v = 0; v < verticalRepeats; v++)
            {
                SDL_FPoint pos = {objectPos.x + h*TILE_SIZE, objectPos.y + v*TILE_SIZE};
                SDL_FRect hitbox = {hitboxOffset.x + pos.x, hitboxOffset.y + pos.y, hitboxOffset.w, hitboxOffset.h};
                objects.push_back(GameObject(objectType, rotation, pos, hitbox, ("res/gfx/" + objectName + ".png").c_str()));
            }
        }
    }
    in.close();
}

void ObjectManager::render()
{
    for (GameObject &gameObject : objects)
    {
        gameObject.render();
    }
}
