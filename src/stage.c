/*
 * stage.c
 *
 * Controla o estado principal da fase.
 *
 * Este módulo coordena a lógica geral do gameplay:
 * - inicialização da fase
 * - atualização dos sistemas
 * - renderização da fase
 * - reset das entidades
 * - transição para a tela de highscores
 *
 * A lógica específica de jogador, inimigos, tiros, efeitos,
 * pontos e HUD fica delegada para módulos próprios.
 */

#include "common.h"

#include "entity.h"

#include "background.h"
#include "draw.h"
#include "highscore.h"
#include "sound.h"
#include "stage.h"
#include "text.h"
#include "util.h"

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

static void resetStage(void);

static int          stageResetTimer;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *alienBulletTexture;
static SDL_Texture *playerTexture;
static SDL_Texture *explosionTexture;
static SDL_Texture *pointsTexture;
/*IMPLEMENTANDO*/
static SDL_Texture *targetterTexture;
static SDL_Texture *donkTexture;


/*
 * Inicializa a fase principal do jogo.
 *
 * Define os delegates de lógica e desenho, carrega as texturas
 * necessárias e prepara o estado inicial das entidades.
 */
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
    /*IMPLEMENTANDO*/
    targetterTexture   = loadTexture("assests/gxf/aindaNaoTenhoImagem.png");
    donkTexture        = loadTexture("assets/gfx/aindaNaoTenhoDonk.png");


    if (!bulletTexture || !enemyTexture || !alienBulletTexture ||
        !playerTexture || !explosionTexture || !pointsTexture ||
        !targetterTexture || !donkTexture) {
        SDL_Log("Erro ao carregar uma ou mais texturas da fase");
        exit(1);
    }

    /*
     * Limpa o estado atual do teclado para evitar que teclas
     * pressionadas em telas anteriores afetem o início da fase.
     */
    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    resetStage();

    stage.score = 0;

    initPlayer(playerTexture);

    stageResetTimer = FPS * 3;
}

/*
 * Limpa todas as listas encadeadas da fase.
 *
 * Remove entidades, tiros, explosões, destroços e pontos
 * antes de reinicializar os ponteiros de cabeça/cauda.
 */
static void resetStage(void)
{
    Entity    *e;
    Explosion *ex;
    Debris    *d;


    while (stage.fighterHead.next) {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
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

    /*
     * Reinicia completamente o estado da fase após liberar
     * todos os recursos dinâmicos associados às listas.
     */
    memset(&stage, 0, sizeof(Stage));

    /*unificando entidades - migracao futura*/
    stage.entityTail    = &stage.entityHead;

    stage.fighterTail   = &stage.fighterHead;
    stage.bulletTail    = &stage.bulletHead;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail    = &stage.debrisHead;
    stage.pointsTail    = &stage.pointsHead;

    initEnemies();
}

/*
 * Atualiza todos os sistemas ativos da fase.
 *
 * A função funciona como orquestradora do gameplay,
 * chamando módulos especializados em ordem controlada.
 */
static void logic(void)
{
    doBackground();
    doStarfield();

    doPlayer(bulletTexture);

    doEnemies(alienBulletTexture);

    doFighters();
    doBullet(pointsTexture);

    doExplosions();
    doDebris();
    doPointsPods();

    spawnEnemies(enemyTexture);
    clipPlayer();

    /*
     * Quando o jogador morre, aguarda alguns segundos
     * antes de registrar a pontuação e ir para a tela de highscores.
     */
    if (player == NULL && --stageResetTimer <= 0) {
        addHighscore(stage.score);
        initHighscores();
    }
}


/*
 * Renderiza todos os elementos visuais da fase.
 *
 * A ordem de desenho é importante:
 * - fundo
 * - elementos de gameplay
 * - efeitos
 * - HUD
 */
static void draw(void)
{
    drawBackground();
    drawStarfield();

    drawEntities();

    drawDebris();
    drawExplosions(app.renderer, explosionTexture);
    drawBullets();

    drawHud();

    blit(targetterTexture, app.mouse.x, app.mouse.y, 1);

}
