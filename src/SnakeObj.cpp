#include "SnakeObj.h"

#include <iostream>

#include "GameConfig.h"

using namespace std;

SDL_Surface *SnakeSegment::snake_head = IMG_Load("resources/head.png");
SDL_Surface *SnakeSegment::snake_body = IMG_Load("resources/body.png");
SDL_Surface *SnakeSegment::snake_corner = IMG_Load("resources/body_corner.png");
SDL_Surface *SnakeSegment::snake_tail = IMG_Load("resources/tail.png");

/* ================= SNAKE SEGMENT ================= */

/*
=================
CONSTRUCTORS / DESTRUCTORS
=================
*/

SnakeSegment::SnakeSegment(int x, int y, SnakeSegment *next,
                           SnakeSegment *prev) {
    
    if (snake_head == NULL || snake_body == NULL || snake_corner == NULL ||
        snake_tail == NULL) {
        cout << "Error loading snake image" << endl;
        exit(1);
    }

    this->x = x;
    this->y = y;
    this->next = next;
    this->prev = prev;
}

SnakeSegment::~SnakeSegment() {}

/* ================= */

/*
=================
PUBLIC FUNCTIONS
=================
*/

int SnakeSegment::getX() { return this->x; }

int SnakeSegment::getY() { return this->y; }

SnakeSegment *SnakeSegment::getNext() { return this->next; }

SnakeSegment *SnakeSegment::getPrev() { return this->prev; }

void SnakeSegment::setX(int x) { this->x = x; }

void SnakeSegment::setY(int y) { this->y = y; }

void SnakeSegment::setNext(SnakeSegment *next) { this->next = next; }

void SnakeSegment::setPrev(SnakeSegment *prev) { this->prev = prev; }

void SnakeSegment::render(SDL_Renderer *renderer, string currentDirection) {

    if (this->prev == NULL) {
        this->renderHead(renderer, currentDirection);
    } else if (this->next == NULL) {
        this->renderTail(renderer);
    } else {
        if (this->prev->getX() == this->next->getX() ||
            this->prev->getY() == this->next->getY()) {
            this->renderBody(renderer);
        } else {
            this->renderCorner(renderer);
        }
    }

    if (this->next != NULL) {
        this->next->render(renderer, currentDirection);
    }
}

/* ================= */

/*
=================
PRIVATE FUNCTIONS
=================
*/

void SnakeSegment::renderHead(SDL_Renderer *renderer, string currentDirection) {
    SDL_Rect rect;
    rect.x = this->x;
    rect.y = this->y;
    rect.w = SNAKE_SIZE;
    rect.h = SNAKE_SIZE;

    int angle = 0;
    if (currentDirection == "up") {
        angle = 0;
    } else if (currentDirection == "down") {
        angle = 180;
    } else if (currentDirection == "left") {
        angle = 270;
    } else if (currentDirection == "right") {
        angle = 90;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, snake_head);
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL,
                     SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);
}

void SnakeSegment::renderBody(SDL_Renderer *renderer) {
    SDL_Rect rect;
    rect.x = this->x;
    rect.y = this->y;
    rect.w = SNAKE_SIZE;
    rect.h = SNAKE_SIZE;

    int angle = 90;
    if (this->prev->getX() == this->getX()) {
        angle = 0;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, snake_body);
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL,
                     SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);
}

void SnakeSegment::renderCorner(SDL_Renderer *renderer) {
    SDL_Rect rect;
    rect.x = this->x;
    rect.y = this->y;
    rect.w = SNAKE_SIZE;
    rect.h = SNAKE_SIZE;

    int angle = 0;
    if (min(this->prev->getX(), this->next->getX()) == this->getX()) {
        if (min(this->prev->getY(), this->next->getY()) == this->getY()) {
            angle = 0;
        } else {
            angle = 270;
        }
    } else {
        if (min(this->prev->getY(), this->next->getY()) == this->getY()) {
            angle = 90;
        } else {
            angle = 180;
        }
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, snake_corner);
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL,
                     SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);
}

