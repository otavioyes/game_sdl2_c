/*
 * entity.c
 */

#include "common.h"

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



/*
 * Atualiza as entidades da lista de lutadores.
 *
 * Move jogador e inimigos, remove inimigos que saíram da tela
 * e libera entidades sem vida.
 */
void doFighters(void)
{
    Entity *e;
    Entity *prev;

    prev = &stage.fighterHead;

    for (e = stage.fighterHead.next; e != NULL; e = e->next) {
        e->x += e->dx;
        e->y += e->dy;

        if (e != player && e->x < -e->w) {
            e->health = 0;
        }

        if (e->health == 0) {
            if (e == player) {
                player = NULL;
            }

            if (e == stage.fighterTail) {
                stage.fighterTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
    }
}



/*
 * Desenha jogador e inimigos.
 *
 * O jogador usa rotação, enquanto os inimigos ainda são
 * desenhados sem rotação.
 */
void drawFighters(void)
{
    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next) {
        if (e == player) {
            blitRotated(e->texture, e->x, e->y, e->angle);
        } else {
            blit(e->texture, e->x, e->y, 0);
        }
    }
}