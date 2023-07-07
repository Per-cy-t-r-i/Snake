#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "GameLogic.h"

#include "IdleMenu.h"

class Game {
   private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    IdleMenu *idleMenu;
    GameLogic *gameLogic;

    void processInput();
    void update();
    void render();

   public:
    Game();
    ~Game();

    void init();
    void mainLoop();
    void clean();
};

#endif