/*
 * player.c
 */

#include "common.h"

#include "player.h"
#include "sound.h"


extern Stage stage;
extern App app;
extern void fireBullet(void);


Entity *player;



/*==============================================================================
 * Inicializa a entidade do jogador.
 *
 * Responsabilidades:
 * - Alocar memória para o player
 * - Inserir o player na lista de entidades da fase
 * - Configurar atributos iniciais
 * - Associar a textura do jogador
 * - Obter largura e altura da textura
 *============================================================================*/
void initPlayer(SDL_Texture *texture) {
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



/*==============================================================================
 * Atualiza a lógica do jogador.
 *
 * Responsabilidades:
 * - Processar entrada do teclado
 * - Controlar movimentação
 * - Controlar tempo de recarga do tiro
 * - Disparar projéteis
 * - Atualizar rotação da nave
 *============================================================================*/
void doPlayer(void) {
    /* Garante que o jogador exista antes de processar lógica */
    if (player == NULL) {
        return;
    }

    /* Reinicia velocidade do jogador a cada frame */
    player->dx = player->dy = 0;

    /* Atualiza temporizador de recarga do tiro */
    if (player->reload > 0) {
        player->reload--;
    }

    /* Movimento horizontal para esquerda */
    if (app.keyboard[SDL_SCANCODE_A] || app.keyboard[SDL_SCANCODE_LEFT]) {
        player->dx = -PLAYER_SPEED;
    }

    /* Movimento horizontal para direita */
    if (app.keyboard[SDL_SCANCODE_D] || app.keyboard[SDL_SCANCODE_RIGHT]) {
        player->dx = PLAYER_SPEED;
    }

    /* Movimento vertical para cima */
    if (app.keyboard[SDL_SCANCODE_W] || app.keyboard[SDL_SCANCODE_UP]) {
        player->dy = -PLAYER_SPEED;
    }

    /* Movimento vertical para baixo */
    if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_DOWN]) {
        player->dy = PLAYER_SPEED;
    }

    /* Dispara projétil caso a recarga esteja disponível */
    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload <= 0) {
        playerSound(SND_PLAYER_FIRE, CH_PLAYER);
        fireBullet();
    }

    /* Rotaciona nave para esquerda */
    if (app.keyboard[SDL_SCANCODE_Q]) {
        player->angle -= 4;
    }

    /* Rotaciona nave para direita */
    if (app.keyboard[SDL_SCANCODE_E]) {
        player->angle += 4;
    }
}



/*==============================================================================
 * Mantém o jogador dentro dos limites visíveis da tela.
 *
 * Responsabilidades:
 * - Validar existência da entidade do jogador
 * - Restringir movimentação horizontal
 * - Restringir movimentação vertical
 * - Garantir que o sprite permaneça totalmente visível
 *============================================================================*/
void clipPlayer(void) {
    /* Garante que o jogador exista antes de aplicar limites */
    if (player == NULL) {
        return;
    }

    /* Limite esquerdo da tela */
    if (player->x < 0) {
        player->x = 0;
    }

    /* Limite superior da tela */
    if (player->y < 0) {
        player->y = 0;
    }

    /*
     * Limite direito da tela.
     * Subtrai a largura do sprite para evitar que o jogador
     * ultrapasse parcialmente a área visível.
     */
    if (player->x > SCREEN_WIDTH - player->w) {
        player->x = SCREEN_WIDTH - player->w;
    }

    /*
     * Limite inferior da tela.
     * Subtrai a altura do sprite para manter o jogador
     * completamente visível dentro da janela.
     */
    if (player->y > SCREEN_HEIGHT - player->h) {
        player->y = SCREEN_HEIGHT - player->h;
    }
}