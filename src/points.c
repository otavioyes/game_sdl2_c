/*
 * points.c
 */

#include "common.h"

#include "util.h"
#include "player.h"
#include "sound.h"
#include "draw.h"

extern Stage stage;


/*==============================================================================
 * Atualiza cápsulas de pontos ativas da fase.
 *
 * Responsabilidades:
 * - Atualizar movimentação das cápsulas
 * - Aplicar colisão com limites da tela
 * - Detectar coleta pelo jogador
 * - Atualizar pontuação da fase
 * - Reproduzir efeito sonoro de coleta
 * - Remover cápsulas expiradas ou coletadas
 * - Liberar memória utilizada
 * - Manter integridade da lista encadeada
 *============================================================================*/
void doPointsPods(void)
{
    Entity *e;
    Entity *prev;

    /* Ponteiro auxiliar para manipulação segura da lista encadeada */
    prev = &stage.pointsHead;

    /* Percorre todas as cápsulas de pontos ativas */
    for (e = stage.pointsHead.next; e != NULL; e = e->next) {

        /* Atualiza posição da cápsula */
        e->x += e->dx;
        e->y += e->dy;

        /*
         * Colisão com limite esquerdo da tela.
         *
         * Quando a cápsula encosta na borda,
         * sua direção horizontal é invertida.
         */
        if (e->x < 0) {
            e->x = 0;
            e->dx = -e->dx;
        }

        /*
         * Colisão com limite direito da tela.
         */
        if (e->x + e->w > SCREEN_WIDTH) {
            e->x = SCREEN_WIDTH - e->w;
            e->dx = -e->dx;
        }

        /*
         * Colisão com limite superior da tela.
         */
        if (e->y < 0) {
            e->y = 0;
            e->dy = -e->dy;
        }

        /*
         * Colisão com limite inferior da tela.
         */
        if (e->y + e->h > SCREEN_HEIGHT) {
            e->y = SCREEN_HEIGHT - e->h;
            e->dy = -e->dy;
        }

        /*
         * Detecta coleta da cápsula pelo jogador.
         */
        if (player != NULL &&
            collision(e->x, e->y, e->w, e->h,
                      player->x, player->y,
                      player->w, player->h)) {

            /* Marca cápsula para remoção */
            e->health = 0;

            /* Incrementa pontuação da fase */
            stage.score++;

            /* Reproduz efeito sonoro de coleta */
            playerSound(SND_POINTS, CH_POINTS);
        }

        /*
         * Reduz tempo de vida da cápsula.
         *
         * Quando health chega a zero, a cápsula
         * é removida da lista.
         */
        if (--e->health <= 0) {

            /*
             * Atualiza ponteiro final da lista
             * caso a cápsula removida seja a última.
             */
            if (e == stage.pointsTail) {
                stage.pointsTail = prev;
            }

            /* Remove cápsula da lista encadeada */
            prev->next = e->next;

            /* Libera memória da entidade removida */
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
 * Renderiza cápsulas de pontos ativas da fase.
 *
 * Responsabilidades:
 * - Percorrer lista de cápsulas ativas
 * - Aplicar efeito visual de piscagem próximo do fim da vida
 * - Renderizar cápsulas visíveis na tela
 *============================================================================*/
void drawPointsPods(void)
{
    Entity *e;

    /* Percorre todas as cápsulas de pontos ativas */
    for (e = stage.pointsHead.next; e != NULL; e = e->next) {

        /*
         * Mantém cápsula sempre visível no início da vida
         * e aplica piscagem quando estiver próxima de expirar.
         */
        if (e->health > (FPS * 2) ||
            e->health % 12 < 6) {

            /* Renderiza cápsula centralizada na posição atual */
            blit(e->texture, e->x, e->y, 1);
        }
    }
}


/*==============================================================================
 * Cria uma cápsula de pontos.
 *
 * Responsabilidades:
 * - Alocar memória para a entidade
 * - Validar falha de alocação
 * - Inserir cápsula na lista de pontos
 * - Configurar posição inicial
 * - Definir movimentação leve e aleatória
 * - Configurar tempo de vida
 * - Associar textura da cápsula
 * - Centralizar renderização na posição informada
 *============================================================================*/
void addPointsPod(int x, int y, SDL_Texture *pointsTexture)
{
    Entity *e;

    /* Aloca memória para nova cápsula */
    e = malloc(sizeof(Entity));

    /* Verifica falha de alocação */
    if (e == NULL) {
        SDL_Log("malloc falhou em addPointsPod");
        exit(1);
    }

    /* Inicializa estrutura com zero */
    memset(e, 0, sizeof(Entity));

    /* Adiciona cápsula ao final da lista encadeada */
    stage.pointsTail->next = e;
    stage.pointsTail = e;

    /* Define posição inicial da cápsula */
    e->x = x;
    e->y = y;

    /*
     * Define movimentação leve em todas as direções.
     *
     * Isso cria sensação de flutuação espacial,
     * evitando movimento artificial apenas para a esquerda.
     */
    e->dx = ((rand() % 5) - 2) / 2.0f;
    e->dy = ((rand() % 5) - 2) / 2.0f;

    /* Define tempo de vida da cápsula */
    e->health = FPS * 10;

    /* Define tipo da entidade, caso usado para debug ou lógica futura */
    e->type = ET_POINTS_POD;

    /* Associa textura da cápsula */
    e->texture = pointsTexture;

    /* Obtém dimensões da textura */
    SDL_QueryTexture(e->texture,
                     NULL,
                     NULL,
                     &e->w,
                     &e->h);

    /*
     * Centraliza cápsula na posição informada
     * para melhor alinhamento visual.
     */
    e->x -= e->w / 2;
    e->y -= e->h / 2;
}