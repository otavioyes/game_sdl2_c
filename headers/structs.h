// structs.h
// Cabecalho que define as estruturas de dados principais usadas no jogo

#ifndef STRUCTS_H // Prevencao contra multiplas inclusoes do cabecalho
#define STRUCTS_H // Define a flag para inclusoes unica

#include "defs.h" // Inclui definicoes e constantes como tamanho e limites
#include <SDL2/SDL.h> // Inclui os tipos da lib SDL (ex: SDL_Windowm, SDL_Renderer, SDL_Texture)

typedef struct Entity Entity; // Declaracao antecipadada struct Entity, permitindo o uso de ponteiros antes da definicao completa

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
