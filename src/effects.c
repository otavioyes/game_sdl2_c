/*
 * effects.c
 */
/*
- doExplosions()
- drawExplosions()
- doDebris()
- drawDebris()
*/

#include "common.h"
#include "draw.h"
extern Stage stage;





/*==============================================================================
 * Atualiza efeitos de explosão ativos da fase.
 *
 * Responsabilidades:
 * - Atualizar movimentação das explosões
 * - Atualizar transparência (fade out)
 * - Remover explosões finalizadas
 * - Liberar memória de efeitos expirados
 * - Manter integridade da lista encadeada
 *============================================================================*/
void doExplosions(void) {
    Explosion *e;
    Explosion *prev;

    /* Ponteiro auxiliar para manipulação segura da lista */
    prev = &stage.explosionHead;

    /* Percorre lista de explosões ativas */
    for (e = stage.explosionHead.next; e != NULL; e = e->next) {

        /* Atualiza posição da explosão */
        e->x += e->dx;
        e->y += e->dy;

        /*
         * Reduz transparência gradualmente.
         * Quando alpha chega a zero,
         * a explosão é removida.
         */
        if (--e->a <= 0) {

            /*
             * Atualiza ponteiro final da lista
             * caso a explosão removida seja a última.
             */
            if (e == stage.explosionTail) {
                stage.explosionTail = prev;
            }

            /* Remove explosão da lista encadeada */
            prev->next = e->next;

            /* Libera memória da explosão */
            free(e);

            /*
             * Retorna uma posição no loop para manter
             * iteração segura após remoção do elemento.
             */
            e = prev;
        }

        /* Avança ponteiro auxiliar */
        prev = e;
    }
}



/*==============================================================================
 * Renderiza efeitos de explosão ativos da fase.
 *
 * Responsabilidades:
 * - Configurar modo de blend aditivo
 * - Aplicar coloração dinâmica das explosões
 * - Aplicar transparência variável
 * - Renderizar explosões na tela
 * - Restaurar estado padrão do renderer
 *============================================================================*/
void drawExplosions(SDL_Renderer *renderer, SDL_Texture *explosionTexture) {
    Explosion *e;

    /*
     * Ativa modo de blend aditivo
     * para criar efeito visual de brilho.
     */
    SDL_SetRenderDrawBlendMode(renderer,
                               SDL_BLENDMODE_ADD);

    /* Configura modo de transparência da textura */
    SDL_SetTextureAlphaMod(explosionTexture,
                           SDL_BLENDMODE_ADD);

    /* Percorre lista de explosões ativas */
    for (e = stage.explosionHead.next;
         e != NULL;
         e = e->next) {

        /* Aplica coloração individual da explosão */
        SDL_SetTextureColorMod(explosionTexture,
                               e->r,
                               e->g,
                               e->b);

        /* Aplica nível atual de transparência */
        SDL_SetTextureAlphaMod(explosionTexture,
                               e->a);

        /* Renderiza explosão */
        blit(explosionTexture,
             e->x,
             e->y);
    }

    /* Restaura modo padrão de renderização */
    SDL_SetRenderDrawBlendMode(renderer,
                               SDL_BLENDMODE_NONE);
}



/*==============================================================================
 * Atualiza fragmentos (debris) ativos da fase.
 *
 * Responsabilidades:
 * - Atualizar movimentação dos fragmentos
 * - Aplicar efeito de gravidade
 * - Atualizar tempo de vida dos debris
 * - Remover fragmentos expirados
 * - Liberar memória utilizada
 * - Manter integridade da lista encadeada
 *============================================================================*/
void doDebris(void){
    Debris *d;
    Debris *prev;

    /* Ponteiro auxiliar para manipulação segura da lista */
    prev = &stage.debrisHead;

    /* Percorre lista de debris ativos */
    for (d = stage.debrisHead.next; d != NULL; d = d->next) {

        /* Atualiza posição baseada na velocidade atual */
        d->x += d->dx;
        d->y += d->dy;

        /*
         * Aplica aceleração vertical simulando gravidade.
         */
        d->dy += 0.5;

        /*
         * Reduz tempo de vida do fragmento.
         * Quando chegar a zero, o debris é removido.
         */
        if (--d->life <= 0) {

            /*
             * Atualiza ponteiro final da lista
             * caso o debris removido seja o último elemento.
             */
            if (d == stage.debrisTail) {
                stage.debrisTail = prev;
            }

            /* Remove debris da lista encadeada */
            prev->next = d->next;

            /* Libera memória do fragmento */
            free(d);

            /*
             * Retorna uma posição no loop para manter
             * iteração segura após remoção.
             */
            d = prev;
        }

        /* Avança ponteiro auxiliar */
        prev = d;
    }
}



/*==============================================================================
 * Renderiza fragmentos (debris) ativos da fase.
 *
 * Responsabilidades:
 * - Percorrer lista de debris ativos
 * - Renderizar fragmentos individuais na tela
 *============================================================================*/
void drawDebris(void){
    Debris *d;

    /* Percorre lista de fragmentos ativos */
    for (d = stage.debrisHead.next;
         d != NULL;
         d = d->next) {

        /* Renderiza fragmento na posição atual */
        blit(d->texture, d->x, d->y);
    }
}