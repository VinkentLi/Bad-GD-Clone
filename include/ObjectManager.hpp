#pragma once
#include <map>
#include "GameObject.hpp"

class ObjectManager {
private:
    std::vector<GameObject> m_objects;
    std::map<std::string, int> m_string_to_type;
    std::map<int, SDL_FRect> m_type_to_hitbox;

public:
    ObjectManager();
    std::vector<GameObject> get_objects();
    SDL_FRect rotate_hitbox(SDL_FRect hitbox, int rotations);
    void clear_objects();
    void load_level_data();
    void render();
};

