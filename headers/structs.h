// structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_BULLETS 100

#include <SDL2/SDL.h>

typedef struct{
    void (*logic)(void);
    void (*draw)(void);
}Delegate;


typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
}App


typedef struct{
    float x, y;
    int width, height;
    int health;
    int reload;
    float scale;
    float dx, dy;
    SDL_Texture *texture;
    Entity *next;
};

//usado para armazenar informacoes sobre lutas e balas
typedef struct{
    Entity fighterHead, *fighterTail;
    Entity bulletHead, *bulletTail;
}Stage;


#endif

