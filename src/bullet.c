/*
 * bullet.c
 */

#include "common.h"

#include "bullet.h"
#include "player.h"
#include "stage.h"
#include "sound.h"
#include "draw.h"
#include "util.h"
#include "points.h"
#include "entity.h"

extern Stage stage;

void fireBullet(SDL_Texture *texture)
{
    Entity *bullet;
    float angleRad;

    bullet = malloc(sizeof(Entity));

    if (bullet == NULL) {
        SDL_Log("malloc falhou em fireBullet");
        exit(1);
    }

    memset(bullet, 0, sizeof(Entity));

    bullet->texture = texture;
    bullet->side = SIDE_PLAYER;
    bullet->type = ET_PLAYER_BULLET;
    bullet->health = 1;

    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    angleRad = player->angle * (M_PI / 180.0f);

    bullet->dx = cosf(angleRad) * PLAYER_BULLET_SPEED;
    bullet->dy = sinf(angleRad) * PLAYER_BULLET_SPEED;

    bullet->x = player->x + (player->w / 2) - (bullet->w / 2);
    bullet->y = player->y + (player->h / 2) - (bullet->h / 2);

    bullet->angle = player->angle;

    addEntity(bullet);

    player->reload = 8;
}

void doBullet(SDL_Texture *pointsTexture)
{
    Entity *b;
    Entity *prev;

    prev = &stage.entityHead;

    for (b = stage.entityHead.next; b != NULL; b = b->next) {

        if (b->type != ET_PLAYER_BULLET &&
            b->type != ET_ALIEN_BULLET) {
            prev = b;
            continue;
        }

        b->x += b->dx;
        b->y += b->dy;

        if (bulletHitFighter(b, pointsTexture) ||
            b->x < -b->w ||
            b->y < -b->h ||
            b->x > SCREEN_WIDTH ||
            b->y > SCREEN_HEIGHT) {

            if (b == stage.entityTail) {
                stage.entityTail = prev;
            }

            prev->next = b->next;
            free(b);
            b = prev;
        }

        prev = b;
    }
}

void drawBullets(void)
{
    Entity *b;

    for (b = stage.entityHead.next; b != NULL; b = b->next) {

        if (b->type == ET_PLAYER_BULLET) {
            blitRotated(b->texture, b->x, b->y, b->angle);

        } else if (b->type == ET_ALIEN_BULLET) {
            blit(b->texture, b->x, b->y, 0);
        }
    }
}

int bulletHitFighter(Entity *b, SDL_Texture *pointsTexture)
{
    Entity *e;

    for (e = stage.entityHead.next; e != NULL; e = e->next) {

        if (e == b) {
            continue;
        }

        if (e->type != ET_PLAYER &&
            e->type != ET_ENEMY) {
            continue;
        }

        if (e->side != b->side &&
            collision(b->x, b->y, b->w, b->h,
                      e->x, e->y, e->w, e->h)) {

            b->health = 0;

            if (e->type == ET_PLAYER) {
                e->health -= 10;

                if (e->health < 0) {
                    e->health = 0;
                }

            } else if (e->type == ET_ENEMY) {
                e->health--;
            }

            addExplosions(e->x, e->y, 5);

            if (e->health <= 0) {
                addDebris(e);

                if (e->type == ET_PLAYER) {
                    playerSound(SND_ALIEN_DIE, CH_PLAYER);

                } else if (e->type == ET_ENEMY) {
                    addPointsPod(e->x + e->w / 2,
                                 e->y + e->h / 2,
                                 pointsTexture);

                    playerSound(SND_ALIEN_DIE, CH_ANY);
                }
            }

            return 1;
        }
    }

    return 0;
}