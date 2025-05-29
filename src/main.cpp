#include "Game.h"

int main(int argc, char **argv) {
    Game game;
    if (game.init() != 0) {
        game.quit();
    }
    game.run();
    return 0;
}
