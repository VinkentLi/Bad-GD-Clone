#include "Trigger.h"
#include "Game.h"
#include <iostream>

Trigger::Trigger(Game *game, ObjectType type, SDL_FPoint pos, SDL_Color color, float duration) 
    : Object(game, type, 0, pos, {}, false, false, nullptr), m_Color(color), m_Duration(duration) {
    
    if (m_Type != ObjectType::BG_TRIGGER && m_Type != ObjectType::G_TRIGGER) {
        std::cerr << "WTF! Trigger is not a trigger!\n";
    }
}

void Trigger::activate() {
    if (m_IsActivated) {
        return;
    }
    m_IsActivated = true;
    if (m_Type == ObjectType::BG_TRIGGER) {
        m_Game->getBackground().fade(m_Color.r, m_Color.g, m_Color.b, m_Duration);
    } else if (m_Type == ObjectType::G_TRIGGER) {
        m_Game->getGround().fade(m_Color.r, m_Color.g, m_Color.b, m_Duration);
    }
}

void Trigger::reset() {
    m_IsActivated = false;
}
