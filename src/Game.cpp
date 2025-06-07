#include <SDL_image.h>
#include <iostream>
#include <cmath>
#include "Game.h"
#include "Background.h"
#include "Ground.h"
#include "TitleScreen.h"
#include "LevelSelect.h"
#include "PlayingState.h"
#include "Player.h"
#include "Text.h"

int Game::init() {
    // prevent scaling issues on windows
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");

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
    m_Width = static_cast<int>((static_cast<float>(m_ScreenWidth) / static_cast<float>(m_ScreenHeight)) * m_Height);
    SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height); // render a WIDTHx1296 screen and scale it for the actual window
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");   // anti-aliasing

    Text::init();
    m_FPSTexture = Text::createTexture(m_Renderer, "FPS: " + std::to_string(m_CurrentFPS));

    m_Background.init(this, 0, 0, 255);
    m_Background.setMoving(true);
    m_Ground.init(this, 0, 0, 255);

    TitleScreen::get()->init(this);
    LevelSelect::get()->init(this);
    PlayingState::get()->init(this);

    pushState(TitleScreen::get());

    m_MenuLoop = Mix_LoadMUS("res/sfx/menuLoop.ogg");
    if (m_MenuLoop == nullptr) {
        std::cerr << "Failed to load menuLoop.ogg! " << SDL_GetError() << '\n';
    }
    Mix_PlayMusic(m_MenuLoop, -1);

    return 0;
}

void Game::quit() {
    m_Background.destroy();
    m_Ground.destroy();
    TitleScreen::get()->destroy();
    LevelSelect::get()->destroy();
    PlayingState::get()->destroy();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_DestroyTexture(m_FPSTexture);
    Mix_FreeMusic(m_MenuLoop);
    Text::destroy();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::update(float deltaTime) {
    m_GameStates.top()->update(deltaTime);
    // exponentially interpolate to target
    m_CameraPosition.y += (m_TargetY - m_CameraPosition.y) * (1.0f - std::pow(0.85f, deltaTime));
}

void Game::render() {
    SDL_RenderClear(m_Renderer);
    m_Background.render();
    // PlayingState will render ground itself to fix layering issues
    if (m_GameStates.top()->getName() != "PlayingState") {
        m_Ground.render();
    }
    m_GameStates.top()->render();
    Text::renderTexture(m_Renderer, m_FPSTexture, 20, 20, false, false, 0.5f);
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
            case SDLK_UP:
                m_IsUpHeld = true;
                break;
            case SDLK_x:
                m_IsXHeld = true;
                break;
            case SDLK_z:
                m_IsZHeld = true;
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
            case SDLK_UP:
                m_IsUpHeld = false;
                break;
            case SDLK_x:
                m_IsXHeld = false;
                break;
            case SDLK_z:
                m_IsZHeld = false;
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

void Game::incrementFrames() {
    m_Frames++;
}

void Game::updateFPS() {
    m_CurrentFPS = m_Frames;
    SDL_DestroyTexture(m_FPSTexture);
    m_FPSTexture = Text::createTexture(m_Renderer, "FPS: " + std::to_string(m_CurrentFPS));
    m_Frames = 0;
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
