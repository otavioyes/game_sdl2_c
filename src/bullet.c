/*
 * bullet.c
 */
#include "common.h"
#include "stage.h"
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