#pragma once
#include <SDL_ttf.h>
#include <string>

class Game;

namespace Text {

extern TTF_Font *font;
extern TTF_Font *fontOutline;
constexpr int OUTLINE_SIZE = 4;
void init();
void destroy();
void renderText(SDL_Renderer *renderer, const std::string &text, int x, int y, bool xCentered = false, bool yCentered = false, float scale = 1.0f);

}
