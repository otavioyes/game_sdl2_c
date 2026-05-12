/*
 * effects.c
 */

/*
 ** effects.c
-
- addExplosions() OK
- doExplosions() OK
- drawExplosions() OK
- addDebris() OK
- doDebris() OK
- drawDebris() OK
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



/*==============================================================================
 * Cria partículas de explosão na posição informada.
 *
 * Responsabilidades:
 * - Alocar memória para partículas de explosão
 * - Inserir explosões na lista encadeada
 * - Configurar posição inicial das partículas
 * - Aplicar movimentação aleatória
 * - Configurar coloração visual
 * - Definir duração do efeito
 *============================================================================*/
void addExplosions(int x, int y, int num)/*ass. em stage.h*/
{
    Explosion *e;

    int i;

    /* Cria quantidade solicitada de partículas */
    for (i = 0; i < num; i++) {

        /* Aloca memória para nova explosão */
        e = malloc(sizeof(Explosion));

        /* Verifica falha de alocação */
        if (e == NULL) {
            SDL_Log("malloc falhou em addExplosions");
            exit(1);
        }

        /* Inicializa estrutura com zero */
        memset(e, 0, sizeof(Explosion));

        /* Adiciona explosão na lista encadeada */
        stage.explosionTail->next = e;
        stage.explosionTail = e;

        /*
         * Aplica pequena variação de posição
         * para criar espalhamento visual da explosão.
         */
        e->x = x + (rand() % 8) - (rand() % 8);
        e->y = y + (rand() % 8) - (rand() % 8);

        /*
         * Define movimentação aleatória das partículas
         * para simular dispersão da explosão.
         */
        e->dx = (rand() % 6) - (rand() % 6);
        e->dy = (rand() % 6) - (rand() % 6);

        /* Reduz intensidade da movimentação */
        e->dx /= 10;
        e->dy /= 10;

        /*
         * Define coloração aleatória da explosão.
         *
         * Tons possíveis:
         * - vermelho
         * - laranja
         * - amarelo
         * - branco
         */
        switch (rand() % 4) {

            case 0:
                e->r = 255;
                break;

            case 1:
                e->r = 255;
                e->g = 128;
                break;

            case 2:
                e->r = 255;
                e->g = 255;
                break;

            default:
                e->r = 255;
                e->g = 255;
                e->b = 255;
                break;
        }

        /*
         * Define duração da explosão
         * utilizando canal alpha.
         */
        e->a = 40 + (rand() % 60);
    }
}



/*==============================================================================
 * Cria fragmentos (debris) a partir de uma entidade destruída.
 *
 * Responsabilidades:
 * - Dividir sprite da entidade em fragmentos
 * - Alocar memória para debris
 * - Inserir debris na lista encadeada
 * - Configurar posição inicial dos fragmentos
 * - Aplicar movimentação aleatória
 * - Definir tempo de vida dos debris
 * - Configurar região da textura utilizada
 *============================================================================*/
void addDebris(Entity *e) /*ass. em stage.h*/
{
    Debris *d;

    int x;
    int y;

    int w;
    int h;

    /*
     * Divide sprite da entidade em 4 partes.
     */
    w = e->w / 2;
    h = e->h / 2;

    /*
     * Percorre regiões da textura para gerar
     * múltiplos fragmentos da entidade destruída.
     */
    for (y = 0; y <= h; y += h) {

        for (x = 0; x <= w; x += w) {

            /* Aloca memória para novo debris */
            d = malloc(sizeof(Debris));

            /* Inicializa estrutura com zero */
            memset(d, 0, sizeof(Debris));

            /* Adiciona debris na lista encadeada */
            stage.debrisTail->next = d;
            stage.debrisTail = d;

            /*
             * Posiciona fragmento no centro
             * da entidade destruída.
             */
            d->x = e->x + e->w / 2;
            d->y = e->y + e->h / 2;

            /*
             * Define movimentação aleatória
             * para efeito de explosão.
             */
            d->dx = (rand() % 5) - (rand() % 5);

            /*
             * Impulsiona debris para cima
             * simulando dispersão da explosão.
             */
            d->dy = -(5 + (rand() % 12));

            /* Define duração do fragmento */
            d->life = FPS * 2;

            /*
             * Define região da textura original
             * utilizada pelo fragmento.
             */
            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }
    }
}