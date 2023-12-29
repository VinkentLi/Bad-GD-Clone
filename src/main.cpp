#include "Global.hpp"
#include "Background.hpp"
#include "Ground.hpp"
#include "TitleScreen.hpp"
#include "LevelSelect.hpp"
#include "PlayingState.hpp"

int init();
void update(float delta);
void render();
void handle_events();
void quit();

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *tile_sheet;
Mix_Music *menu_loop;
SDL_Point mouse_pos;
SDL_FPoint camera_pos;
TTF_Font *font;
TTF_Font *font_outline;
Background *bg = nullptr;             // pointer so i can make it null
Ground *ground = nullptr;             // i want it null so i can call the constructor when
TitleScreen *title_screen = nullptr;   // SDL is initiated
LevelSelect *level_select = nullptr;   // there's probably a better way of doing it
PlayingState *playing_state = nullptr; // but I'm an idiot

int WIDTH, SCREEN_WIDTH, SCREEN_HEIGHT, frames = 0, current_fps = 0, gameState = TITLE_SCREEN, level_selected = 0;

bool running = true, mouse_held = false;

int main(int argc, char **argv) {
    if (init() != 0) {
        quit();
    }

    Mix_PlayMusic(menu_loop, -1);

    float interval = 1000.0f / 60.0f;
    uint64_t current_time = SDL_GetTicks64();
    uint64_t new_time;
    int timer = 0;

    while (running) {
        new_time = SDL_GetTicks64();
        timer += new_time - current_time;
        float delta = (new_time - current_time) / interval;
        current_time = new_time;

        handle_events();
        update(delta);
        render();
        frames++;

        if (timer >= 1000) {
            current_fps = frames;
            frames = 0;
            timer -= 1000;
        }
    }
    quit();

    return 0;
}

void update(float delta) {
    switch (gameState) {
    case TITLE_SCREEN:
        ground->set_pos({ground->get_pos().x, HEIGHT - 300});
        bg->set_moving(true);
        bg->update(delta);
        ground->update();
        ground->move(-17.31f, delta);
        title_screen->update(gameState, &mouse_pos, mouse_held);
        break;
    case LEVEL_SELECT:
        ground->set_pos({0, HEIGHT - 200});
        ground->set_on_top(false);
        bg->set_moving(false);
        // ground->reset_pos();
        level_select->update(gameState, &mouse_pos, mouse_held);
        break;
    case PLAYING:
        if (level_select->get_need_to_recall_playing_state_constructor()) {
            delete playing_state;
            playing_state = new PlayingState();
        }

        ground->set_pos({ground->get_pos().x, HEIGHT - 300});
        bg->update(delta);
        playing_state->update(gameState, delta, mouse_held);

        if (camera_pos.x != 0) {
            bg->set_moving(true);
        }

        if (playing_state->get_player_gamemode() == SHIP) {
            ground->set_on_top(true);
        }
        else {
            ground->set_on_top(false);
        }
        break;
    case PAUSED:
        playing_state->update(gameState, delta, mouse_held);
        break;
    }
}

void render() {
    SDL_RenderClear(renderer);
    bg->render(gameState);
    ground->render();

    switch (gameState) {
    case TITLE_SCREEN:
        title_screen->render();
        break;
    case LEVEL_SELECT:
        level_select->render();
        break;
    case PLAYING:
        playing_state->render();
        break;
    case PAUSED:
        playing_state->render();
        break;
    }

    // render fps

    SDL_Surface *fpsSurface = TTF_RenderText_Blended(font, ("FPS: " + std::to_string(current_fps)).c_str(), {255, 255, 255});
    SDL_Texture *fpsTexture = SDL_CreateTextureFromSurface(renderer, fpsSurface);
    SDL_Rect fpsSRC = {0, 0, fpsSurface->w, fpsSurface->h};
    SDL_Rect fpsDST = {20, 20, fpsSurface->w, fpsSurface->h};
    SDL_RenderCopy(renderer, fpsTexture, &fpsSRC, &fpsDST);
    SDL_FreeSurface(fpsSurface);
    SDL_DestroyTexture(fpsTexture);

    SDL_RenderPresent(renderer);
}

void handle_events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                running = false;
                break;
            case SDLK_ESCAPE:
                switch (gameState) {
                case LEVEL_SELECT:
                    gameState = TITLE_SCREEN;
                    break;
                case PLAYING:
                    playing_state->set_to_pause(gameState);
                    break;
                case PAUSED:
                    gameState = LEVEL_SELECT;
                    playing_state->reset_music();
                    Mix_PlayMusic(menu_loop, -1);
                    camera_pos = {0, 0};
                    break;
                case TITLE_SCREEN:
                    running = false;
                    break;
                }
                break;
            case SDLK_SPACE:
                switch (gameState) {
                case TITLE_SCREEN:
                    gameState = LEVEL_SELECT;
                    break;
                case PAUSED:
                    playing_state->set_back_to_play(gameState);
                    break;
                }
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN: {
            int mouseState = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
            mouse_held = mouseState == 1;
            break;
        }
        case SDL_MOUSEBUTTONUP:
            mouse_held = false;
            break;
        }
    }
}

void quit() {
    TTF_CloseFont(font);
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int init() {
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

    SDL_Rect display_bounds;

    SDL_GetDisplayBounds(0, &display_bounds);

    SCREEN_WIDTH = display_bounds.w;
    SCREEN_HEIGHT = display_bounds.h;

    // SCREEN_WIDTH = 1280;
    // SCREEN_HEIGHT = 720;

    window = SDL_CreateWindow(
        "GDClone",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    WIDTH = ((float)(SCREEN_WIDTH) / (float)(SCREEN_HEIGHT)) * HEIGHT;
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT); // render a WIDTHx1080 screen and scale it for the actual window
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");   // anti-aliasing
    tile_sheet = IMG_LoadTexture(renderer, "res/gfx/tileSheet.png");
    font = TTF_OpenFont("res/fonts/pusab.ttf", 50);
    bg = new Background(0, 0, 255);
    bg->set_moving(true);
    ground = new Ground(0, 0, 255);
    title_screen = new TitleScreen();
    level_select = new LevelSelect();
    playing_state = new PlayingState();
    menu_loop = Mix_LoadMUS("res/sfx/menuLoop.wav");
    camera_pos = {0, 0};

    return 0;
}
