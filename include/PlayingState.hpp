#pragma once
#include "Player.hpp"
#include "GameStates.hpp"
#include "ObjectManager.hpp"

class PlayingState {
private:
    Player *m_player = nullptr;
    float m_timer;
    bool m_timer_finished, m_song_playing, m_is_player_dead;
    std::vector<Mix_Music *> m_songs;
    ObjectManager *m_object_manager = nullptr;
    
public:
    PlayingState();
    ~PlayingState();
    void update(int &game_state, float delta, bool mouseHeld);
    void set_to_pause(int &game_state);
    void set_back_to_play(int &game_state);
    void reset_music();
    void attempt_reset_timer();
    void render();
    int get_player_gamemode();
};
