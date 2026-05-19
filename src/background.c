/*
 * background.c
 */

#include "common.h"

#include "background.h"
#include "draw.h"

extern App app;

static int         backgroundX;
static Star        star[MAX_STARS];
static SDL_Texture *background;


/*==============================================================================
 * Inicializa o background principal da fase.
 *
 * Responsabilidades:
 * - Carregar textura de fundo
 * - Validar carregamento da textura
 * - Reiniciar posição horizontal do scrolling
 *============================================================================*/
void initBackground(void)
{
    background = loadTexture("assets/gfx/background.png");

    if (background == NULL) {
        SDL_Log("Erro ao carregar textura do background em initBackground");
        exit(1);
    }

    backgroundX = 0;
}


/*==============================================================================
 * Inicializa o campo de estrelas.
 *
 * Responsabilidades:
 * - Distribuir estrelas aleatoriamente pela tela
 * - Definir velocidades variadas
 * - Criar sensação inicial de profundidade espacial
 *============================================================================*/
void initStarfield(void)
{
    int i;

    for (i = 0; i < MAX_STARS; i++) {
        star[i].x = rand() % SCREEN_WIDTH;
        star[i].y = rand() % SCREEN_HEIGHT;

        /*
         * Velocidades diferentes criam sensação de profundidade:
         * estrelas mais rápidas parecem estar mais próximas.
         */
        star[i].speed = 1 + rand() % 8;
    }
}


/*==============================================================================
 * Atualiza o deslocamento horizontal do background.
 *
 * Responsabilidades:
 * - Mover background para esquerda
 * - Reiniciar posição ao completar um ciclo
 * - Criar scrolling contínuo da textura
 *============================================================================*/
void doBackground(void)
{
    backgroundX--;

    if (backgroundX < -SCREEN_WIDTH) {
        backgroundX = 0;
    }
}


/*==============================================================================
 * Atualiza o campo de estrelas.
 *
 * Responsabilidades:
 * - Mover estrelas para esquerda
 * - Reposicionar estrelas que saem da tela
 * - Manter efeito contínuo de movimento espacial
 *============================================================================*/
void doStarfield(void)
{
    int i;

    for (i = 0; i < MAX_STARS; i++) {
        star[i].x -= star[i].speed;

        /*
         * Quando a estrela sai pela esquerda,
         * ela retorna pelo lado direito mantendo continuidade.
         */
        if (star[i].x < 0) {
            star[i].x = SCREEN_WIDTH + star[i].x;
            star[i].y = rand() % SCREEN_HEIGHT;
        }
    }
}


/*==============================================================================
 * Renderiza o campo de estrelas.
 *
 * Responsabilidades:
 * - Definir brilho baseado na velocidade
 * - Renderizar pequenas linhas horizontais
 * - Reforçar sensação de velocidade e profundidade
 *============================================================================*/
void drawStarfield(void)
{
    int i;
    int c;

    for (i = 0; i < MAX_STARS; i++) {

        /*
         * Estrelas mais rápidas ficam mais claras,
         * simulando camadas próximas ao observador.
         */
        c = 32 * star[i].speed;

        if (c > 255) {
            c = 255;
        }

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

        SDL_RenderDrawLine(app.renderer,
                           star[i].x,
                           star[i].y,
                           star[i].x + 3,
                           star[i].y);
    }
}


/*==============================================================================
 * Renderiza o background da fase.
 *
 * Responsabilidades:
 * - Aplicar modulação de cor na textura
 * - Renderizar background em loop horizontal
 * - Preencher toda área visível da tela
 * - Criar efeito contínuo de scrolling
 *============================================================================*/
void drawBackground(void)
{
    SDL_Rect dest;
    int      x;

    if (background == NULL) {
        return;
    }

    /*
     * Mantém cores originais da textura.
     *
     * Alterar esses valores permite aplicar tint/color grading
     * no background sem modificar o arquivo original.
     */
    SDL_SetTextureColorMod(background, 255, 255, 255);

    /*
     * Renderiza múltiplas cópias do background para criar
     * continuidade durante o scrolling horizontal.
     */
    for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH) {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }
}