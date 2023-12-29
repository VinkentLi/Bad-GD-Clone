#include <algorithm>
#include "Player.hpp"

Player::Player() {
    m_player_texture = IMG_LoadTexture(renderer, "res/gfx/icon.png");
    m_ship_texture = IMG_LoadTexture(renderer, "res/gfx/ship.png");
    m_death_sound = Mix_LoadWAV("res/sfx/deathSound.ogg");
    m_pos = {-TILE_SIZE, HEIGHT - 300 - TILE_SIZE};
    m_previous_pos = m_pos;
    m_x_velocity = 17.31;
    m_y_velocity = 0;
    m_jump_strength = -37.2671;
    m_pad_strength = -52.61467;
    m_gravity = 2.874767;
    m_rotation_adder = 6.92308;
    m_ship_up_adder = 1.4373;
    m_ship_down_adder = 1.3798;
    m_rotation = 0;
    m_target_rotation = 0;
    m_hazard_hitbox = {m_pos.x, m_pos.y, TILE_SIZE, TILE_SIZE};
    m_solid_hitbox = {m_pos.x + TILE_SIZE / 3, m_pos.y + TILE_SIZE / 3, TILE_SIZE / 3, TILE_SIZE / 3};
    m_grounded = true;
    m_mouse_held = false;
    m_orb_buffered = false;
    m_dead = false;
    m_dead_timer = 0;
    m_gravity_multiplier = 1;
    m_gamemode = CUBE;
}

void Player::update(float delta, bool mouse_held, std::vector<GameObject> objects) {
    bool mouseClicked = (!m_mouse_held && mouse_held);
    bool mouseReleased = (m_mouse_held && !mouse_held);
    m_mouse_held = mouse_held;

    if (m_dead) {
        m_dead_timer -= delta;

        if (m_dead_timer < 0) {
            m_gamemode = CUBE;
            m_pressed_orbs.clear();
            m_dead = false;
            m_grounded = true;
            m_hazard_hitbox.x = -TILE_SIZE;
            m_hazard_hitbox.y = HEIGHT - 300 - TILE_SIZE;
            m_solid_hitbox.x = m_hazard_hitbox.x + TILE_SIZE / 3;
            m_solid_hitbox.y = m_hazard_hitbox.y + TILE_SIZE / 3;
            m_pos = {m_hazard_hitbox.x, m_hazard_hitbox.y};
            camera_pos = {0, 0};
            m_rotation = 0;
            m_target_rotation = 0;
            m_gravity_multiplier = 1;
        }
        return;
    }

    switch (m_gamemode) {
    case CUBE:
        if (mouseClicked) {
            m_orb_buffered = true;
        }
        else if (mouseReleased) {
            m_orb_buffered = false;
        }

        if (m_grounded && m_mouse_held) {
            m_y_velocity = m_jump_strength;
            m_grounded = false;
        }

        m_y_velocity += m_gravity * delta;
        m_rotation += m_gravity_multiplier == 1 ? m_rotation_adder * delta : -m_rotation_adder * delta;

        if (m_gravity_multiplier == 1) {
            if (!m_grounded && m_rotation > m_target_rotation) {
                m_target_rotation = ((int)m_rotation / 90) * 90 + 90;
                // std::cout << targetRotation << ' ';
            }
            else if (m_rotation > m_target_rotation) {
                m_rotation = m_target_rotation;
            }
        }
        else {
            if (!m_grounded && m_rotation < m_target_rotation) {
                m_target_rotation = ((int)m_rotation / 90) * 90 - 90;
                // std::cout << targetRotation << ' ';
            }
            else if (m_rotation < m_target_rotation) {
                m_rotation = m_target_rotation;
            }
        }

        if (m_y_velocity > TILE_SIZE / 2) {
            m_y_velocity = TILE_SIZE / 2;
        }
        break;
    case SHIP: {
        double ship_up = m_ship_up_adder;
        double ship_down = m_ship_down_adder;
        double less_help_up = 0.9;
        double less_help_down = 0.667;

        if (m_gravity_multiplier == -1) {
            double t1 = ship_up;
            ship_up = ship_down;
            ship_down = t1;
            double t2 = less_help_up;
            less_help_up = less_help_down;
            less_help_down = t2;
        }

        if (mouse_held) {
            if (m_y_velocity > -7) {
                m_y_velocity -= ship_up * delta;
            }
            else {
                m_y_velocity -= ship_up * less_help_up * delta;
            }
        }
        else {
            if (m_y_velocity < -7) {
                m_y_velocity += ship_down * delta;
            }
            else {
                m_y_velocity += ship_down * less_help_down * delta;
            }
        }

        if (m_y_velocity < -80.0 / 3.0) {
            m_y_velocity = -80.0f / 3.0f;
        }
        else if (m_y_velocity > 64.0 / 3.0) {
            m_y_velocity = 64.0 / 3.0;
        }
        break;
    }
    }

    m_y_velocity *= m_gravity_multiplier;

    m_hazard_hitbox.x += m_x_velocity * delta;
    m_hazard_hitbox.y += m_y_velocity * delta;
    m_solid_hitbox.x += m_x_velocity * delta;
    m_solid_hitbox.y += m_y_velocity * delta;

    m_y_velocity /= m_gravity_multiplier;

    handle_collisions(objects);

    if (m_gamemode == SHIP) {
        // i tried to make rotations accurate, but i couldn't get it to work

        if (m_y_velocity == 0.0) {
            if (m_rotation < 0) {
                m_rotation = std::clamp(m_rotation + m_rotation_adder, -360.0, 0.0);
            }
            else if (m_rotation > 0) {
                m_rotation = std::clamp(m_rotation - m_rotation_adder, 0.0, 360.0);
            }
        }
        else {
            m_rotation = m_y_velocity * 2.0;
            m_rotation /= std::clamp(5.0 / std::abs(m_y_velocity), 1.0, 3.0);
        }

        m_rotation *= m_gravity_multiplier;
    }

    if (m_pos.x - camera_pos.x > CAMERA_SCROLL) {
        camera_pos.x = m_pos.x - CAMERA_SCROLL;
    }

    switch (m_gamemode) {
    case CUBE:
        if (m_pos.y - camera_pos.y < CAMERA_UP_SCROLL) {
            camera_pos.y = m_pos.y - CAMERA_UP_SCROLL;
        }

        if (m_pos.y - camera_pos.y > CAMERA_DOWN_SCROLL) {
            camera_pos.y = m_pos.y - CAMERA_DOWN_SCROLL;
        }
        break;
    }

    if (m_pos.y < -7 * BACKGROUND_SIZE) {
        die();
    }

    if (m_grounded) {
        m_orb_buffered = false;
    }

    m_previous_pos = m_pos;
}

