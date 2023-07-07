#ifndef FOOD_H
#define FOOD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include "GameConfig.h"

class Food {
    private:
        SDL_Surface *food_surface;
        int cnt;
        int x;
        int y;
        int w;
        int h;
        int score;
    
    public:
        Food();
        ~Food();

        int getX();
        int getY();
        int getW();
        int getH();

        int getScore();

        void generate();

        void render(SDL_Renderer *renderer);
};

#endif