#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern Mix_Music *menu_loop;
extern SDL_FPoint camera_pos;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int WIDTH;
extern int level_selected;
inline constexpr int HEIGHT = 1080;
inline constexpr int TILE_SIZE = 100;
inline constexpr int CAMERA_SCROLL = TILE_SIZE * 6;
inline constexpr int CAMERA_UP_SCROLL = HEIGHT / 4;
inline constexpr int CAMERA_DOWN_SCROLL = HEIGHT - 400;
inline constexpr int LEVEL_COUNT = 2;
inline constexpr int BACKGROUND_SIZE = 2048;
