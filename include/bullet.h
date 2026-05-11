/*
 * bullet.h
 */

/* Cria um novo projétil */
void fireBullet(SDL_Texture *texture);

/* Atualiza lógica dos projéteis */
void doBullet(void);

/* Renderiza projéteis ativos */
void drawBullets(void);

/* Verifica colisão entre projétil e entidades */
int bulletHitFighter(Entity *b);


