#include "Text.h"
#include "Game.h"

namespace Text {

TTF_Font *font = nullptr;
TTF_Font *fontOutline = nullptr;

void init() {
    font = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    fontOutline = TTF_OpenFont("res/fonts/pusab.ttf", 100);
    TTF_SetFontOutline(fontOutline, OUTLINE_SIZE);
}

void destroy() {
    TTF_CloseFont(font);
    TTF_CloseFont(fontOutline);
}

void renderText(SDL_Renderer *renderer, const std::string &text, int x, int y, bool xCentered, bool yCentered, float scale) {
    SDL_Surface *shadowSurface = TTF_RenderText_Blended(font, text.c_str(), {0, 0, 0, 100});
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255});
    SDL_Surface *outlineSurface = TTF_RenderText_Blended(fontOutline, text.c_str(), {0, 0, 0});
    SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);
    SDL_Rect rect = {OUTLINE_SIZE, OUTLINE_SIZE, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, NULL, outlineSurface, &rect);
    SDL_FreeSurface(textSurface);
    SDL_Texture *levelShadowTexture = SDL_CreateTextureFromSurface(renderer, shadowSurface);
    SDL_Texture *levelStringTexture = SDL_CreateTextureFromSurface(renderer, outlineSurface);
    SDL_Rect textRect = { x, y, outlineSurface->w, outlineSurface->h };
    textRect.w *= scale;
    textRect.h *= scale;
    if (xCentered) {
        textRect.x -= textRect.w/2;
    }
    if (yCentered) {
        textRect.y -= textRect.h/2;
    }
    SDL_Rect shadowRect = textRect;
    shadowRect.x += 5 * scale;
    shadowRect.y += 5 * scale;
    SDL_RenderCopy(renderer, levelShadowTexture, NULL, &shadowRect);
    SDL_RenderCopy(renderer, levelStringTexture, NULL, &textRect);
    SDL_FreeSurface(shadowSurface);
    SDL_DestroyTexture(levelShadowTexture);
    SDL_FreeSurface(outlineSurface);
    SDL_DestroyTexture(levelStringTexture);
}

}
