#include "Global.hpp"
#include "Background.hpp"
#include "GameStates.hpp"

Background::Background(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b)
{
    bgTexture = IMG_LoadTexture(renderer, "res/gfx/background.png");
    emptyBG = IMG_LoadTexture(renderer, "res/gfx/emptyBG.png");
    SDL_SetTextureColorMod(bgTexture, r, g, b);
    SDL_SetTextureColorMod(emptyBG, r, g, b);
    bgCount = WIDTH / BACKGROUND_SIZE + 2;
    pos = {0, 0};
    moving = false;
    fading = false;
    targetR = r;
    targetG = g;
    targetB = b;
}

void Background::fade(uint8_t r, uint8_t g, uint8_t b, float time)
{
    targetR = r;
    targetG = g;
    targetB = b;
    fadeTime = time * 60;
    fading = true;
}

SDL_FPoint Background::getPos()
{
    return pos;
}

void Background::resetPos()
{
    pos.x = 0;   
}

void Background::setMoving(bool value)
{
    moving = value;
}

void Background::update(float delta)
{
    if (moving)
    {
        pos.x -= 2 * delta;

        if (pos.x < -BACKGROUND_SIZE)
        {
            pos.x += BACKGROUND_SIZE;
        }
    }

    if (fading)
    {
        fadeTime -= delta;
        r += delta * (targetR - r) / fadeTime;
        g += delta * (targetG - g) / fadeTime;
        b += delta * (targetB - b) / fadeTime;

        if (fadeTime <= 0)
        {
            r = targetR;
            g = targetG;
            b = targetB;
            fadeTime = 0;
            fading = false;
        }

        SDL_SetTextureColorMod(bgTexture, r, g, b);
    }
}

void Background::render(int gameState)
{
    for (int i = 0; i < bgCount; i++)
    {
        SDL_FRect dst = {pos.x + i * BACKGROUND_SIZE, pos.y - (BACKGROUND_SIZE - HEIGHT) - cameraPos.y/10, BACKGROUND_SIZE, BACKGROUND_SIZE};

        if (gameState == LEVEL_SELECT)
        {
            dst.y += (BACKGROUND_SIZE - HEIGHT);
            SDL_RenderCopyExF(renderer, emptyBG, NULL, &dst, 0, NULL, SDL_FLIP_VERTICAL);
            continue;
        }


        if (SDL_RenderCopyF(renderer, bgTexture, NULL, &dst) != 0)
        {
            std::cerr << "Failed to render background! " << SDL_GetError() << std::endl;
        }
    }
}
