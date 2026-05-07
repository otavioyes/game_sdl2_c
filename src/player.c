/*
 * player.c
 */

#include "common.h"


extern Stage stage;

void initPlayer(SDL_Texture *texture);

static Entity       *player;



/*
 * Inicializa a entidade do jogador.
 *
 * Responsabilidades:
 * - Alocar memória para o player
 * - Inserir o player na lista de entidades da fase
 * - Configurar atributos iniciais
 * - Associar a textura do jogador
 * - Obter largura e altura da textura
 */
void initPlayer(SDL_Texture *texture)
{
    /* Aloca memória para a entidade do jogador */
    player = malloc(sizeof(Entity));

    /* Verifica falha na alocação */
    if (player == NULL) {
        SDL_Log("malloc falhou em initPlayer");
        exit(1);
    }

    /* Zera todos os dados da estrutura */
    memset(player, 0, sizeof(Entity));

    /* Adiciona o jogador na lista de fighters da fase */
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    /* Define atributos iniciais do jogador */
    player->health = PLAYER_MAX_HEALTH;
    player->x = 100;
    player->y = 100;

    /* Associa textura do jogador */
    player->texture = texture;

    /* Obtém dimensões da textura */
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

    /* Define o lado da entidade */
    player->side = SIDE_PLAYER;
}

