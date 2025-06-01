#include "Game.h"

#ifdef __EMSCRIPTEN__
#define game emGame
#endif

Game game;

int main(int argc, char **argv) {
    if (game.init() != 0) {
        game.quit();
    }
    game.run();
    return 0;
}
