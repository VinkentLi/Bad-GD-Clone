#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *tileSheet;
extern Mix_Chunk *menuLoop;
extern SDL_FPoint cameraPos;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int WIDTH;
inline constexpr int HEIGHT = 1080;
inline constexpr int TILE_SIZE = 100;
inline constexpr int CAMERA_SCROLL = TILE_SIZE * 6;
inline constexpr int CAMERA_UP_SCROLL = HEIGHT / 4;
inline constexpr int CAMERA_DOWN_SCROLL = 3 * CAMERA_UP_SCROLL;
