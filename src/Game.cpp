#include "Game.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "GameConfig.h"
#include "GameLogic.h"

/*
=================
CONSTRUCTORS / DESTRUCTORS
=================
*/

Game::Game() {
    this->isRunning = false;
    this->window = nullptr;
    this->renderer = nullptr;
}

Game::~Game() {}

/* ================= */

/*
=================
PUBLIC FUNCTIONS
=================
*/

void Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL_ttf." << std::endl;
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "Error initializing SDL_image." << std::endl;
        return;
    }

    this->window = SDL_CreateWindow("Snake game", SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                    WINDOW_HEIGHT, 0);

    if (!this->window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    this->renderer =
        SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

    if (!this->renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    this->isRunning = true;
    this->gameLogic = new GameLogic(this->renderer);
    this->idleMenu = new IdleMenu(this->renderer);
    return;
}

void Game::mainLoop() {
    while (this->isRunning &&
           this->idleMenu->getState() != MenuState::EXITING) {
        this->processInput();
        this->update();
        this->render();
    }
}

void Game::clean() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
    gameLogic->saveHighScore();
    delete this->gameLogic;
}

/* ================= */

/*
=================
PRIVATE FUNCTIONS
=================
*/

void Game::processInput() {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        this->isRunning = false;
    }

    if (this->idleMenu->getState() == MenuState::IDLE) {
        this->idleMenu->processInput(event);
        if (this->idleMenu->getState() == MenuState::PLAYING) {
            delete this->gameLogic;
            this->gameLogic = new GameLogic(this->renderer);
        }
        return;
    }

    bool gameCont = gameLogic->processInput(event);
    if (!gameCont) {
        this->idleMenu->setState(MenuState::IDLE);
    }
}

void Game::update() {
    if (this->idleMenu->getState() == MenuState::IDLE) return;
    if (!gameLogic->update()) {
        this->idleMenu->setState(MenuState::IDLE);
    }
}

void Game::render() {
    if (this->idleMenu->getState() == MenuState::IDLE) {
        SDL_SetRenderDrawColor(this->renderer, BG_COLOR);
        SDL_RenderClear(this->renderer);
        idleMenu->render();
        SDL_RenderPresent(this->renderer);
        return;
    }
    SDL_SetRenderDrawColor(this->renderer, BG_COLOR);
    SDL_RenderClear(this->renderer);
    gameLogic->render();
    SDL_RenderPresent(this->renderer);
}