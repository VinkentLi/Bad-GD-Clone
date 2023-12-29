#include "PlayingState.hpp"

PlayingState::PlayingState() {
    m_player = new Player();
    m_object_manager = new ObjectManager();
    m_timer = 60;
    m_timer_finished = false;
    m_song_playing = false;
    m_is_player_dead = false;
    m_songs.clear();

    for (int i = 0; i < LEVEL_COUNT; i++) {
        m_songs.push_back(Mix_LoadMUS(("res/sfx/" + std::to_string(i) + ".mp3").c_str()));
    }
}

PlayingState::~PlayingState() {
    delete m_player;
    delete m_object_manager;
}

void PlayingState::update(int &game_state, float delta, bool mouseHeld) {
    if (game_state == PAUSED) {
        return;
    }

    if (m_timer > 0) {
        m_timer--;
        return;
    }

    m_timer_finished = true;

    if (!m_song_playing || (m_is_player_dead && !m_player->is_dead())) {
        Mix_PlayMusic(m_songs[level_selected], 0);
        m_song_playing = true;
    }

    m_is_player_dead = m_player->is_dead();
    m_player->update(delta, mouseHeld, m_object_manager->get_objects());
}

void PlayingState::set_to_pause(int &game_state) {
    game_state = PAUSED;
    Mix_PauseMusic();
}

void PlayingState::set_back_to_play(int &game_state) {
    game_state = PLAYING;
    Mix_ResumeMusic();
}

void PlayingState::reset_music() {
    Mix_HaltMusic();
}

void PlayingState::attempt_reset_timer() {
    if (m_timer_finished) {
        return;
    }

    m_timer = 60;
}

void PlayingState::render() {
    m_object_manager->render();
    m_player->render();
}

int PlayingState::get_player_gamemode() {
    return m_player->get_gamemode();
}
