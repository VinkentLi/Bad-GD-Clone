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
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int WIDTH;
inline constexpr int HEIGHT = 1080;
inline constexpr int TILE_SIZE = 122;
