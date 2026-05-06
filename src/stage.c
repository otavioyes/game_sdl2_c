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
extern Highscores highscores;
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
static  void doEnemies(void);
static void clipPlayer(void);
static void resetStage(void);
static void drawExplosions(void);
static void doExplosions(void);
static void doDebris(void);
static void drawDebris(void);
static void drawHud(void);
static void doPointsPods(void);
static void drawPointsPods(void);

static void drawPlayerHealthBar(void);

static void addExplosions(int x, int y, int num);
static void addPointsPod(int x, int y);

static int bulletHitFighter(Entity *e);
static void fireAlienBullet(Entity *e);
static void addDebris(Entity *e);
static int canAlienShootPlayer(Entity *e);
//static void drawAlienShootCone(Entity *e);






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

    player->health = PLAYER_MAX_HEALTH;
    player->x = 100;
    player->y = 100;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

    player->side = SIDE_PLAYER;
}


static void logic(void){
    doBackground();
    doStarfield();
    doPlayer();
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

static void doPlayer(void) {
    if (player == NULL) {
        return;
    }

    player->dx = player->dy = 0;

    if (player->reload > 0) {
        player->reload--;
    }

    if (app.keyboard[SDL_SCANCODE_A] || app.keyboard[SDL_SCANCODE_LEFT]) {
        player->dx = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_D] || app.keyboard[SDL_SCANCODE_RIGHT]) {
        player->dx = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_W] || app.keyboard[SDL_SCANCODE_UP]) {
        player->dy = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_DOWN]) {
        player->dy = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload <= 0) {
        playerSound(SND_PLAYER_FIRE, CH_PLAYER);
        fireBullet();
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


//Atualiza as balas e remove as que sairam da tela (lado direito)
static void doBullet(void){
    Entity *b, *prev;
    prev = &stage.bulletHead;

    for (b = stage.bulletHead.next; b != NULL ; b = b->next){
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

    for (e = stage.fighterHead.next; e != NULL; e = e->next){
        if (e->side != b->side && collision(b->x, b->y, b->w,
                                            b->h, e->x, e->y,
                                            e->w, e->h)){
            b->health = 0;
            //e->health--;

            if (e == player) {
                e->health -= 10;

                if (e->health < 0) {
                    e->health = 0;
                }
            } else {
                e->health--;
            }

            addExplosions(e->x, e->y, 32);

            addDebris(e);

            if (e == player){
                playerSound(SND_ALIEN_DIE,CH_PLAYER);
            }
            else{
                addPointsPod(e->x + e->w / 2, e->y + e->h / 2) ;
                playerSound(SND_ALIEN_DIE, CH_ANY);
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


static void clipPlayer(void)
{
    /* Verificação de segurança:
     * Evita acessar memória inválida caso o player ainda não tenha sido inicializado.
     */
    if (player == NULL) {
        return;
    }

    /* Limite esquerdo:
     * Impede o player de sair pela lateral esquerda da tela.
     */
    if (player->x < 0) {
        player->x = 0;
    }

    /* Limite superior:
     * Impede o player de ultrapassar o topo da tela.
     */
    if (player->y < 0) {
        player->y = 0;
    }

    /* Limite direito:
     * Garante que o player permaneça dentro da largura da tela,
     * considerando o tamanho (largura) do sprite.
     * (x + largura não pode ultrapassar SCREEN_WIDTH)
     */
    if (player->x > SCREEN_WIDTH - player->w) {
        player->x = SCREEN_WIDTH - player->w;
    }

    /* Limite inferior:
     * Garante que o player permaneça dentro da altura da tela,
     * considerando o tamanho (altura) do sprite.
     * (y + altura não pode ultrapassar SCREEN_HEIGHT)
     */
    if (player->y > SCREEN_HEIGHT - player->h) {
        player->y = SCREEN_HEIGHT - player->h;
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
    int        i;

    for (i = 0; i < num; i++){
        e = malloc(sizeof (Explosion));
        memset(e, 0, sizeof (Explosion));
        stage.explosionTail->next = e;
        stage.explosionTail = e;

        e->x = x + (rand() % 32) - (rand() % 32);
        e->y = y + (rand() % 32) - (rand() % 32);
        e->dx = (rand() % 10) - (rand() % 10);
        e->dy = (rand() % 10) - (rand() % 10);

        e->dx /= 10;
        e->dy /= 10;

        switch (rand() % 4){
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
        e->a = rand() % FPS * 3;
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


/*static void drawAlienShootCone(Entity *e)
{
    int centerX;
    int centerY;
    int length;

    centerX = e->x + (e->w / 2);
    centerY = e->y + (e->h / 2);
    length = 180;

    /*
     * Cone de tiro do inimigo.
     * O inimigo olha para a esquerda.
     *
     * 135° = diagonal esquerda/cima
     * 225° = diagonal esquerda/baixo
     */
   /* SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);

    SDL_RenderDrawLine(app.renderer,
                       centerX,
                       centerY,
                       centerX + cosf(135.0f * M_PI / 180.0f) * length,
                       centerY + sinf(135.0f * M_PI / 180.0f) * length);

    SDL_RenderDrawLine(app.renderer,
                       centerX,
                       centerY,
                       centerX + cosf(225.0f * M_PI / 180.0f) * length,
                       centerY + sinf(225.0f * M_PI / 180.0f) * length);
}
*/
static void drawPlayerHealthBar(void){
    SDL_Rect bg;
    SDL_Rect segment;

    int i;
    int maxLives = 10;
    int currentLives;
    int segmentWidth = 25;
    int segmentHeight = 20;
    int gap = 4;

    if (player == NULL) {
        return;
    }

    currentLives = player->health / 10;

    bg.x = 10;
    bg.y = 50;
    bg.w = (segmentWidth * maxLives) + (gap * (maxLives - 1));
    bg.h = segmentHeight;

    /* Fundo geral da barra */
    SDL_SetRenderDrawColor(app.renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(app.renderer, &bg);

    for (i = 0; i < maxLives; i++) {
        segment.x = bg.x + (i * (segmentWidth + gap));
        segment.y = bg.y;
        segment.w = segmentWidth;
        segment.h = segmentHeight;

        if (i < currentLives) {
            /* Bloco com vida */
            SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(app.renderer, &segment);
        } else {
            /* Bloco vazio */
            SDL_SetRenderDrawColor(app.renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(app.renderer, &segment);
        }

        /* Borda de cada bloco */
        SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(app.renderer, &segment);
    }
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
        blit(e->texture, e->x, e->y);

        /*if (e != player) {
            drawAlienShootCone(e);
        }*/
    }
}


static void drawBullets(void){
    Entity *b;
    for (b = stage.bulletHead.next; b != NULL; b = b->next){
        blit(b->texture, b->x, b->y);
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