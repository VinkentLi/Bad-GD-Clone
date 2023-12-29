#include "ObjectManager.hpp"
#include <fstream>

ObjectManager::ObjectManager() {
    m_string_to_type = { {"BLOCK", BLOCK},
                         {"SPIKE", HAZARD},
                         {"ywORB", ORB},
                         {"ywPAD", PAD},
                         {"pSHIP", SHIP_PORTAL},
                         {"pCUBE", CUBE_PORTAL},
                         {"pUPSD", UPSIDE_DOWN_PORTAL},
                         {"pRGLR", NORMAL_PORTAL} };
    
    m_type_to_hitbox = { {BLOCK,              {0,  0,  TILE_SIZE, TILE_SIZE}},
                         {HAZARD,             {36, 31, 27, 50}},
                         {ORB,                {0,  0,  TILE_SIZE, TILE_SIZE}},
                         {PAD,                {10, 90, 77, 10}},
                         {SHIP_PORTAL,        {0,  0,  TILE_SIZE, TILE_SIZE*3}},
                         {CUBE_PORTAL,        {0,  0,  TILE_SIZE, TILE_SIZE*3}},
                         {UPSIDE_DOWN_PORTAL, {0,  0,  TILE_SIZE, TILE_SIZE*3}},
                         {NORMAL_PORTAL,      {0,  0,  TILE_SIZE, TILE_SIZE*3}} };

    load_level_data();
}

std::vector<GameObject> ObjectManager::get_objects() {
    return m_objects;
}

void ObjectManager::clear_objects() {
    m_objects.clear();
}

SDL_FRect ObjectManager::rotate_hitbox(SDL_FRect hitbox, int rotations) {
    if (rotations == 0) {
        return hitbox;
    }

    SDL_FRect rotated = hitbox;
    SDL_FPoint center = {rotated.x + rotated.w/2, rotated.y + rotated.h/2};
    rotated.x -= center.x;
    rotated.y -= center.y;

    for (int i = 0; i < rotations; i++) {
        // (x, y) -> (y - h, -x)

        float new_x = -rotated.y - rotated.h;
        float new_y = rotated.x;

        // std::cout << new_x + center.x << ", " << new_y + center.y << '\n';

        rotated.x = new_x;
        rotated.y = new_y;

        std::swap(rotated.w, rotated.h);
    }

    rotated.x += center.x;
    rotated.y += center.y;

    return rotated;
}

void ObjectManager::load_level_data() {
    std::ifstream fin;
    fin.open("res/leveldata/" + std::to_string(level_selected) + ".level");
    int lines;
    fin >> lines;

    while (lines --> 0) {
        std::string object_name;
        SDL_FPoint object_pos;
        int horizontal_repeats, vertical_repeats, rotation;
        fin >> object_name >> object_pos.x >> object_pos.y >> horizontal_repeats >> vertical_repeats >> rotation;
        int object_type = m_string_to_type.at(object_name);
        SDL_FRect hitbox_offset = rotate_hitbox(m_type_to_hitbox.at(object_type), rotation);

        for (int h = 0; h < horizontal_repeats; h++) {
            for (int v = 0; v < vertical_repeats; v++) {
                SDL_FPoint pos = {object_pos.x + h*TILE_SIZE, object_pos.y + v*TILE_SIZE};
                SDL_FRect hitbox = {hitbox_offset.x + pos.x, hitbox_offset.y + pos.y, hitbox_offset.w, hitbox_offset.h};
                m_objects.push_back(GameObject(object_type, rotation, pos, hitbox, ("res/gfx/" + object_name + ".png").c_str()));
            }
        }
    }
    fin.close();
}

void ObjectManager::render() {
    for (GameObject &game_object : m_objects) {
        game_object.render();
    }
}
