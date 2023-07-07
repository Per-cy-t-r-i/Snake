#include "IdleMenu.h"

#include <iostream>

#include "GameConfig.h"

using namespace std;

IdleMenu::IdleMenu(SDL_Renderer *renderer) {
    this->font = TTF_OpenFont("resources/joystix_monospace.otf", 24);

    this->renderer = renderer;
    this->state = MenuState::IDLE;
}

IdleMenu::~IdleMenu() { TTF_CloseFont(this->font); }

void IdleMenu::processInput(SDL_Event event) {
    switch (event.type) {
        case SDL_QUIT: {
            this->state = MenuState::EXITING;
            break;
        }
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_p: {
                    this->state = MenuState::PLAYING;
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}

void IdleMenu::render() {
    string text = "Snake";
    int text_width = text.length() * 24;

    SDL_Surface *surface = TTF_RenderText_Solid(this->font, text.c_str(), SDL_Color{TEXT_COLOR});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    SDL_Rect text_rect = {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50, text_width, 100};
    SDL_RenderCopy(this->renderer, texture, NULL, &text_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    text = "[p]lay";
    text_width = text.length() * 20;
    surface = TTF_RenderText_Solid(this->font, text.c_str(), SDL_Color{TEXT_COLOR});
    texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    text_rect = {WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 50, text_width, 80};
    SDL_RenderCopy(this->renderer, texture, NULL, &text_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

MenuState IdleMenu::getState() { return this->state; }

void IdleMenu::setState(MenuState state) { this->state = state; }