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
    memset(player, 0, sizeof(Entity));//inicializando os campos da strut com 0 S/lixo de mem.
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x       = 100; //posicao X onde o player nasce
    player->y       = 100; //posicao Y onde o player nasce
    player->scale   = 0.1f;//tamanho da imagem /scale
    player->texture = loadTexture("gfx/player1.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->width, &player->height);
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

    if (app.keyboard[SDL_SCANCODE_W])
    {
        player->dy = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_S])
    {
        player->dy = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_A])
    {
        player->dx = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_D])
    {
        player->dx = PLAYER_SPEED;
    }

    if (app.mouse[SDL_BUTTON(SDL_BUTTON_LEFT)] && player->reload == 0)
    {
        fireBullet();
    }

    //restigindo o jogador aos limites da tela
    if(player->x + player->width * player->scale > SCREEN_WIDTH)
    {
        player->x = SCREEN_WIDTH - player->width * player->scale;
    }


    if(player->x < 0)
    {
        player->x = 0;
    }

    if(player->y + player->height * player->scale > SCREEN_HEIGHT)
    {
        player->y = SCREEN_HEIGHT - player->width * player->scale;
    }

    if(player->y < 0)
    {
        player->y = 0;
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
        b->x    += b->dx;
        b->y    += b->dy;

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


//funcao que cria a bala
static void fireBullet(void)
{
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->scale   = 0.1f; //tamanho da bala

    bullet->texture = bulletTexture;
    //pego primeiro o tamanho orignal da bala
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->width, &bullet->height);

    bullet->x   =   player->x + (player->width * player->scale / 2) - (bullet->x * bullet->scale / 2);
    bullet->y   =   player->y + (player->height * player->scale / 2) - (bullet->y * bullet->scale / 2);

    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;

    player->reload = 8;
}
