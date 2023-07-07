#include "Food.h"

#include <iostream>

/*
=================
CONSTRUCTORS / DESTRUCTORS
=================
*/

Food::Food() {
    this->food_surface = IMG_Load("resources/food.png");
    if (!this->food_surface) {
        std::cerr << "Error loading food image." << std::endl;
        return;
    }
    srand(time(NULL));
    this->cnt = 0;
    this->generate();
    this->score = 1;
}

Food::~Food() {}

/* ================= */

/*
=================
PUBLIC FUNCTIONS
=================
*/

int Food::getX() { return this->x; }

int Food::getY() { return this->y; }

int Food::getW() { return this->w; }

int Food::getH() { return this->h; }

int Food::getScore() { return this->score; }

void Food::generate() {
    cnt++;
    this->x = (rand() % WINDOW_WIDTH);
    this->y = (rand() % (WINDOW_HEIGHT - GAME_BOARD_Y)) + GAME_BOARD_Y;

    this->x = this->x - (this->x % SNAKE_SIZE);
    this->y = this->y - (this->y % SNAKE_SIZE);
    if (cnt % 5 != 0) {
        this->w = SNAKE_SIZE;
        this->h = SNAKE_SIZE;
        this->score = 1;
    } else {
        this->w = SNAKE_SIZE * 2;
        this->h = SNAKE_SIZE * 2;
        this->score = 5;
    }
}

void Food::render(SDL_Renderer *renderer) {
    if (this->food_surface == NULL) {
        SDL_SetRenderDrawColor(renderer, FOOD_COLOR);
        SDL_Rect rect = {this->x, this->y, this->w, this->h};
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_Texture *food_texture =
            SDL_CreateTextureFromSurface(renderer, this->food_surface);
        SDL_Rect rect = {this->x, this->y, this->w, this->h};
        SDL_RenderCopy(renderer, food_texture, NULL, &rect);
        SDL_DestroyTexture(food_texture);
    }
}