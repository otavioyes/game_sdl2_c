//stage.c
/*
#include "common.h"

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    initPlayer();

    bulletTexture = loadTexture("gfx/playerBullet.png");

}
/*
A função initStage configura uma série de coisas para jogar o jogo.
Ela atribui a lógica do delegado e os ponteiros de desenho a duas
funções estáticas de lógica e desenho próprias, e também prepara
as listas vinculadas de lutadores e balas para uso. Em seguida, ela
chama a função initPlayer . Este é mais ou menos o mesmo código
que tínhamos em main.c , exceto que o objeto jogador agora está
mallocado e adicionado à lista vinculada de lutadores. Também
definimos w e h da entidade jogador (sua largura e altura) com base
no tamanho da textura, chamando SDL_QueryTexture . Também pré-
armazenamos a imagem da bala em bulletTexture , para que não
fiquemos carregando isso (e desperdiçando memória) sempre que
criarmos uma bala.
*/
/*
static void initPlayer()
{
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = 100;
    player->y = 100;
    player->texture = loadTexture("gfx/player1.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->width, &player->height);
}


static void logic(void)
{
    doPlayer();
    doBUllet();
}


static void doPlayer(void)
{
    player->dx = player->dy = 0;

    if(player->reload > 0)
    {
        player->reload--;
    }

    if(app.keyboard[SDL_SCANCODE_UP])
    {
        player->dy = -PLAYER_SPEED;
    }

    if(app.keyboard[SDL_SCANCODE_DOWN])
    {
        player->dy = PLAYER_SPEED;
    }

    if(app.keyboard[SDL_SCANCODE_LEFT])
    {
        player->dx = -PLAYER_SPEED;
    }

    if(app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player->dx = -PLAYER_SPEED;
    }

    if(app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player->dx = PLAYER_SPEED;
    }

    if(app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
    {
        fireBullet();
    }

    player->x += player->dx;
    player->y += player->dy;
}

static void fireBullet(void)
{
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = player->x;
    bullet->y = player->y;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->width, &bullet->height);

    bullet->y += (player->height / 2) - (bullet->height / 2);

    player->reload = 8;
}


static void doBullet(void)
{

    Entity *b, *prev;

    prev = &stage.bulletHead;

    for(b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        b->x += b->dx;
        b->y += b->dy;

        if(b->x > SCREEN_WIDTH)
        {
            if(b == stage.bulletTail)
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
*/



#include "common.h"

// protótipos internos
static void initPlayer(void);
static void doPlayer(void);
static void doBullet(void);
static void fireBullet(void);

void initStage(void) {
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    memset(&stage, 0, sizeof(stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail  = &stage.bulletHead;

    initPlayer();
    bulletTexture = loadTexture("gfx/playerBullet.png");
}

static void initPlayer(void) {
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x       = 100;
    player->y       = 100;
    player->texture = loadTexture("gfx/player1.png");
    SDL_QueryTexture(player->texture, NULL, NULL,
                     &player->width, &player->height);
}

void logic(void) {
    doPlayer();
    doBullet();
}

static void doPlayer(void) {
    // …
    if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
        fireBullet();
    // …
}

static void doBullet(void) {
    // …
}

static void fireBullet(void) {
    // …
}
