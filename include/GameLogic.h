#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "SnakeObj.h"
#include "Food.h"

class GameLogic{
    private:
        TTF_Font *font;
        SDL_Renderer *game_renderer;

        SnakeObj *snake;
        Food* food;
        int game_tick;
        int current_score;
        int high_score;

        void renderPanels();
        void renderScore();

        bool checkWallCollision();
        bool checkFoodCollision();

        void eatFood();
        void loadHighScore();
    public:
        GameLogic(SDL_Renderer *renderer);
        ~GameLogic();

        bool processInput(SDL_Event event);
        bool update();
        void render();
        void saveHighScore();
};

#endif