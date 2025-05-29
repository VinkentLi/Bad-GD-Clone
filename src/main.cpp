#include <SDL_image.h>
#include <iostream>

#include "Game.h"
#include "Background.h"
#include "Ground.h"
#include "TitleScreen.h"
#include "LevelSelect.h"
#include "PlayingState.h"
#include "Player.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int main(int argc, char **argv) {
    Game game;
    if (game.init() != 0) {
        game.quit();
    }
    game.run();
    return 0;
}

int Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL VIDEO INIT HAS FAILED! " << SDL_GetError() << std::endl;
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cerr << "IMG PNG INIT HAS FAILED! " << SDL_GetError() << std::endl;
        return -1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF Init has failed! " << SDL_GetError() << std::endl;
        return -1;
    }
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cerr << "Mix Open Audio has failed! " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(0, &displayBounds);
    m_ScreenWidth = displayBounds.w;
    m_ScreenHeight = displayBounds.h;
    m_Window = SDL_CreateWindow(
        "GDClone",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        m_ScreenWidth,
        m_ScreenHeight,
#ifdef __EMSCRIPTEN__
        SDL_WINDOW_SHOWN
#else
        SDL_WINDOW_FULLSCREEN_DESKTOP
#endif
    );

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    m_Width = ((float)(m_ScreenWidth) / (float)(m_ScreenHeight)) * m_Height;
    SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height); // render a WIDTHx1080 screen and scale it for the actual window
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");   // anti-aliasing
    m_Font = TTF_OpenFont("res/fonts/pusab.ttf", 50);
    m_Background.init(this, 0, 0, 255);
    m_Background.setMoving(true);
    m_Ground.init(this, 0, 0, 255);

    TitleScreen::get()->init(this);
    LevelSelect::get()->init(this);
    PlayingState::get()->init(this);

    pushState(TitleScreen::get());

    m_MenuLoop = Mix_LoadMUS("res/sfx/menuLoop.wav");
    m_CameraPosition = {0, 0};

    return 0;
}

void Game::quit() {
    m_Background.destroy();
    m_Ground.destroy();
    TitleScreen::get()->destroy();
    LevelSelect::get()->destroy();
    PlayingState::get()->destroy();
    TTF_CloseFont(m_Font);
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    Mix_FreeMusic(m_MenuLoop);
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::run() {
    Mix_PlayMusic(m_MenuLoop, -1);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 60, 1);
#else
    float interval = 1000.0f / 60.0f;
    uint64_t currentTime = SDL_GetTicks64();
    uint64_t newTime;
    while (true) {
        newTime = SDL_GetTicks64();
        m_Timer += newTime - currentTime;
        float delta = (newTime - currentTime) / interval;
        currentTime = newTime;
        mainLoop(delta);
    }
#endif
}

#ifdef __EMSCRIPTEN__
void Game::mainLoop() {
    if (!m_GameRunning) {
        quit();
        emscripten_cancel_main_loop();
    }
    handleEvents();
    update(1);
    render();
    m_CurrentFPS = 60;
}
#else
void Game::mainLoop(float deltaTime) {
    if (!m_IsGameRunning) {
        quit();
        exit(0);
    }
    handleEvents();
    update(deltaTime);
    render();
    m_Frames++;
    if (m_Timer >= 1000) {
        m_CurrentFPS = m_Frames;
        m_Frames = 0;
        m_Timer -= 1000;
    }
}
#endif

void Game::update(float deltaTime) {
    m_GameStates.top()->update(deltaTime);
}

void Game::render() {
    SDL_RenderClear(m_Renderer);
    m_Background.render();
    m_Ground.render();
    m_GameStates.top()->render();

    // render fps
    SDL_Surface *fpsSurface = TTF_RenderText_Blended(m_Font, ("FPS: " + std::to_string(m_CurrentFPS)).c_str(), {255, 255, 255});
    SDL_Texture *fpsTexture = SDL_CreateTextureFromSurface(m_Renderer, fpsSurface);
    SDL_Rect fpsSRC = {0, 0, fpsSurface->w, fpsSurface->h};
    SDL_Rect fpsDST = {20, 20, fpsSurface->w, fpsSurface->h};
    SDL_RenderCopy(m_Renderer, fpsTexture, &fpsSRC, &fpsDST);
    SDL_FreeSurface(fpsSurface);
    SDL_DestroyTexture(fpsTexture);
    SDL_RenderPresent(m_Renderer);
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            m_IsGameRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                m_IsEscapeHeld = true;
                break;
            case SDLK_SPACE:
                m_IsSpaceHeld = true;
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                m_IsEscapeHeld = false;
                break;
            case SDLK_SPACE:
                m_IsSpaceHeld = false;
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN: {
            int mouseState = SDL_GetMouseState(&m_MousePosition.x, &m_MousePosition.y);
            m_IsMouseHeld = mouseState == 1;
            break;
        }
        case SDL_MOUSEBUTTONUP:
            m_IsMouseHeld = false;
            break;
        }
    }
}

void Game::pushState(GameState *state) {
    m_GameStates.push(state);
    state->enter();
}

void Game::popState() {
    if (m_GameStates.size() <= 1) {
        m_IsGameRunning = false;
        return;
    }
    m_GameStates.top()->exit();
    m_GameStates.pop();
}

void Game::changeState(GameState *state) {
    popState();
    pushState(state);
}
