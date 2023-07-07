#ifndef IDLE_MENU_H
#define IDLE_MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum class MenuState { IDLE, PLAYING, EXITING };

class IdleMenu {
   private:
    MenuState state;
    SDL_Renderer *renderer;
    TTF_Font *font;

   public:
    IdleMenu(SDL_Renderer *renderer);
    ~IdleMenu();

    void processInput(SDL_Event event);
    void render();
    MenuState getState();

    void setState(MenuState state);
};

#endif