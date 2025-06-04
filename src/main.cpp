#include "Game.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Game game;
uint64_t currentTime = 0;
uint64_t newTime = 0;
uint64_t timer = 0;

void mainLoop() {
    if (!game.isGameRunning()) {
        game.quit();
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#else
        exit(0);
#endif
    }
    newTime = SDL_GetTicks64();
    timer += newTime - currentTime;
    constexpr float interval = 1000.0f / 60.0f;
    float deltaTime = (newTime - currentTime) / interval;
    currentTime = newTime;
    game.handleEvents();
    game.update(deltaTime);
    game.render();
    game.incrementFrames();
    if (timer >= 1000) {
        game.updateFPS();
        timer -= 1000;
    }
}

int main(int argc, char **argv) {
    if (game.init() != 0) {
        game.quit();
    }
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (true) {
        mainLoop();
    }
#endif
    return 0;
}
