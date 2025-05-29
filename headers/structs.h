// structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_BULLETS 100

#include <SDL2/SDL.h>

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    int up;
    int down;
    int left;
    int right;
    int fire;
}App;


typedef struct{
    int x, y;
    int width, height;
    int speed;
    float scale;
    int health;
    int dx, dy;
    SDL_Texture *texture;
}Entity;


#endif