void Player::handle_collisions(std::vector<GameObject> objects) {   
    switch (m_gamemode) {
    case CUBE:    
        if (m_hazard_hitbox.y > HEIGHT - 300 - TILE_SIZE) {
            m_hazard_hitbox.y = HEIGHT - 300 - TILE_SIZE;
            m_solid_hitbox.y = m_hazard_hitbox.y + TILE_SIZE / 3;
            m_grounded = true;
            m_y_velocity = 0;
        }
        break;
    case SHIP:
        if (m_hazard_hitbox.y < m_bounds.first) {
            m_hazard_hitbox.y = m_bounds.first;
            m_solid_hitbox.y = m_hazard_hitbox.y + TILE_SIZE / 3;
            m_y_velocity = 0;
        }

        if (m_hazard_hitbox.y + m_hazard_hitbox.h > m_bounds.second) {
            m_hazard_hitbox.y = m_bounds.second - m_hazard_hitbox.h;
            m_solid_hitbox.y = m_hazard_hitbox.y + TILE_SIZE / 3;
            m_grounded = true;
            m_y_velocity = 0;
        }
        break;
    }

    for (GameObject &object : objects) {
        SDL_FRect intersect; // this is useless but im too lazy to remove it lmfao

        if (SDL_IntersectFRect(&m_hazard_hitbox, object.get_hitbox(), &intersect)) {
            switch (object.get_type()) {
            case HAZARD:
                die();
                break;
            case BLOCK:
                if (!SDL_HasIntersectionF(&m_solid_hitbox, object.get_hitbox())) {
                    if (m_gravity_multiplier == 1) {
                        if (m_y_velocity > 0 && m_solid_hitbox.y + m_solid_hitbox.h < object.get_hitbox()->y) {
                            m_hazard_hitbox.y = object.get_pos().y - TILE_SIZE;
                            m_y_velocity = 0;
                        }
                    }
                    else {
                        if (m_y_velocity > 0 && m_solid_hitbox.y > object.get_hitbox()->y + object.get_hitbox()->h) {
                            m_hazard_hitbox.y = object.get_pos().y + TILE_SIZE;
                            m_y_velocity = 0;
                        }
                    }

                    if (m_gamemode == SHIP) {
                        if (m_y_velocity < 0 && m_solid_hitbox.y > object.get_hitbox()->y + object.get_hitbox()->h) {
                            m_hazard_hitbox.y = object.get_pos().y + TILE_SIZE;
                            m_y_velocity = 0;
                        }
                    }
                    break;
                }
                die();
                break;
            case PAD:
                m_y_velocity = m_pad_strength;
                break;
            case ORB:
                if (m_orb_buffered && (m_pressed_orbs.empty() || std::find(m_pressed_orbs.begin(), m_pressed_orbs.end(), object.get_hitbox()) == m_pressed_orbs.end())) {
                    m_y_velocity = m_jump_strength;
                    m_pressed_orbs.push_back(object.get_hitbox());
                }
                break;
            case UPSIDE_DOWN_PORTAL:
                if (m_gravity_multiplier == -1) {
                    break;
                }

                m_y_velocity /= 2;
                m_gravity_multiplier = -1;

                if (m_y_velocity < 0) {
                    m_y_velocity *= -1;
                }
                break;
            case NORMAL_PORTAL:
                if (m_gravity_multiplier == 1) {
                    break;
                }

                m_y_velocity /= 2;
                m_gravity_multiplier = 1;

                if (m_y_velocity < 0) {
                    m_y_velocity *= -1;
                }
                break;
            case SHIP_PORTAL:
                m_gamemode = SHIP;
                m_rotation = 0;

                if (m_gamemode != SHIP) {
                    m_y_velocity /= 2;
                }

                camera_pos.y = std::clamp((float)((((int)(object.get_pos().y / TILE_SIZE)) - 3) * TILE_SIZE), -10000.0f, -260.0f);
                m_bounds.first = camera_pos.y + 40;
                m_bounds.second = m_bounds.first + 10 * TILE_SIZE;
                break;
            case CUBE_PORTAL:
                m_gamemode = CUBE;

                if (m_gamemode != CUBE) {
                    m_y_velocity /= 2;
                }
                break;
            }
        }
    }

    m_grounded = m_y_velocity == 0 && (!m_mouse_held || m_gamemode != SHIP);
    m_solid_hitbox.x = m_hazard_hitbox.x + TILE_SIZE / 3;
    m_solid_hitbox.y = m_hazard_hitbox.y + TILE_SIZE / 3;
    m_pos.x = m_hazard_hitbox.x;
    m_pos.y = m_hazard_hitbox.y;
}

