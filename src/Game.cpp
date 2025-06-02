#include <SDL_image.h>
#include <iostream>

#include "Game.h"
#include "Background.h"
#include "Ground.h"
#include "TitleScreen.h"
#include "LevelSelect.h"
#include "PlayingState.h"
#include "Player.h"
#include "Text.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
void emMainLoop();
#endif

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
    m_Width = (static_cast<float>(m_ScreenWidth) / static_cast<float>(m_ScreenHeight)) * m_Height;
    SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height); // render a WIDTHx1080 screen and scale it for the actual window
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

void Game::run() {
    Mix_PlayMusic(m_MenuLoop, -1);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(emMainLoop, 0, 1);
#else
    m_CurrentTime = SDL_GetTicks64();
    while (true) {
        mainLoop();
    }
#endif
}

// TODO: Fix this lmfao
#ifdef __EMSCRIPTEN__
void emMainLoop() {
    if (!emGame.isGameRunning()) {
        emGame.quit();
        emscripten_cancel_main_loop();
    }
    emGame.m_NewTime = SDL_GetTicks64();
    emGame.m_Timer += emGame.m_NewTime - emGame.m_CurrentTime;
    constexpr float interval = 1000.0f / 60.0f;
    float deltaTime = (emGame.m_NewTime - emGame.m_CurrentTime) / interval;
    emGame.m_CurrentTime = emGame.m_NewTime;
    emGame.handleEvents();
    emGame.update(deltaTime);
    emGame.render();
    emGame.m_Frames++;
    if (emGame.m_Timer >= 1000) {
        emGame.m_CurrentFPS = emGame.m_Frames;
        emGame.m_Frames = 0;
        emGame.m_Timer -= 1000;
    }
}
#else
void Game::mainLoop() {
    if (!m_IsGameRunning) {
        quit();
        exit(0);
    }
    m_NewTime = SDL_GetTicks64();
    m_Timer += m_NewTime - m_CurrentTime;
    constexpr float interval = 1000.0f / 60.0f;
    float deltaTime = (m_NewTime - m_CurrentTime) / interval;
    m_CurrentTime = m_NewTime;
    handleEvents();
    update(deltaTime);
    render();
    m_Frames++;
    if (m_Timer >= 1000) {
        m_CurrentFPS = m_Frames;
        m_Frames = 0;
        m_Timer -= 1000;
        SDL_DestroyTexture(m_FPSTexture);
        m_FPSTexture = Text::createTexture(m_Renderer, "FPS: " + std::to_string(m_CurrentFPS));
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
