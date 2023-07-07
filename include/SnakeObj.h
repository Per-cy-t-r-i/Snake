#ifndef SNAKE_OBJ_H
#define SNAKE_OBJ_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>
#include <utility>

using namespace std;

class SnakeSegment {
    private:
        static SDL_Surface *snake_head;
        static SDL_Surface *snake_body;
        static SDL_Surface *snake_corner;
        static SDL_Surface *snake_tail;
        int x;
        int y;
        SnakeSegment *next;
        SnakeSegment *prev;

        void renderBody(SDL_Renderer *renderer);
        void renderHead(SDL_Renderer *renderer, string currentDirection);
        void renderCorner(SDL_Renderer *renderer);
        void renderTail(SDL_Renderer *renderer);
    public:
        SnakeSegment(int x, int y, SnakeSegment *next, SnakeSegment *prev);
        ~SnakeSegment();

        int getX();
        int getY();
        SnakeSegment *getNext();
        SnakeSegment *getPrev();

        void setX(int x);
        void setY(int y);
        void setNext(SnakeSegment *next);
        void setPrev(SnakeSegment *prev);

        void render(SDL_Renderer *renderer, string currentDirection);
};

class SnakeObj {
    private:
        map<string, pair<int, int> > direction;
        string currentDirection;
        SnakeSegment *head;
        SnakeSegment *tail;

        void changeDirection(string direction);
    
    public:
        SnakeObj();
        ~SnakeObj();

        void grow();
        SnakeSegment *getHead();

        void move();

        bool selfCollision();

        void handleInput(SDL_Event event);
        void render(SDL_Renderer *renderer);
};

#endif