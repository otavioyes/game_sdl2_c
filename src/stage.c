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
    bullet->scale = 5;
    bullet->health = 1;
    bullet->texture = alienBulletTexture;
    bullet->side = SIDE_ALIEN;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->width, &bullet->height);

    bullet->x += (e->width / 2) - (bullet->width / 2);
    bullet->y += (e->height / 2) - (bullet->height / 2);

    calcSlop(player->x + (player->width / 2),
             player->y + (player->height / 2),
             e->x,
             e->y,
             &bullet->dx,
             &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    e->reload = (rand() % FPS *2);
}




// Gera inimigos periodicamente e adiciona à lista de lutadores
static void spawnsEnemies(void)
{
    Entity *enemy;

    // Decrementa o temporizador e verifica se é hora de criar um inimigo
    if(--enemySpawnTimer <=0)
    {
        enemy = malloc(sizeof(Entity)); //aloca memoria para um novo inimigo
        memset(enemy, 0, sizeof(Entity)); //zera os dados da estrutura

        // Adiciona o inimigo ao final da lista ligada de lutadores
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->scale = 1; // define o tamnho visual do inimigo(scale)

        enemy->x = SCREEN_WIDTH; //Comeca fora da tela a direita
        enemy->y = rand() % SCREEN_HEIGHT;//coordenada Y e escolhida aleatoriamente com base em SCREEN_HEIGHT
        enemy->texture = enemyTexture; // usa textura do inimigo
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->width, &enemy->height); //define largura e altura original

        enemy->dx = -(2 + (rand() % 4));//velocidade para a esquerda negativa
        enemy->dy = rand() %5 - 2; //Movimentacao do eixo Y eleatoria
        enemySpawnTimer = 20  + (rand() % 60); //taxa de geracao do inimigo
        enemy->reload = FPS * (1 + (rand() % 3));

        enemy->side = SIDE_ALIEN;

        enemy->health = 3;
    }
}


static void clipPlayer(void)
{
    if (player == NULL) return;

    // Usa largura/altura * scale para clipping consistente
    float w = player->width * player->scale;
    float h = player->height * player->scale;

    if (player->x < 0) player->x = 0;
    if (player->y < 0) player->y = 0;
    if (player->x > SCREEN_WIDTH  - w) player->x = SCREEN_WIDTH  - w;
    if (player->y > SCREEN_HEIGHT - h) player->y = SCREEN_HEIGHT - h;
}





//Atualiza os lutadores e remove os que saíram da tela
static void doFighters(void)
{
    Entity *e = stage.fighterHead.next;
    Entity *prev = &stage.fighterHead; // começo no cabeçalho da lista de lutadores

    while (e != NULL)
    {
        // Atualiza posição com base na velocidade
        e->x += e->dx;
        e->y += e->dy;

        // Restringe o lutador ao limite inferior da tela (corrigido e usando altura)
        if (e->y + e->height * e->scale > SCREEN_HEIGHT)
        {
            e->y = SCREEN_HEIGHT - e->height * e->scale;
        }

        // Se o lutador não for o player e saiu da tela pela esquerda, marca vida como 0
        if (e != player && e->x < -e->width)
        {
            e->health = 0;
        }

        // Se a vida for 0, trata remoção e colisão
        if (e->health == 0)
        {
            if (e == player)
            {
                player = NULL;
            }

            if (player && e != player && checkCollisionEntities(e, player))
            {
                e->health = 0; // inimigo morre ao colidir com jogador
                // player->health -= 1; // opcional: reduzir vida do jogador
                SDL_Log("Inimigo colidiu com o jogador");
            }
        }

        // Restringe limite superior da tela
        if (e->y < 0)
        {
            e->y = 0;
        }

        // Condição para remover lutadores que saíram da tela à esquerda ou estão mortos
        if (e != player && ((e->x + e->width * e->scale) < 0 || e->health <= 0))
        {
            SDL_Log("Inimigo removido! Vida = %d", e->health);

            // Se for o último da lista, atualiza ponteiro do final
            if (e == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            prev->next = e->next; // remove da lista

            Entity *toFree = e; // salva ponteiro para liberar depois
            e = e->next;        // avança antes de liberar

            free(toFree);       // libera memória

            continue;           // pula o avanço de prev para manter a lista consistente
        }

        // Avança ponteiros normalmente
        prev = e;
        e = e->next;
    }
}


// Função que testa se a bala `b` atingiu algum inimigo
static int bulletHitFighter(Entity *b)
{
    Entity *e; // Ponteiro para percorrer a lista de fighters

    // Para cada fighter na lista encadeada de inimigos…
    for(e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if(e->side != b->side && checkCollisionEntities(b, e))
        {
            b->health = 0; // Marca a bala para a remocao
            e->health -= 1; // Marca o inimigo para a remocao
            return 1; // Marca um "hit" (colisao bem sucedida)
        }
    }
    return 0; //Nenhua colisao
}


//Atualiza as balas e remove as que sairam da tela (lado direito)
static void doBullet(void)
{
    Entity *b, *prev;

    prev = &stage.bulletHead; // Ponteiro anterior, começa no cabeçalho da lista de balas

    //Percorre a lista de balas
    for (b = stage.bulletHead.next ; b != NULL ; b = b->next)
    {
        b->x    += b->dx; //atualiza a posicao X da bala
        b->y    += b->dy; //atualiza a posicao y da bala

        if (bulletHitFighter(b) || b->x < -b->width ||
           b->y < -b->height || b->x > SCREEN_WIDTH ||
           b->y > SCREEN_HEIGHT )//Se a bala passou do limite direito da tela
        {
            //Se a bala era ultima da lista, atualiza o ponteiro
            if (b == stage.bulletTail)
            {
                stage.bulletTail = prev;
            }

            prev->next = b->next; //Remove a bala da lista
            free(b); //Libera a memoria da bala
            b = prev; //Volta a uma posição da lista para manter o loop seguro
        }

        prev = b;//Avanca o ponteiro anterior
    }
}




