// structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <SDL2/SDL.h>

typedef struct Entity Entity;

typedef struct {
    void (*logic)(void);
    void (*draw) (void);
} Delegate;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window   *window;
    Delegate      delegate;
    int           keyboard[MAX_KEYBOARD_KEYS];
} App;

// lista ligada de entidades
struct Entity {
    float        x, y;
    int          width, height;
    int          health, reload;
    float        scale, dx, dy;
    SDL_Texture *texture;
    Entity      *next;
};

typedef struct {
    Entity  fighterHead, *fighterTail;
    Entity  bulletHead,  *bulletTail;
} Stage;

#endif // STRUCTS_H
