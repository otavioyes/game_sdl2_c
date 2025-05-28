// structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    int up;
    int down;
    int left;
    int right;
}App;


typedef struct{
    int x, y;
    int width, height;
    float scale;
    SDL_Texture *texture;
}Entity;


#endif
