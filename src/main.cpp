#include "Global.hpp"
#include "GameObject.hpp"
#include "Background.hpp"
#include "Ground.hpp"
#include "TitleScreen.hpp"
#include "LevelSelect.hpp"
#include "GameStates.hpp"
#include "PlayingState.hpp"

int init();
void update(float delta);
void render();
void handleEvents();
void quit();

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *tileSheet;
Mix_Chunk *menuLoop;
SDL_Point mousePos;
SDL_FPoint cameraPos;
TTF_Font *font;
TTF_Font *fontOutline;
Background bg;
Ground ground;
TitleScreen titleScreen;
LevelSelect levelSelect;
PlayingState playingState;

int WIDTH, SCREEN_WIDTH, SCREEN_HEIGHT, frames = 0, currentFPS = 0, gameState = TITLE_SCREEN;

bool gameRunning = true, mouseHeld = false;

int main(int argc, char **argv)
{
    if (init() != 0)
    {
        quit();
    }

    Mix_PlayChannel(0, menuLoop, -1);

    float interval = 1000.0f / 60.0f;
    uint64_t currentTime = SDL_GetTicks64();
    uint64_t newTime;
    int timer = 0;

    while (gameRunning)
    {
        newTime = SDL_GetTicks64();
        timer += newTime - currentTime;
        float delta = (newTime - currentTime) / interval;
        currentTime = newTime;

        handleEvents();
        update(delta);
        render();
        frames++;

        if (timer >= 1000)
        {
            currentFPS = frames;
            frames = 0;
            timer -= 1000;
        }
    }

    quit();

    return 0;
}

void update(float delta)
{
    switch (gameState)
    {
    case TITLE_SCREEN:
        ground.setPos({ground.getPos().x, HEIGHT - 300});
        bg.setMoving(true);
        bg.update(delta);
        ground.update();
        ground.move(-21.1182f, delta);
        titleScreen.update(gameState, &mousePos, mouseHeld);
        break;
    case LEVEL_SELECT:
        ground.setPos({0, HEIGHT - 200});
        bg.setMoving(false);
        // ground.resetPos();
        levelSelect.update(gameState, &mousePos, mouseHeld);
        break;
    case PLAYING:
        ground.setPos({ground.getPos().x, HEIGHT - 300});
        playingState.update(delta, mouseHeld);
        break;
    }
}

void render()
{
    SDL_RenderClear(renderer);
    bg.render(gameState);
    ground.render();

    switch (gameState)
    {
    case TITLE_SCREEN:
        titleScreen.render();
        break;
    case LEVEL_SELECT:
        levelSelect.render();
        break;
    case PLAYING:
        playingState.render();
        break;
    }

    // render fps

    SDL_Surface *fpsSurface = TTF_RenderText_Blended(font, ("FPS: " + std::to_string(currentFPS)).c_str(), {255, 255, 255});
    SDL_Texture *fpsTexture = SDL_CreateTextureFromSurface(renderer, fpsSurface);
    SDL_Rect fpsSRC = {0, 0, fpsSurface->w, fpsSurface->h};
    SDL_Rect fpsDST = {20, 20, fpsSurface->w, fpsSurface->h};
    SDL_RenderCopy(renderer, fpsTexture, &fpsSRC, &fpsDST);
    SDL_FreeSurface(fpsSurface);
    SDL_DestroyTexture(fpsTexture);

    SDL_RenderPresent(renderer);
}

void handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            gameRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                gameRunning = false;
                break;
            case SDLK_ESCAPE:
                if (gameState == LEVEL_SELECT)
                {
                    gameState = TITLE_SCREEN;
                }
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            {
            int mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);
            mouseHeld = mouseState == 1;
            break;
            }
        case SDL_MOUSEBUTTONUP:
            mouseHeld = false;
            break;
        }
    }
}

void quit()
{
    TTF_CloseFont(font);
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL VIDEO INIT HAS FAILED! " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG)))
    {
        std::cerr << "IMG PNG INIT HAS FAILED! " << SDL_GetError() << std::endl;
        return -1;
    }

    if (TTF_Init() != 0)
    {
        std::cerr << "TTF Init has failed! " << SDL_GetError() << std::endl;
        return -1;
    }

    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
    {
        std::cerr << "Mix Open Audio has failed! " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Rect displayBounds;

    SDL_GetDisplayBounds(0, &displayBounds);

    // const int SCREEN_WIDTH = displayBounds.w;
    // const int SCREEN_HEIGHT = displayBounds.h;

    SCREEN_WIDTH = 1280;
    SCREEN_HEIGHT = 720;

    window = SDL_CreateWindow("GDClone", 
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 
                                          SCREEN_WIDTH, 
                                          SCREEN_HEIGHT, 
                                          SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    WIDTH = ((float) (SCREEN_WIDTH) / (float) (SCREEN_HEIGHT)) * HEIGHT;
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    tileSheet = IMG_LoadTexture(renderer, "res/gfx/tileSheet.png");
    font = TTF_OpenFont("res/fonts/pusab.ttf", 50);
    bg = Background(0, 0, 255);
    bg.setMoving(true);
    ground = Ground(0, 0, 255);
    titleScreen = TitleScreen();
    levelSelect = LevelSelect();
    playingState = PlayingState();
    menuLoop = Mix_LoadWAV("res/sfx/menuLoop.wav");
    cameraPos = {0, 0};

    return 0;
}
