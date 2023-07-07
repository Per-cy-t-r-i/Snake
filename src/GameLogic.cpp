#include "GameLogic.h"

#include <fstream>
#include <iostream>

#include "GameConfig.h"

using namespace std;

/*
=================
CONSTRUCTORS / DESTRUCTORS
=================
*/

GameLogic::GameLogic(SDL_Renderer *renderer) {
    this->font = TTF_OpenFont("resources/joystix_monospace.otf", 24);

    this->game_renderer = renderer;

    this->snake = new SnakeObj();
    this->food = new Food();
    this->game_tick = 0;

    this->current_score = 0;
    this->high_score = 0;

    loadHighScore();
}

GameLogic::~GameLogic() {}

/* ================= */

/*
=================
PUBLIC FUNCTIONS
=================
*/

bool GameLogic::processInput(SDL_Event event) {
    switch (event.type) {
        case SDL_KEYDOWN: {
            this->snake->handleInput(event);
            return true;
        }
        default: {
            return true;
        }
    }
}

bool GameLogic::update() {
    game_tick %= SNAKE_SPEED_CAP - SNAKE_SPEED;

    if (game_tick == 0) {
        this->snake->move();
        if (this->snake->selfCollision()) {
            return false;
        }
        if (checkWallCollision()) {
            return false;
        }
        if (checkFoodCollision()) {
            eatFood();
        }
    }
    game_tick++;

    SDL_Delay(1000 / FPS);

    return true;
}

void GameLogic::render() {
    renderPanels();
    renderScore();
    this->food->render(this->game_renderer);
    this->snake->render(this->game_renderer);
}

/* ================= */

/*
=================
PRIVATE FUNCTIONS
=================
*/

void GameLogic::renderPanels() {
    SDL_SetRenderDrawColor(this->game_renderer, OUTLINE_COLOR);
    SDL_Rect rect;

    // Draw a rectangle outlining the window
    rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderDrawRect(this->game_renderer, &rect);

    // Draw a rectangle at the top left corner of the screen
    rect = {0, 0, SCORE_PANEL_SIZE_W, SCORE_PANEL_SIZE_H};
    SDL_RenderDrawRect(this->game_renderer, &rect);

    // Draw a rectangle at the top right corner of the screen
    rect = {SCORE_PANEL_SIZE_W, 0, WINDOW_WIDTH - SCORE_PANEL_SIZE_W,
            SCORE_PANEL_SIZE_H};
    SDL_RenderDrawRect(this->game_renderer, &rect);
}

void GameLogic::renderScore() {
    string text = "Score:" + to_string(this->current_score);
    int width = text.length() * 20;

    SDL_Color color = {TEXT_COLOR};
    SDL_Surface *surface =
        TTF_RenderText_Solid(this->font, text.c_str(), color);
    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(this->game_renderer, surface);

    SDL_Rect rect = {10, 10, width, 30};
    SDL_RenderCopy(this->game_renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    text = ("High Score:" + to_string(this->high_score));
    width = text.length() * 20;
    surface = TTF_RenderText_Solid(this->font, text.c_str(), color);
    texture = SDL_CreateTextureFromSurface(this->game_renderer, surface);
    rect = {10, 50, width, 30};
    SDL_RenderCopy(this->game_renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

bool GameLogic::checkWallCollision() {
    if (this->snake->getHead()->getX() < 0 ||
        this->snake->getHead()->getX() > WINDOW_WIDTH) {
        return true;
    }
    if (this->snake->getHead()->getY() < GAME_BOARD_Y ||
        this->snake->getHead()->getY() > WINDOW_HEIGHT) {
        return true;
    }
    return false;
}

bool GameLogic::checkFoodCollision() {
    int head_x = this->snake->getHead()->getX();
    int head_y = this->snake->getHead()->getY();
    int food_x = this->food->getX();
    int food_y = this->food->getY();
    int food_w = this->food->getW();
    int food_h = this->food->getH();

    if (head_x >= food_x && head_x <= food_x + food_w / 2 && head_y >= food_y &&
        head_y <= food_y + food_h / 2) {
        return true;
    }

    return false;
}

void GameLogic::eatFood() {
    this->snake->grow();
    this->current_score += food->getScore();
    this->food->generate();
    this->high_score = max(this->current_score, this->high_score);
}

void GameLogic::loadHighScore() {
    ifstream file("resources/high_score.txt");
    if (file.is_open()) {
        file >> this->high_score;
        file.close();
    } else {
        this->high_score = 0;
        // Create the file if it doesn't exist
        ofstream file("resources/high_score.txt");
        file << this->high_score;
    }
}

void GameLogic::saveHighScore() {
    ofstream file("resources/high_score.txt");
    if (file.is_open()) {
        file << this->high_score;
        file.close();
    }
}

/* ================= */