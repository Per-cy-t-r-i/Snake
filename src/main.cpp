#include "Game.h"

int main(){
    Game game;

    game.init();
    game.mainLoop();
    game.clean();

    return 0;
}