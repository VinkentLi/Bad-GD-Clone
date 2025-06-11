#pragma once
#include "Object.h"

class Game;

class Trigger : public Object {
public:
    Trigger(Game *game, ObjectType type, SDL_FPoint pos, SDL_Color color, float duration);
    void activate();
    void reset();

private:
    SDL_Color m_Color;
    float m_Duration;
    bool m_IsActivated = false;
};