void SnakeSegment::renderTail(SDL_Renderer *renderer) {
    SDL_Rect rect;
    rect.x = this->x;
    rect.y = this->y;
    rect.w = SNAKE_SIZE;
    rect.h = SNAKE_SIZE;

    int angle = 0;
    if (this->prev->getX() == this->getX()) {
        if (this->prev->getY() > this->getY()) {
            angle = 90;
        } else {
            angle = 270;
        }
    } else {
        if (this->prev->getX() > this->getX()) {
            angle = 0;
        } else {
            angle = 180;
        }
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, snake_tail);
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL,
                     SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);
}

/* ================= END SNAKE SEGMENT ================= */

/* ================= SNAKE OBJECT ================= */

/*
=================
CONSTRUCTORS / DESTRUCTORS
=================
*/

SnakeObj::SnakeObj() {
    int center_x = WINDOW_WIDTH / 2;
    center_x = center_x - (center_x % SNAKE_SIZE);
    int center_y = WINDOW_HEIGHT / 2;
    center_y = center_y - (center_y % SNAKE_SIZE);

    this->head = new SnakeSegment(center_x, center_y, NULL, NULL);
    this->tail = this->head;

    this->direction["up"] = make_pair(0, -SNAKE_SIZE);
    this->direction["down"] = make_pair(0, SNAKE_SIZE);
    this->direction["left"] = make_pair(-SNAKE_SIZE, 0);
    this->direction["right"] = make_pair(SNAKE_SIZE, 0);
    this->direction["none"] = make_pair(0, 0);

    this->currentDirection = "none";
}

SnakeObj::~SnakeObj() {}

/* ================= */

/*
=================
PUBLIC FUNCTIONS
=================
*/

void SnakeObj::grow() {
    SnakeSegment *new_tail = new SnakeSegment(
        this->tail->getX(), this->tail->getY(), NULL, this->tail);
    this->tail->setNext(new_tail);
    this->tail = new_tail;
}

SnakeSegment *SnakeObj::getHead() { return this->head; }

void SnakeObj::handleInput(SDL_Event event) {
    switch (event.type) {
        // Check for wasd
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_w:
                    this->changeDirection("up");
                    break;
                case SDLK_s:
                    this->changeDirection("down");
                    break;
                case SDLK_a:
                    this->changeDirection("left");
                    break;
                case SDLK_d:
                    this->changeDirection("right");
                    break;
            }
            break;
    }
}

void SnakeObj::move() {
    int new_x =
        this->head->getX() + this->direction[this->currentDirection].first;
    int new_y =
        this->head->getY() + this->direction[this->currentDirection].second;
    SnakeSegment *new_head = new SnakeSegment(new_x, new_y, this->head, NULL);
    this->head->setPrev(new_head);
    this->head = new_head;
    this->tail = this->tail->getPrev();
    SnakeSegment *old_tail = this->tail->getNext();
    this->tail->setNext(NULL);
    delete old_tail;
}

bool SnakeObj::selfCollision() {
    SnakeSegment *current = this->head->getNext();
    while (current != NULL) {
        if (current->getX() == this->head->getX() &&
            current->getY() == this->head->getY()) {
            return true;
        }
        current = current->getNext();
    }
    return false;
}

void SnakeObj::render(SDL_Renderer *renderer) { this->head->render(renderer, currentDirection); }

/* ================= */

/*
=================
PRIVATE FUNCTIONS
=================
*/

void SnakeObj::changeDirection(string newDirection) {
    int x_change = this->direction[newDirection].first +
                   this->direction[this->currentDirection].first;
    int y_change = this->direction[newDirection].second +
                   this->direction[this->currentDirection].second;

    if (x_change != 0 || y_change != 0) {
        this->currentDirection = newDirection;
    }
}