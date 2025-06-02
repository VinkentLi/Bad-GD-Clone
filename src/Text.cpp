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

SDL_Texture *createTexture(SDL_Renderer *renderer, const std::string &text) {
    SDL_Surface *shadowSurface = TTF_RenderText_Blended(font, text.c_str(), {0, 0, 0, 100});
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255});
    SDL_Surface *outlineSurface = TTF_RenderText_Blended(fontOutline, text.c_str(), {0, 0, 0});
    SDL_Surface *finalSurface = SDL_CreateRGBSurfaceWithFormat(
        0, 
        outlineSurface->w + SHADOW_SIZE,
        outlineSurface->h + SHADOW_SIZE, 
        32, 
        SDL_PIXELFORMAT_RGBA32
    );
    SDL_SetSurfaceBlendMode(finalSurface, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceBlendMode(shadowSurface, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceBlendMode(outlineSurface, SDL_BLENDMODE_BLEND);
    SDL_Rect shadowRect = { 2*OUTLINE_SIZE+SHADOW_SIZE, 2*OUTLINE_SIZE+SHADOW_SIZE, shadowSurface->w, shadowSurface->h };
    SDL_Rect outlineRect = { 0, 0, outlineSurface->w, outlineSurface->h };
    SDL_Rect rect = { OUTLINE_SIZE, OUTLINE_SIZE, textSurface->w, textSurface->h };
    SDL_BlitSurface(shadowSurface, NULL, finalSurface, &shadowRect);
    SDL_BlitSurface(outlineSurface, NULL, finalSurface, &outlineRect);
    SDL_BlitSurface(textSurface, NULL, finalSurface, &rect);
    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(outlineSurface);
    SDL_FreeSurface(shadowSurface);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, finalSurface);
    SDL_FreeSurface(finalSurface);
    return texture;
}

void renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, bool xCentered, bool yCentered, float scale) {
    SDL_Rect textRect = { x, y, 0, 0 };
    SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.w *= scale;
    textRect.h *= scale;
    const int widthWithoutShadow = textRect.w - SHADOW_SIZE*scale;
    const int heightWithoutShadow = textRect.h - SHADOW_SIZE*scale;
    if (xCentered) {
        textRect.x -= widthWithoutShadow/2;
    }
    if (yCentered) {
        textRect.y -= heightWithoutShadow/2;
    }
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    
}

void renderText(SDL_Renderer *renderer, const std::string &text, int x, int y, bool xCentered, bool yCentered, float scale) {
    SDL_Texture *texture = createTexture(renderer, text);
    renderTexture(renderer, texture, x, y, xCentered, yCentered, scale);
    SDL_DestroyTexture(texture);
}

}
