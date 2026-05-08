/*
 * bullet.c
 */
#include "common.h"

#include "player.h"

extern Stage stage;



/*==============================================================================
 * Cria um novo projétil disparado pelo jogador.
 *
 * Responsabilidades:
 * - Alocar memória para o projétil
 * - Inserir projétil na lista de balas da fase
 * - Configurar propriedades iniciais do disparo
 * - Calcular direção baseada no ângulo da nave
 * - Posicionar projétil no centro do jogador
 * - Aplicar tempo de recarga do disparo
 *============================================================================*/
void fireBullet(SDL_Texture *texture) {
    Entity *bullet;

    /* Ângulo convertido para radianos */
    float angleRad;

    /* Aloca memória para a nova bala */
    bullet = malloc(sizeof(Entity));

    /* Verifica falha de alocação */
    if (bullet == NULL) {
        SDL_Log("malloc falhou em fireBullet");
        exit(1);
    }

    /* Inicializa estrutura com zero */
    memset(bullet, 0, sizeof(Entity));

    /* Adiciona bala na lista encadeada de projéteis */
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    /* Configuração inicial do projétil */
    bullet->texture = texture;
    bullet->side = SIDE_PLAYER;
    bullet->health = 1;

    /* Obtém dimensões da textura */
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    /*
     * SDL_RenderCopyEx utiliza graus para rotação,
     * porém funções trigonométricas utilizam radianos.
     */
    angleRad = player->angle * (M_PI / 180.0f);

    /*
     * Calcula velocidade horizontal e vertical
     * baseada no ângulo atual da nave.
     *
     * cos() controla eixo X
     * sin() controla eixo Y
     */
    bullet->dx = cosf(angleRad) * PLAYER_BULLET_SPEED;
    bullet->dy = sinf(angleRad) * PLAYER_BULLET_SPEED;

    /*
     * Posiciona o projétil no centro do jogador
     * para manter alinhamento visual do disparo.
     */
    bullet->x = player->x + (player->w / 2) - (bullet->w / 2);
    bullet->y = player->y + (player->h / 2) - (bullet->h / 2);

    /* Mantém rotação visual sincronizada com a nave */
    bullet->angle = player->angle;

    /* Aplica tempo de recarga entre disparos */
    player->reload = 8;
}



/*==============================================================================
 * Atualiza lógica dos projéteis ativos.
 *
 * Responsabilidades:
 * - Atualizar posição das balas
 * - Detectar colisões com entidades
 * - Remover projéteis fora da tela
 * - Liberar memória de projéteis destruídos
 * - Manter integridade da lista encadeada
 *============================================================================*/
void doBullet(void)
{
    Entity *b;
    Entity *prev;

    /* Ponteiro auxiliar para manipulação segura da lista encadeada */
    prev = &stage.bulletHead;

    /* Percorre todos os projéteis ativos */
    for (b = stage.bulletHead.next; b != NULL; b = b->next) {

        /* Atualiza posição baseada na velocidade atual */
        b->x += b->dx;
        b->y += b->dy;

        /*
         * Remove projétil caso:
         * - atinja uma entidade
         * - saia dos limites visíveis da tela
         */
        if (bulletHitFighter(b) ||
            b->x < -b->w ||
            b->y < -b->h ||
            b->x > SCREEN_WIDTH ||
            b->y > SCREEN_HEIGHT) {

            /*
             * Atualiza ponteiro final da lista
             * caso o projétil removido seja o último elemento.
             */
            if (b == stage.bulletTail) {
                stage.bulletTail = prev;
            }

            /* Remove projétil da lista encadeada */
            prev->next = b->next;

            /* Libera memória da entidade removida */
            free(b);

            /*
             * Retorna uma posição no loop para manter
             * iteração segura após remoção do elemento.
             */
            b = prev;
        }

        /* Avança ponteiro auxiliar */
        prev = b;
    }
}