void Player::render() {
    switch (m_gamemode) {
    case CUBE: {
        SDL_FRect dst = {m_pos.x - camera_pos.x, m_pos.y - camera_pos.y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopyExF(renderer, m_player_texture, NULL, &dst, m_rotation, NULL, SDL_FLIP_NONE);
        break;
    }
    case SHIP: {
        if (m_gravity_multiplier == 1) {
            SDL_FRect cube_dst = {m_pos.x - camera_pos.x + TILE_SIZE / 4, m_pos.y - camera_pos.y, TILE_SIZE * 3 / 5, TILE_SIZE * 3 / 5};
            SDL_FRect ship_dst = {m_pos.x - camera_pos.x - 14, m_pos.y - camera_pos.y + 22, 132, 78};
            SDL_FPoint cube_center = {ship_dst.x + ship_dst.w / 2 - cube_dst.x, ship_dst.y + ship_dst.h / 2 - cube_dst.y};
            SDL_RenderCopyExF(renderer, m_player_texture, NULL, &cube_dst, m_rotation, &cube_center, SDL_FLIP_NONE);
            SDL_RenderCopyExF(renderer, m_ship_texture, NULL, &ship_dst, m_rotation, NULL, SDL_FLIP_NONE);
        }
        else {
            SDL_FRect cube_dst = {m_pos.x - camera_pos.x + TILE_SIZE / 4, m_pos.y - camera_pos.y + TILE_SIZE * 2 / 5, TILE_SIZE * 3 / 5, TILE_SIZE * 3 / 5};
            SDL_FRect ship_dst = {m_pos.x - camera_pos.x - 14, m_pos.y - camera_pos.y, 132, 78};
            SDL_FPoint cube_center = {ship_dst.x + ship_dst.w / 2 - cube_dst.x, ship_dst.y + ship_dst.h / 2 - cube_dst.y};
            SDL_RenderCopyExF(renderer, m_player_texture, NULL, &cube_dst, m_rotation, &cube_center, SDL_FLIP_VERTICAL);
            SDL_RenderCopyExF(renderer, m_ship_texture, NULL, &ship_dst, m_rotation, NULL, SDL_FLIP_VERTICAL);
        }
        break;
    }
    }

    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 125);
    // SDL_FRect temp = hazardHitbox;
    // temp.x -= cameraPos.x;
    // SDL_RenderFillRectF(renderer, &temp);
    // SDL_SetRenderDrawColor(renderer, 0, 0, 255, 125);
    // SDL_FRect temp2 = m_solid_hitbox;
    // temp2.x -= cameraPos.x;
    // SDL_RenderFillRectF(renderer, &temp2);
}

void Player::die() {
    m_dead = true;
    m_dead_timer = 90;
    Mix_HaltMusic();
    Mix_PlayChannel(-1, m_death_sound, 0);
    m_y_velocity = 0;
}

bool Player::is_dead() {
    return m_dead;
}

int Player::get_gamemode() {
    return m_gamemode;
}

// SDL_FRect Player::getHazardHitbox()
// {
//     return m_hazard_hitbox;
// }

// SDL_FRect Player::getSolidHitbox()
// {
//     return m_solid_hitbox;
// }
