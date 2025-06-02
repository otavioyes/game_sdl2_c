//stage.c
#include "common.h"

// protótipos internos
static void initPlayer(void);
static void doPlayer(void);
static void doBullet(void);
static void fireBullet(void);


void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    memset(&stage, 0, sizeof(stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail  = &stage.bulletHead;

    initPlayer();
    bulletTexture = loadTexture("gfx/playerBullet.png");
}

static void initPlayer(void)
{
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x       = 100; //posicao X onde o player nasce
    player->y       = 100; //posicao Y onde o player nasce
    player->scale   = 0.1f;//tamanho da imagem /scale
    player->texture = loadTexture("gfx/player1.png");
    SDL_QueryTexture(player->texture, NULL, NULL,
                     &player->width, &player->height);
}


void logic(void)
{
    doPlayer();
    doBullet();
}

//funcao de  movimentacao do jogador
static void doPlayer(void)
{
    player->dx = player->dy = 0;

    if (player->reload > 0)
    {
        player->reload--;
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        player->dy = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        player->dy = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_LEFT])
    {
        player->dx = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player->dx = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
    {
        fireBullet();
    }

    player->x += player->dx;
    player->y += player->dy;
}


static void doBullet(void)
{
    Entity *b, *prev;

    prev = &stage.bulletHead;

    for (b = stage.bulletHead.next ; b != NULL ; b = b->next)
    {
        b->x     += b->dx;
        b->y     += b->dy;
        b->scale += 0.1f;

        if (b->x > SCREEN_WIDTH)
        {
            if (b == stage.bulletTail)
            {
                stage.bulletTail = prev;
            }

            prev->next = b->next;
            free(b);
            b = prev;
        }

        prev = b;
    }
}


//funcao que chama valocidade das balas e faz atirar
static void fireBullet(void)
{
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x       = player->x;
    bullet->y       = player->y;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->width, &bullet->height);

    bullet->y += (player->height / 2) - (bullet->height / 2);

    player->reload = 8;
}
