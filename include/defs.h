// defs.h
// Arquivo de definicoes e constantes globais para o jogo

#ifndef DEFS_H
#define DEFS_H

/* --- Se possível, inclua SDL para garantir que SDL_NUM_SCANCODES exista --- */
/* Usamos __has_include para ser mais tolerante; se o seu compilador não suportar,
   pode incluí-lo via common.h antes de incluir defs.h */
#if defined(__has_include)
  #if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>
  #endif
#endif

/* Se SDL_NUM_SCANCODES não estiver definido (fallback), defina um valor razoável */
#ifndef SDL_NUM_SCANCODES
  #define SDL_NUM_SCANCODES 350
#endif

/* Número máximo de teclas e botões do mouse */
#define MAX_KEYBOARD_KEYS   SDL_NUM_SCANCODES
#define MAX_MOUSE_BUTTONS   3

/* Tamanho da janela (pixels) */
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT  720

/* Velocidades e constantes de jogo */
#define PLAYER_SPEED         4   /* pixels por frame */
#define PLAYER_BULLET_SPEED 16   /* velocidade das balas do jogador (px/frame) */

#define MAX_BULLETS 128 /* numero maximo de projeteis ativos */

/* Lados para colisao */
#define SIDE_PLAYER 0
#define SIDE_ALIEN  1

/* Macros utilitárias - versão segura usando inline functions para evitar
   dupla avaliação (side effects) quando possível. Essas funções tratam inteiros. */
static inline int max_int(int a, int b) { return a > b ? a : b; }
static inline int min_int(int a, int b) { return a < b ? a : b; }

/* Mantemos as macros tradicionais para compatibilidade, mas delegando às inline funcs.
   Evite passar expressões com efeitos colaterais (por ex. i++) para MAX/MIN. */
#define MAX(a,b) (max_int((int)(a), (int)(b)))
#define MIN(a,b) (min_int((int)(a), (int)(b)))

/* FPS e velocidade de bala inimiga */
#define FPS 60
#define ALIEN_BULLET_SPEED 8

#endif /* DEFS_H */
