/*
 * player.c
 */

#include "common.h"


extern Stage    stage;

static void initPlayer(void);

static SDL_Texture  *playerTexture;
static Entity       *player;



static void initPlayer(void) {
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    if (player == NULL) {
        SDL_Log("malloc falhou em initPlayer");
        exit(1);
    }

    player->health = PLAYER_MAX_HEALTH;
    player->x = 100;
    player->y = 100;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

    player->side = SIDE_PLAYER;
}