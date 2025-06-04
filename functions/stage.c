//stage.c
#include "common.h"

// protótipos internos
static void initPlayer(void);
static void logic(void);
static void doPlayer(void);
static void doBullet(void);
static void fireBullet(void);

//novos prototipos sendo declarados para a criação de inimigos
static void doFighters(void);
static void spawnsEnemies(void);

static int enemySpawnTimer;

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    memset(&stage, 0, sizeof(stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail  = &stage.bulletHead;

    initPlayer();

    bulletTexture = loadTexture("gfx/playerBullet.png");
    enemyTexture = loadTexture("gfx/enemy.png");

    enemySpawnTimer = 0;


}

/*Iinicia o player com localizacao e tamanho*/
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


/*Inicializando o inimigo */
static void spawnEnemies(void)
{
    Entity *enemy;

    if(--enemySpawnTimer <=0)
    {
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->scale = 0.1f;

        enemy->x = SCREEN_WIDTH;

        //coordenada Y e escolhida aleatoriamente com base em SCREEN_HEIGHT
        enemy->y = rand() % SCREEN_HEIGHT;
        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->width, &enemy->height);

        enemy->dx = -(2 + (rand() % 4));

        enemySpawnTimer = 30 + (rand() % 60);
    }
}

static void logic(void)
{
    doPlayer();
    doFighters();
    doBullet();
    spawnEnemies();

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
}



//funcao que lida com todos os lutadores da tela
static void doFighters(void)
{
    Entity *e, *prev;

    prev = &stage.fighterHead;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if(e != player && (e->x +e->width < 0))
        {
            if(e == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
    }
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
