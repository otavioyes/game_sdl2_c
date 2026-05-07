/*
 * player.c
 */

#include "common.h"


extern Stage stage;

void initPlayer(SDL_Texture *texture);

static Entity       *player;
static SDL_Texture  *playerTexture;



void initPlayer(SDL_Texture *texture) {
    player = malloc(sizeof(Entity));

    if (player == NULL) {
        SDL_Log("malloc falhou em initPlayer");
        exit(1);
    }

    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;



    player->health = PLAYER_MAX_HEALTH;
    player->x = 100;
    player->y = 100;
    player->texture = texture;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

    player->side = SIDE_PLAYER;
}