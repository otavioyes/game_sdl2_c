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

//NOVOS ARQUIVOS
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "effects.h"
#include "points.h"
#include "hud.h"

extern App app;
extern Highscores highscores;
extern Stage stage;

static void logic(void);
static void draw(void);
static void doFighters(void);
static void drawFighters(void);
static void resetStage(void);


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
    initPlayer(playerTexture);
    initEnemies();
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

    initEnemies();
}



static void logic(void){
    doBackground();
    doStarfield();
    doPlayer(bulletTexture);
    doEnemies(alienBulletTexture);
    doFighters();
    doBullet(pointsTexture);
    doExplosions();
    doDebris();
    doPointsPods();
    spawnsEnemies(enemyTexture);
    clipPlayer();

    if (player == NULL && --stageResetTimer <= 0){
        addHighscore(stage.score);
        initHighscores();
    }
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


void addExplosions(int x, int y, int num){
    Explosion *e;
    int i;

    for (i = 0; i < num; i++) {
        e = malloc(sizeof(Explosion));

        if (e == NULL) {
            SDL_Log("malloc falhou em addExplosions");
            exit(1);
        }

        memset(e, 0, sizeof(Explosion));

        stage.explosionTail->next = e;
        stage.explosionTail = e;

        /* Espalhamento menor */
        e->x = x + (rand() % 8) - (rand() % 8);
        e->y = y + (rand() % 8) - (rand() % 8);

        /* Movimento mais lento */
        e->dx = (rand() % 6) - (rand() % 6);
        e->dy = (rand() % 6) - (rand() % 6);

        e->dx /= 10;
        e->dy /= 10;

        switch (rand() % 4) {
            case 0:
                e->r = 255;
                break;

            case 1:
                e->r = 255;
                e->g = 128;
                break;

            case 2:
                e->r = 255;
                e->g = 255;
                break;

            default:
                e->r = 255;
                e->g = 255;
                e->b = 255;
                break;
        }

        /* Duração menor da explosão */
        e->a = 40 + (rand() % 60);
    }
}


void addDebris(Entity *e){
    Debris *d;
    int     x, y, w, h;

    w = e->w / 2;
    h = e->h / 2;

    for (y = 0; y <= h; y += h){
        for (x = 0; x <= w; x += w){
            d = malloc(sizeof (Debris ));
            memset(d, 0, sizeof(Debris));
            stage.debrisTail->next = d;
            stage.debrisTail = d;

            d->x = e->x + e->w / 2;
            d->y = e->y + e->h / 2;
            d->dx = (rand() % 5) - (rand() % 5);
            d->dy = -(5 + (rand() % 12));
            d->life = FPS * 2;

            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }
    }
}



static void draw(void){
    drawBackground();
    drawStarfield();
    drawPointsPods();
    drawFighters();
    drawDebris();
    drawExplosions(app.renderer, explosionTexture);
    drawBullets();
    drawHud();

}



static void drawFighters(void){
    Entity *e;
    for (e = stage.fighterHead.next; e != NULL; e = e->next){
        if (e == player) {
            blitRotated(e->texture, e->x, e->y, e->angle);
        } else {
            blit(e->texture, e->x, e->y);
        }

        /*if (e != player) {
            drawAlienShootCone(e);
        }*/
    }
}
