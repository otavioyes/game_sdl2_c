// structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
}App;

typedef struct{
    int x, y;
    SDL_Texture *texture;
}Entity;

#endif
