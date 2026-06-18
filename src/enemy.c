/*
 * enemy.c
 */

#include "common.h"

#include "sound.h"
#include "util.h"
#include "enemy.h"
#include "player.h"
#include "entity.h"

extern Stage stage;

static int enemySpawnTimer;

void initEnemies(void)
{
    enemySpawnTimer = 0;
}

void spawnEnemies(SDL_Texture *enemyTexture)
{
    Entity *enemy;

    if (--enemySpawnTimer <= 0) {
        enemy = malloc(sizeof(Entity));

        if (enemy == NULL) {
            SDL_Log("malloc falhou em spawnEnemies");
            exit(1);
        }

        memset(enemy, 0, sizeof(Entity));

        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % SCREEN_HEIGHT;
        enemy->texture = enemyTexture;

        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

        enemy->dx = -(2 + (rand() % 4));
        enemy->dy = (-100 + (rand() % 200)) / 100.0f;

        enemy->side = SIDE_ALIEN;
        enemy->type = ET_ENEMY;
        enemy->health = 1;
        enemy->reload = FPS * (1 + (rand() % 3));

        addEntity(enemy);

        enemySpawnTimer = 30 + (rand() % FPS);
    }
}

void doEnemies(SDL_Texture *alienBulletTexture)
{
    Entity *e;

    for (e = stage.entityHead.next; e != NULL; e = e->next) {
        if (e->type == ET_ENEMY) {
            e->y = MIN(MAX(e->y, 0), SCREEN_HEIGHT - e->h);

            if (player != NULL && --e->reload <= 0) {
                fireAlienBullet(e, alienBulletTexture);
                playerSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
            }
        }
    }
}

void fireAlienBullet(Entity *e, SDL_Texture *texture)
{
    Entity *bullet;

    if (!canAlienShootPlayer(e)) {
        return;
    }

    bullet = malloc(sizeof(Entity));

    if (bullet == NULL) {
        SDL_Log("malloc falhou em fireAlienBullet");
        exit(1);
    }

    memset(bullet, 0, sizeof(Entity));

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->texture = texture;
    bullet->side = SIDE_ALIEN;
    bullet->type = ET_ALIEN_BULLET;

    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    calcSlop(player->x + (player->w / 2),
             player->y + (player->h / 2),
             e->x + (e->w / 2),
             e->y + (e->h / 2),
             &bullet->dx,
             &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    addEntity(bullet);

    e->reload = rand() % (FPS * 2);
}

int canAlienShootPlayer(Entity *e)
{
    float enemyCenterX;
    float enemyCenterY;
    float playerCenterX;
    float playerCenterY;
    float dx;
    float dy;
    float length;
    float dot;
    float cosAngle;

    if (player == NULL) {
        return 0;
    }

    enemyCenterX = e->x + (e->w / 2);
    enemyCenterY = e->y + (e->h / 2);

    playerCenterX = player->x + (player->w / 2);
    playerCenterY = player->y + (player->h / 2);

    dx = playerCenterX - enemyCenterX;
    dy = playerCenterY - enemyCenterY;

    length = sqrtf(dx * dx + dy * dy);

    if (length == 0.0f) {
        return 1;
    }

    dot = (dx * -1.0f) + (dy * 0.0f);

    cosAngle = dot / length;

    return cosAngle >= 0.0f;
}