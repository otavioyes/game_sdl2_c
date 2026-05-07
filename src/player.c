/*
 * player.c
 */

#include "common.h"

#include "player.h"
#include "sound.h"


extern Stage stage;
extern App app;
extern void fireBullet(void);

void initPlayer(SDL_Texture *texture);
void doPlayer(void);

Entity *player;



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


void doPlayer(void) {
    if (player == NULL) {
        return;
    }

    player->dx = player->dy = 0;

    if (player->reload > 0) {
        player->reload--;
    }

    if (app.keyboard[SDL_SCANCODE_A] || app.keyboard[SDL_SCANCODE_LEFT]) {
        player->dx = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_D] || app.keyboard[SDL_SCANCODE_RIGHT]) {
        player->dx = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_W] || app.keyboard[SDL_SCANCODE_UP]) {
        player->dy = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_DOWN]) {
        player->dy = PLAYER_SPEED;
    }


    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload <= 0) {
        playerSound(SND_PLAYER_FIRE, CH_PLAYER);
        fireBullet();
    }


    if (app.keyboard[SDL_SCANCODE_Q]) {
        player->angle -= 4;
    }

    if (app.keyboard[SDL_SCANCODE_E]) {
        player->angle += 4;
    }
}

