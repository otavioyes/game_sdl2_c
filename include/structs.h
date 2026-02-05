
//Cabecalho que define as estruturas de dados principais usadas no jogo


typedef struct Entity       Entity;
typedef struct Explosion    Explosion;
typedef struct Debris       Debris;
typedef struct Texture      Texture;

typedef struct
{
    void (*logic) (void);
    void (*draw) (void);
}Delegate;


#ifndef STRUCTS_H // Prevencao contra multiplas inclusoes do cabecalho
#define STRUCTS_H // Define a flag para inclusoes unica

#include "defs.h" // Inclui definicoes e constantes como tamanho e limites
#include <SDL2/SDL.h> // Inclui os tipos da lib SDL (ex: SDL_Windowm, SDL_Renderer, SDL_Texture)

//typedef struct Entity Entity; // Declaracao antecipadada struct Entity, permitindo o uso de ponteiros antes da definicao completa

// Encapsula funcoes de logicas de desenho
typedef struct {
    void (*logic)(void); // Ponteiro para funcao que trata a logica (atualizacao)
    void (*draw) (void); // Ponteiro para funcao que trata desenho (renderizacao)
} Delegate;

// Dados globais do sistema (janela/renderder, entradas)
typedef struct {
    SDL_Renderer    *renderer; // Ponteiros para renderizaacao SDL
    SDL_Window      *window; // Ponteiros para janela SDL
    Delegate        delegate; // Estruturas com ponteiros para funcao de logica e desenho
    int             keyboard[MAX_KEYBOARD_KEYS]; // Estado das teclas do teclado
    int             mouse[MAX_MOUSE_BUTTONS]; // EStado dos botoes do mouse
} App;


/* Usado para representar objetos como jogadores, balas, inimigos etc..
+ encadeamento*/
typedef struct Entity {
    float           x, y; // Posicao no mundo
    int             width, height; // Dimensoes da entidade
    int             health, reload; // Vida e tempo de recarga (usado para atirar, por exemplo)
    float           scale, dx, dy; // Escala de desenho, velocidade nos eixos X e Y
    SDL_Texture     *texture; // Ponteiro para a textura usada na renderizacao da entidade
    struct Entity   *next; // Ponteiro para a proxima entidade na lista (lista encadeada)
    int             side;
} Entity;

int checkCollisionEntities(Entity *x, Entity *y);


// Representa o estado da fase do jogo com lista de entidade
typedef struct
{
    Entity  fighterHead, *fighterTail; // Lista de entidades do tipo lutador (jogador, inimigo)
    Entity  bulletHead,  *bulletTail; // Lista de projeteis (balas)
} Stage;

#endif // STRUCTS_H // Finaliza a protecao contra multiplas inclusoes
