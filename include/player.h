/*
 * initPLayer.h
 */


/* Entidade principal controlada pelo jogador */
extern Entity *player;

/* Inicializa o jogador */
void initPlayer(SDL_Texture *texture);

/* Atualiza lógica do jogador */
void doPlayer(SDL_Texture *bulletTexture);

/* Mantém jogador dentro da tela */
void clipPlayer(void);

/* Desenha HUD de vida do jogador */
void drawPlayerHealthBar(void);


