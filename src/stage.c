/*
 * stage.c
 */

#include "common.h"

#include "entity.h"
#include "background.h"
#include "draw.h"
#include "highscore.h"
#include "stage.h"

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "effects.h"
#include "points.h"
#include "hud.h"

extern App app;
extern Stage stage;

static void logic(void);
static void draw(void);
static void resetStage(void);

static int          stageResetTimer;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *alienBulletTexture;
static SDL_Texture *playerTexture;
static SDL_Texture *explosionTexture;
static SDL_Texture *pointsTexture;

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    bulletTexture      = loadTexture("assets/gfx/playerBullet.png");
    enemyTexture       = loadTexture("assets/gfx/enemy.png");
    alienBulletTexture = loadTexture("assets/gfx/alienBullet.png");
    playerTexture      = loadTexture("assets/gfx/player1.png");
    explosionTexture   = loadTexture("assets/gfx/explosion.png");
    pointsTexture      = loadTexture("assets/gfx/points.png");

    if (!bulletTexture || !enemyTexture || !alienBulletTexture ||
        !playerTexture || !explosionTexture || !pointsTexture) {
        SDL_Log("Erro ao carregar uma ou mais texturas da fase");
        exit(1);
    }

    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    resetStage();

    stage.score = 0;

    initPlayer(playerTexture);

    stageResetTimer = FPS * 3;
}

static void resetStage(void)
{
    Entity    *e;
    Explosion *ex;
    Debris    *d;

    while (stage.entityHead.next) {
        e = stage.entityHead.next;
        stage.entityHead.next = e->next;
        free(e);
    }

    while (stage.bulletHead.next) {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        free(e);
    }

    while (stage.explosionHead.next) {
        ex = stage.explosionHead.next;
        stage.explosionHead.next = ex->next;
        free(ex);
    }

    while (stage.debrisHead.next) {
        d = stage.debrisHead.next;
        stage.debrisHead.next = d->next;
        free(d);
    }

    while (stage.pointsHead.next) {
        e = stage.pointsHead.next;
        stage.pointsHead.next = e->next;
        free(e);
    }

    memset(&stage, 0, sizeof(Stage));

    stage.entityTail    = &stage.entityHead;
    stage.bulletTail    = &stage.bulletHead;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail    = &stage.debrisHead;
    stage.pointsTail    = &stage.pointsHead;

    initEnemies();
}

static void logic(void)
{
    doBackground();
    doStarfield();

    doPlayer(bulletTexture);
    doEnemies(alienBulletTexture);

    doEntities();

    doBullet(pointsTexture);

    doExplosions();
    doDebris();
    doPointsPods();

    spawnEnemies(enemyTexture);
    clipPlayer();

    if (player == NULL && --stageResetTimer <= 0) {
        addHighscore(stage.score);
        initHighscores();
    }
}

static void draw(void)
{
    drawBackground();
    drawStarfield();

    drawPointsPods();
    drawEntities();

    drawDebris();
    drawExplosions(app.renderer, explosionTexture);
    drawBullets();

    drawHud();
}