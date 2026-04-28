/*
 * stage.c
 */

#include "common.h"

#include "background.h"
#include "draw.h"
#include "highscore.h"
#include "sound.h"
#include "stage.h"
#include "text.h"
#include "util.h"

extern App app;
extern Highscore highscore;
extern Stage stage;

static void logic(void);
static void draw(void);
static void initPlayer(void);
static void fireBullet(void);
static void doPlayer(void);
static void doFighters(void);
static void doBullet(void);
static void drawFighters(void);
static void drawBullets(void);
static void spawnsEnemies(void);
static  void doEnimies(void);
static void clipPlayer(void);
static void resetStage(void);
static void drawExplosions(void);
static void doExplosions(void);
static void doDebris(void);
static void drawDebris(void);
static void drawHud(void);
static void doPointsPods(void);
static void drawPointsPods(void);

static void addExplosions(int x, int y, int num);
static void addPointsPod(int x, int y);

static int bulletHitFighter(Entity *e);
static void fireAlienBullet(Entity *e);
static void addDebris(Entity *e);
static

static Entity       *player;
static int          enemySpawnTimer;
static int          stageResetTimer;
static SDL_Texture  *bulletTexture;
static SDL_Texture  *enemyTexture;
static SDL_Texture  *alienBulletTexture;
static SDL_Texture  *playerTexture;
static SDL_Texture  *explosionTexture;
static SDL_Texture  *pointsTexture;


void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    // Carrega texturas primeiro (uma só vez)
    bulletTexture        = loadTexture("assets/gfx/playerBullet.png");
    enemyTexture         = loadTexture("assets/gfx/enemy.png");
    alienBulletTexture   = loadTexture("assets/gfx/alienBullet.png");
    playerTexture        = loadTexture("assets/gfx/player1.png");
    explosionTexture     = loadTexture("assets/gfx/explosion.png");
    pointsTexture        = loadTexture("assets/gfx/points.png");

    if (playerTexture == NULL){
        SDL_Log("Erro ao carregar texturas FUNCAO => initStage");
        exit(1);
    }

    memset(app.keyboard, 0, sizeof(int ) * MAX_KEYBOARD_KEYS);

    resetStage();
    stage.score = 0;
    initPlayer();
    enemySpawnTimer = 0;
    stageResetTimer = FPS * 3;
}


static void resetStage(void){
    Entity     *e;
    Explosion  *ex;
    Debris     *d;

    while (stage.fighterHead.next){
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        free(e);
    }

    while (stage.bulletHead.next){
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        free(e);
    }

    while (stage.explosionHead.next){
        ex = stage.explosionHead.next;
        stage.explosionHead.next = ex->next;
        free(ex);
    }

    while (stage.debrisHead.next){
        d = stage.debrisHead.next;
        stage.debrisHead.next = d->next;
        free(d);
    }

    while (stage.pointsHead.next){
        e = stage.pointsHead.next;
        stage.pointsHead.next = e->next;
        free(e);
    }

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;
    stage.pointsTail = &stage.pointsHead;

}


static void initPlayer(void) {
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    if (player == NULL) {
        SDL_Log("malloc falhou em initPlayer");
        exit(1);
    }

    player->health = 1;
    player->x = 100;
    player->y = 100;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

    player->side = SIDE_PLAYER;
}


static void logic(void)
{
    doBackground();
    doStarfield();
    doPlayer();
    doEnimies();
    doFighters();
    doBullet();
    doBullet();
    doExplosions();
    doDebris();
    doPointsPods();
    spawnsEnemies();
    clipPlayer();

    if (player == NULL && --stageResetTimer <= 0){
        addHighscore(stage.score);
        initHighscore();
    }
}

static void doPlayer(void) {
    if (player == NULL) {
        player->dx = player->dy = 0;

        if (player->reload > 0) {
            player->reload--;
        }

        if (app.keyboard[SDL_SCANCODE_UP])    { player->dy = -PLAYER_SPEED; }
        if (app.keyboard[SDL_SCANCODE_DOWN])  { player->dy = PLAYER_SPEED; }
        if (app.keyboard[SDL_SCANCODE_RIGHT]) { player->dx = -PLAYER_SPEED; }
        if (app.keyboard[SDL_SCANCODE_LEFT])  { player->dx = PLAYER_SPEED; }

        if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload <= 0) {
            playerSound(SND_PLAYER_FIRE, CH_PLAYER);
            fireBullet();
        }
    }
}


// Função que cria uma nova bala (disparo do jogador)
static void fireBullet(void){
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = player->x;
    bullet->y = player->y;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->texture = bulletTexture;
    bullet->side = player->side;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->y += (player->h / 2) - (bullet->h / 2);

    bullet->side = SIDE_PLAYER;

    player->reload = 8;
}

