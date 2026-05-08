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

extern App app;
extern Highscores highscores;
extern Stage stage;

static void logic(void);
static void draw(void);

static void doFighters(void);

/*
static void doBullet(void);
 */

static void drawFighters(void);
static void drawBullets(void);
static void spawnsEnemies(void);
static void doEnemies(void);

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


static void fireAlienBullet(Entity *e);
static void addDebris(Entity *e);
static int canAlienShootPlayer(Entity *e);


//static Entity       *player;
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
    initPlayer(playerTexture);
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



static void logic(void){
    doBackground();
    doStarfield();
    doPlayer(bulletTexture);
    doEnemies();
    doFighters();
    doBullet();
    doExplosions();
    doDebris();
    doPointsPods();
    spawnsEnemies();
    clipPlayer();

    if (player == NULL && --stageResetTimer <= 0){
        addHighscore(stage.score);
        initHighscores();
    }
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
/*calcular o ângulo entre “para onde o inimigo olha” e “onde o player está”.*/
static int canAlienShootPlayer(Entity *e)
{
    float enemyCenterX, enemyCenterY;
    float playerCenterX, playerCenterY;
    float dx, dy;
    float length;
    float dot;
    float cosAngle;

    if (player == NULL) {
        return 0;
    }

    enemyCenterX = e->x + (e->w / 2);
    enemyCenterY = e->y + (e->h / 2);

    playerCenterX = player->x + (player->w / 2);
    playerCenterY = player->y + (player->h / 2);

    dx = playerCenterX - enemyCenterX;
    dy = playerCenterY - enemyCenterY;

    length = sqrtf(dx * dx + dy * dy);

    if (length == 0) {
        return 1;
    }

    /*
     * Direção frontal do inimigo: esquerda.
     * Vetor forward = (-1, 0)
     */
    dot = (dx * -1.0f) + (dy * 0.0f);

    cosAngle = dot / length;

    /*
     * Permite tiro até 135 graus.
     * cos(135°) = -0.7071
     *
     * Se passar de 135°, o inimigo para de atirar.
     */
    return cosAngle >= -0.0f;
}


static void fireAlienBullet(Entity *e)
{
    Entity *bullet;

    if (!canAlienShootPlayer(e)) {
        return;
    }

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
             e->x + (e->w / 2),
             e->y + (e->h / 2),
             &bullet->dx, &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    e->reload = rand() % (FPS * 2);
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


static void doPointsPods(void){
    Entity *e, *prev;

    prev = &stage.pointsHead;

    for (e = stage.pointsHead.next; e != NULL; e = e->next){
        if (e->x < 0){
            e->x = 0;
            e->dx = -e->dx;
        }

        if (e->x + e->w > SCREEN_WIDTH){
            e->x = SCREEN_WIDTH - e->w;
            e->dx = -e->dx;
        }

        if (e->y < 0){
            e->y = 0;
            e->dy = -e->dy;
        }

        if (e->y + e->h > SCREEN_HEIGHT){
            e->y = SCREEN_HEIGHT - e->y;
            e->dy = -e->dy;
        }
        e->x += e->dx;
        e->y += e->dy;

        if (player != NULL && collision(e->x, e->y, e->w, e->h, player->x, player->y, player->w, player->h)){
            e->health = 0;
            stage.score++;
            playerSound(SND_POINTS, CH_POINTS);
        }

        if (--e->health <= 0){
            if (e == stage.pointsTail){
                stage.pointsTail = prev;
            }
            prev->next = e->next;
            free(e);
            e = prev;
        }
        prev = e;
    }
}


static void addExplosions(int x, int y, int num){
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


static void addDebris(Entity *e){
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

static void addPointsPod(int x, int y){
    Entity *e;

    e = malloc(sizeof(Entity));
    memset(e, 0, sizeof(Entity));
    stage.pointsTail->next = e;
    stage.pointsTail = e;

    e->x = x;
    e->y = y;
    e->dx = -(rand() % 5);
    e->dy = (rand() % 5) - (rand() %  5);
    e->health = FPS * 10;
    e->texture = pointsTexture;

    SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);

    e->x -= e->w / 2;
    e->y -= e->h / 2;
}



static void draw(void){
    drawBackground();
    drawStarfield();
    drawPointsPods();
    drawFighters();
    drawDebris();
    drawExplosions();
    drawBullets();
    drawHud();

}


static void drawPointsPods(void){
    Entity  *e;
    for (e = stage.pointsHead.next; e != NULL; e = e->next){
        if (e->health > (FPS * 2) || e->health % 12 < 6){
            blit(e->texture, e->x, e->y);
        }
    }
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


static void drawBullets(void){
    Entity *b;
    for (b = stage.bulletHead.next; b != NULL; b = b->next){
        if (b->side == SIDE_PLAYER) {
            blitRotated(b->texture, b->x, b->y, b->angle);
        } else {
            blit(b->texture, b->x, b->y);
        }
    }
}


static void drawDebris(void){
    Debris *d;
    for (d = stage.debrisHead.next; d != NULL; d = d->next){
        blit(d->texture, d->x, d->y);
    }
}

static void drawExplosions(void){
    Explosion *e;

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureAlphaMod(explosionTexture, SDL_BLENDMODE_ADD);

    for (e = stage.explosionHead.next; e != NULL; e = e->next){
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
        SDL_SetTextureAlphaMod(explosionTexture, e->a);
        blit(explosionTexture, e->x, e->y);
    }
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

static void drawHud(void){
    drawText(10, 10, 255, 255, 255, TEXT_LEFT, "SCORE: %03d", stage.score);

    drawPlayerHealthBar();

    if (stage.score < highscores.highscore[0].score){
        drawText(SCREEN_WIDTH - 10, 10, 255, 255, 255, TEXT_RIGHT, "HIGHSCORE: %03d", highscores.highscore[0].score);
    }
    else{
        drawText(SCREEN_WIDTH -10, 10, 0, 255, 0, TEXT_RIGHT, "HIGHSCORE: %03d", stage.score);
    }
}