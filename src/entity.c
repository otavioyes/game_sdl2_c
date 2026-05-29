/*
 * entity.c
 */

#include "common.h"

#include "stage.h"
#include "player.h"
#include "draw.h"

extern Stage stage;

/*==============================================================================
 * Verificar se a entidade é o jogador
 * e impedir que ele saia da tela
 * Os cálculos dos valores de 'x' e 'y', suando
 * as macros MIN e MAX, garante que o jogador nunca
 * ultrapasse os limites da tela
 *============================================================================*/


void doEntities(void)
{
    Entity *e;
    for(e = stage.entityHead.next; e != NULL; e = e->next){
        e->x += e->dx;
        e->y += e->dy;

        if (e == player){
            e->x = MIN(MAX(e->x, e->w / 2), SCREEN_WIDTH - e->w / 2);
            e->y = MIN(MAX(e->y, e->h / 2), SCREEN_HEIGHT - e->h /2);
        }
    }
}

static void drawEntities(void)
{
    Entity *e;

    for (e = stage.entityHead.next; e != NULL; e = e->next){
        blitRotated(e->texture, e->x, e->y, e->angle);
    }
}

