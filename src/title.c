/*
 * title.c
 */

#include "common.h"

#include "background.h"
#include "draw.h"
#include "highscore.h"
#include "stage.h"
#include "text.h"
#include "title.h"

extern App app;

static void logic(void);
static void draw(void);
static void drawLogo(void);

static SDL_Texture *sdl2Texture;
static SDL_Texture *shooterTexture;

static int reveal;
static int timeout;


/*==============================================================================
 * Inicializa a tela de título do jogo.
 *
 * Responsabilidades:
 * - Definir delegates de lógica e renderização
 * - Limpar estado atual do teclado
 * - Carregar texturas da tela inicial
 * - Inicializar animação de revelação do logo
 * - Configurar tempo automático até a tela de highscores
 *============================================================================*/
void initTitle(void)
{
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    sdl2Texture = loadTexture("assets/gfx/sdl2.png");
    shooterTexture = loadTexture("assets/gfx/shooter.png");

    if (sdl2Texture == NULL || shooterTexture == NULL) {
        SDL_Log("Erro ao carregar texturas da tela de título");
        exit(1);
    }

    reveal = 0;
    timeout = FPS * 5;
}


/*==============================================================================
 * Atualiza a lógica da tela de título.
 *
 * Responsabilidades:
 * - Atualizar background e starfield
 * - Controlar animação de revelação do logo
 * - Ir automaticamente para highscores após timeout
 * - Iniciar fase quando ENTER for pressionado
 *============================================================================*/
static void logic(void)
{
    doBackground();
    doStarfield();

    /*
     * Aumenta gradualmente a área visível dos logos,
     * criando efeito simples de revelação vertical.
     */
    if (reveal < SCREEN_HEIGHT) {
        reveal++;
    }

    /*
     * Após alguns segundos sem input,
     * exibe a tela de highscores automaticamente.
     */
    if (--timeout <= 0) {
        initHighscores();
        return;
    }

    /*
     * ENTER inicia imediatamente a fase principal.
     */
    if (app.keyboard[SDL_SCANCODE_RETURN]) {
        app.keyboard[SDL_SCANCODE_RETURN] = 0;
        initStage();
    }
}


/*==============================================================================
 * Renderiza a tela de título.
 *
 * Responsabilidades:
 * - Desenhar background
 * - Desenhar starfield
 * - Desenhar logos
 * - Exibir instrução piscante para iniciar
 *============================================================================*/
static void draw(void)
{
    drawBackground();
    drawStarfield();
    drawLogo();

    /*
     * Texto piscante para orientar o jogador.
     */
    if (timeout % 40 < 20) {
        drawText(SCREEN_WIDTH / 2,
                 600,
                 255,
                 255,
                 255,
                 TEXT_CENTER,
                 "PRESSIONE ENTER PARA INICIAR");
    }
}


/*==============================================================================
 * Renderiza os logos da tela inicial.
 *
 * Responsabilidades:
 * - Obter dimensões das texturas
 * - Aplicar recorte vertical progressivo
 * - Centralizar logos horizontalmente
 * - Criar efeito visual de revelação
 *============================================================================*/
static void drawLogo(void)
{
    SDL_Rect r;

    /*
     * Renderiza logo SDL2 com recorte vertical baseado
     * no valor atual de reveal.
     */
    r.x = 0;
    r.y = 0;

    SDL_QueryTexture(sdl2Texture, NULL, NULL, &r.w, &r.h);

    r.h = MIN(reveal, r.h);

    blitRect(sdl2Texture,
             &r,
             (SCREEN_WIDTH / 2) - (r.w / 2),
             100);

    /*
     * Renderiza logo principal do jogo com o mesmo
     * efeito de revelação vertical.
     */
    r.x = 0;
    r.y = 0;

    SDL_QueryTexture(shooterTexture, NULL, NULL, &r.w, &r.h);

    r.h = MIN(reveal, r.h);

    blitRect(shooterTexture,
             &r,
             (SCREEN_WIDTH / 2) - (r.w / 2),
             250);
}