// structs.h
#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include <SDL2/SDL.h>

typedef struct Entity Entity;

//encapsula funcoes de logicas de desenho
typedef struct {
    void (*logic)(void);
    void (*draw) (void);
} Delegate;

//dados globais do sistema(janela/renderder, input).
typedef struct {
    SDL_Renderer    *renderer;
    SDL_Window      *window;
    Delegate        delegate;
    int             keyboard[MAX_KEYBOARD_KEYS];
    int             mouse[MAX_MOUSE_BUTTONS];
} App;


/*Usado para representar objetos como jogadores, balas, inimigos etc..
+ encadeamento*/
struct Entity {
    float        x, y;
    int          width, height;
    int          health, reload;
    float        scale, dx, dy;
    SDL_Texture *texture;
    Entity      *next; //ponteiro que liga ao próximo nó
};

typedef struct {
    Entity  fighterHead, *fighterTail;
    Entity  bulletHead,  *bulletTail;
} Stage;

#endif // STRUCTS_H
