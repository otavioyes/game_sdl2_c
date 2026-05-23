/*
 * effects.c
 */

#include "common.h"

#include "draw.h"

extern Stage stage;


/*==============================================================================
 * Atualiza efeitos de explosão ativos da fase.
 *
 * Responsabilidades:
 * - Atualizar movimentação das explosões
 * - Reduzir transparência gradualmente
 * - Remover explosões finalizadas
 * - Liberar memória de efeitos expirados
 * - Manter integridade da lista encadeada
 *============================================================================*/
void doExplosions(void)
{
    Explosion *e;
    Explosion *prev;

    /* Ponteiro auxiliar para remoção segura na lista encadeada */
    prev = &stage.explosionHead;

    /* Percorre todas as explosões ativas */
    for (e = stage.explosionHead.next; e != NULL; e = e->next) {

        /* Atualiza posição da partícula */
        e->x += e->dx;
        e->y += e->dy;

        /*
         * Reduz o canal alpha gradualmente.
         *
         * Quando alpha chega a zero, a explosão
         * é considerada finalizada e removida.
         */
        if (--e->a <= 0) {

            /*
             * Se o item removido for o último da lista,
             * atualiza o ponteiro tail para o item anterior.
             */
            if (e == stage.explosionTail) {
                stage.explosionTail = prev;
            }

            /* Remove explosão da lista */
            prev->next = e->next;

            /* Libera memória da explosão removida */
            free(e);

            /*
             * Retorna o iterador para o item anterior
             * para manter o loop seguro após o free.
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
 * - Configurar blend aditivo para brilho
 * - Aplicar cor individual em cada explosão
 * - Aplicar transparência individual
 * - Renderizar partículas de explosão
 * - Restaurar estado padrão do renderer
 *============================================================================*/
void drawExplosions(SDL_Renderer *renderer, SDL_Texture *explosionTexture)
{
    Explosion *e;

    /*
     * Blend aditivo cria efeito de brilho,
     * ideal para explosões, fogo e partículas luminosas.
     */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    /* Percorre todas as explosões ativas */
    for (e = stage.explosionHead.next; e != NULL; e = e->next) {

        /* Define cor individual da partícula */
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);

        /* Define transparência atual da partícula */
        SDL_SetTextureAlphaMod(explosionTexture, e->a);

        /* Renderiza explosão na posição atual */
        blit(explosionTexture, e->x, e->y, 1);
    }

    /*
     * Restaura o estado padrão para evitar que outros
     * elementos do jogo herdem o blend aditivo.
     */
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_BLEND);
}


/*==============================================================================
 * Atualiza fragmentos ativos da fase.
 *
 * Responsabilidades:
 * - Atualizar posição dos debris
 * - Aplicar gravidade simulada
 * - Reduzir tempo de vida
 * - Remover fragmentos expirados
 * - Liberar memória utilizada
 * - Manter integridade da lista encadeada
 *============================================================================*/
void doDebris(void)
{
    Debris *d;
    Debris *prev;

    /* Ponteiro auxiliar para remoção segura na lista encadeada */
    prev = &stage.debrisHead;

    /* Percorre todos os fragmentos ativos */
    for (d = stage.debrisHead.next; d != NULL; d = d->next) {

        /* Atualiza posição do fragmento */
        d->x += d->dx;
        d->y += d->dy;

        /*
         * Aplica aceleração vertical,
         * simulando gravidade no fragmento.
         */
        /*d->dy += 0.5f;*/

        /*
         * Reduz tempo de vida.
         *
         * Quando life chega a zero, o fragmento
         * é removido da fase.
         */
        if (--d->life <= 0) {

            /*
             * Se o debris removido for o último da lista,
             * atualiza o ponteiro tail para o item anterior.
             */
            if (d == stage.debrisTail) {
                stage.debrisTail = prev;
            }

            /* Remove debris da lista */
            prev->next = d->next;

            /* Libera memória do fragmento removido */
            free(d);

            /*
             * Retorna o iterador para o item anterior
             * para manter o loop seguro após o free.
             */
            d = prev;
        }

        /* Avança ponteiro auxiliar */
        prev = d;
    }
}


/*==============================================================================
 * Renderiza fragmentos ativos da fase.
 *
 * Responsabilidades:
 * - Percorrer lista de debris
 * - Renderizar apenas a região correta da textura original
 * - Exibir fragmentos visuais da entidade destruída
 *============================================================================*/
void drawDebris(void)
{
    Debris *d;

    /* Percorre todos os fragmentos ativos */
    for (d = stage.debrisHead.next; d != NULL; d = d->next) {

        /*
         * Renderiza apenas o recorte da textura original.
         *
         * Isso cria o efeito visual de sprite quebrado
         * em pedaços menores.
         */
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
}


/*==============================================================================
 * Cria partículas de explosão na posição informada.
 *
 * Responsabilidades:
 * - Alocar memória para partículas de explosão
 * - Inserir explosões na lista encadeada
 * - Configurar posição inicial
 * - Aplicar movimentação aleatória
 * - Configurar coloração visual
 * - Definir duração do efeito via alpha
 *============================================================================*/
void addExplosions(int x, int y, int num)
{
    Explosion *e;
    int i;

    /* Cria a quantidade solicitada de partículas */
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

        /* Adiciona explosão ao final da lista encadeada */
        stage.explosionTail->next = e;
        stage.explosionTail = e;

        /*
         * Aplica pequena variação de posição,
         * criando espalhamento visual da explosão.
         */
        e->x = x + (rand() % 8) - (rand() % 8);
        e->y = y + (rand() % 8) - (rand() % 8);

        /*
         * Define movimentação aleatória.
         *
         * A divisão por 10.0f força cálculo em ponto flutuante,
         * evitando perda de movimento por divisão inteira.
         */
        e->dx = ((rand() % 6) - (rand() % 6)) / 10.0f;
        e->dy = ((rand() % 6) - (rand() % 6)) / 10.0f;

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
         * Define duração visual da explosão.
         *
         * O valor alpha também controla o tempo
         * até a partícula desaparecer.
         */
        e->a = 40 + (rand() % 60);
    }
}


/*==============================================================================
 * Cria fragmentos a partir de uma entidade destruída.
 *
 * Responsabilidades:
 * - Dividir sprite da entidade em regiões menores
 * - Alocar memória para cada fragmento
 * - Inserir debris na lista encadeada
 * - Configurar posição inicial
 * - Aplicar movimentação aleatória
 * - Definir tempo de vida
 * - Armazenar recorte da textura original
 *============================================================================*/
void addDebris(Entity *e)
{
    Debris *d;

    int x;
    int y;

    int w;
    int h;

    /*
     * Divide o sprite da entidade em 4 partes.
     */
    w = e->w / 4;
    h = e->h / 4;

    /*
     * Percorre as regiões da textura original
     * para gerar múltiplos fragmentos visuais.
     */
    for (y = 0; y < e->h; y += h) {

        for (x = 0; x < e->w; x += w) {

            /* Aloca memória para novo debris */
            d = malloc(sizeof(Debris));

            /* Verifica falha de alocação */
            if (d == NULL) {
                SDL_Log("malloc falhou em addDebris");
                exit(1);
            }

            /* Inicializa estrutura com zero */
            memset(d, 0, sizeof(Debris));

            /* Adiciona debris ao final da lista encadeada */
            stage.debrisTail->next = d;
            stage.debrisTail = d;

            /*
             * Usa a mesma textura da entidade destruída.
             *
             * O debris renderiza apenas uma região dessa textura,
             * definida pelo SDL_Rect abaixo.
             */
            d->texture = e->texture;

            /*
             * Posiciona fragmento no centro da entidade destruída.
             */
            d->x = e->x + e->w / 2;
            d->y = e->y + e->h / 2;

            /*
             * Aplica pequeno deslocamento aleatório,
             * mantendo os destroços próximos da explosão.
             * */
            d->dx = ((rand() % 5) - 2) / 2.0f;
            d->dy = ((rand() % 5) - 2) / 2.0f;

            /* Define duração menor do fragmento
             * evitando poluicao visual na tela
             */
            d->life = FPS * 1;

            /*
             * Define qual parte da textura original
             * será renderizada por este debris.
             */
            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }
    }
}