static void doEnemies(void){
    Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next){
        if (e != player){
            e->y = MIN(MAX(e->y, 0), SCREEN_HEIGHT - e->h);
            if (player != NULL && --e->reload <= 0){
                fireAlienBullet(e);
                playerSound(SND_PLAYER_FIRE, CH_ALIEN_FIRE);
            }
        }
    }
}


static void fireAlienBullet(Entity *e){
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->texture = alienBulletTexture;
    bullet->side = SIDE_ALIEN;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    calcSlop(player->x + (player->w / 2),
             player->y + (player->h / 2),
             e->x, e->y, &bullet->dx, &bullet->dy);

        bullet->dx *= ALIEN_BULLET_SPEED;
        bullet->dy *= ALIEN_BULLET_SPEED;

        e->reload = (rand() % FPS * 2);

}

//Atualiza os lutadores e remove os que saíram da tela
static void doFighters(void) {
    Entity *e, *prev;
    prev = &stage.fighterHead;

    for (e = stage.fighterHead.next; e != NULL; e = e->next) {
        e->x += e->dx;
        e->y += e->dy;

        if (e != player && e->x < -e->w) {
            e->health = 0;
        }
        if (e->health == 0) {
            if (e == player) {
                player = NULL;
            }
            if (e == stage.fighterTail){
                stage.fighterTail = prev;
            }
            prev->next = e->next;
            free(e);
            e = prev;
        }
        prev = e;
    }
}


//Atualiza as balas e remove as que sairam da tela (lado direito)
static void doBullet(void){
    Entity *b, *prev;
    prev = &stage.bulletHead;

    for (b = stage.bulletHead.next ; b != NULL ; b = b->next){
        b->x    += b->dx;
        b->y    += b->dy;

        if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT ){
            if (b == stage.bulletTail){
                stage.bulletTail = prev;
            }

            prev->next = b->next; /*Remove a bala da lista*/
            free(b); /*Libera a memoria da bala*/
            b = prev; /*Volta a uma posição da lista para manter o loop seguro*/
        }
        prev = b; /*Avanca o ponteiro anterior*/
    }
}


static int bulletHitFighter(Entity *b){
    Entity *e;

    for (e = stage.fighterHead.next; e != NULL, e = e->next){
        if (e->side != b->side && collision(b->x, b->y, b->w,
                                            b->h, e->x, e->y,
                                            e->w, e->h)){
            b->health = 0;
            e->health = 0;

            addExplosions(e->x, e->y, 32);

            addDebris(e);

            if (e == player){
                playerSound(SND_ALINE_DIE,CH_PLAYER);
            }
            else{
                addPointsPod(e->x + e->w / 2, e->y + e->h / 2) ;
                playerSound(SND_ALINE_DIE, CH_ANY);
            }
            return 1;
        }
    }
    return  0;
}


// Gera inimigos periodicamente e adiciona à lista de lutadores
static void spawnsEnemies(void){
    Entity *enemy;

    if(--enemySpawnTimer <=0){
        enemy = malloc(sizeof(Entity));
        memset(enemy, 0, sizeof(Entity));
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % SCREEN_HEIGHT;
        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

        enemy->dx = -(2 + (rand() % 4));
        enemy->dy = -100 + rand() % 200;
        enemy->dy /= 100;
        enemy->side = SIDE_ALIEN;
        enemy->health = 1;
        enemy->reload = FPS * (1 + (rand() % 3));

        enemySpawnTimer = 30  + (rand() % FPS); //taxa de geracao do inimigo
    }
}


static void clipPlayer(void){
    if (player == NULL){

        if (player->x < 0){
            player->x = 0;
        }

        if (player->y < 0){
            player->y = 0;
        }

        if (player->x > SCREEN_WIDTH - player->w){
            player->x = SCREEN_WIDTH - player->w;
        }

        if (player->h > SCREEN_HEIGHT - player->h){
            player->h = SCREEN_HEIGHT - player->h;
        }
    }
}

static void doExplosions(void){
    Explosion *e, *prev;

    prev = &stage.explosionHead;

    for (e = stage.explosionHead.next; e != NULL; e = e->next){
        e->x += e->dx;
        e->y += e->dy;

        if (--e->a <= 0){
            if (e == stage.explosionTail){
                stage.explosionTail = prev;
            }
            prev->next = e->next;
            free(e);
            e = prev;
        }
        prev = e;
    }
}

static void doDebris(void){
    Debris *d, *prev;

    prev = &stage.debrisHead;

    for (d = stage.debrisHead.next; d != NULL; d = d->next){
        d->x += d->dx;
        d->y += d->dy;

        d->dy += 0.5;

        if (--d->life <= 0){
            if (d == stage.debrisTail){
                stage.debrisTail = prev;
            }
            prev->next = d->next;
            free(d);
            d = prev;
        }
        prev = d;
    }
}







