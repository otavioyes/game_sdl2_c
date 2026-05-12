/*
 * bullet.h
 */

/* Cria um novo projétil */
void fireBullet(SDL_Texture *texture);

/* Atualiza lógica dos projéteis */
void doBullet(SDL_Texture *pointsTexture);

/* Renderiza projéteis ativos */
void drawBullets(void);

/* Verifica colisão entre projétil e entidades */
int bulletHitFighter(Entity *b, SDL_Texture *pointsTexture);


