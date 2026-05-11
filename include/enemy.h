/*
 * enemy.h
 * */

void initEnemies(void);
void spawnsEnemies(SDL_Texture *enemyTexture);
void doEnemies(SDL_Texture *alienBulletTexture);
void fireAlienBullet(Entity *e, SDL_Texture *texture);
int canAlienShootPlayer(Entity *e);

