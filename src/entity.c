/*
 * entity.c
 */

#include "common.h"

#include "entity.h"
#include "player.h"
#include "draw.h"

extern Stage stage;

void addEntity(Entity *e)
{
    stage.entityTail->next = e;
    stage.entityTail = e;
}

void drawEntities(void)
{
    Entity *e;

    for (e = stage.entityHead.next; e != NULL; e = e->next) {
        if (e->type == ET_PLAYER) {
            blitRotated(e->texture, e->x, e->y, e->angle);
        } else if (e->type == ET_ENEMY) {
            blit(e->texture, e->x, e->y, 0);
        }
    }
}

void doEntities(void)
{
    Entity *e;
    Entity *prev;

    prev = &stage.entityHead;

    for (e = stage.entityHead.next; e != NULL; e = e->next) {
        e->x += e->dx;
        e->y += e->dy;

        if (e->type == ET_ENEMY && e->x < -e->w) {
            e->health = 0;
        }

        if (e->health == 0) {
            if (e == player) {
                player = NULL;
            }

            if (e == stage.entityTail) {
                stage.entityTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
    }
}