/*
 *background.c
 */

#include "common.h"

#include "background.h"
#include "draw.h"

extern App app;

static int          backgroundX;
static Star         star[MAX_STARS];
static SDL_Texture  *background;

void initBackground(void) {
    background = loadTexture("assets/gfx/background.png");
    backgroundX = 0;
}

void initStarfield(void) {
    int i;
    for (i = 0; i < MAX_STARS; i++) {
        star[i].x = rand() % SCREEN_WIDTH;
        star[i].y = rand() % SCREEN_HEIGHT;
        star[i].speed = 1 + rand() % 8;
    }
}

void doBackground(void) {
    if (--backgroundX < -SCREEN_WIDTH) {
        backgroundX = 0;
    }
}

void doStarfield(void) {
    int i;
    for (i = 0; i < MAX_STARS; i++) {
        star[i].x -= star[i].speed;
        if (star[i].x < 0) {
            star[i].x = SCREEN_WIDTH + star[i].x;
        }
    }
}

void drawStarfield(void) {
    int i, c;
    for (i = 0; i < MAX_STARS; i++) {
        c = 32 * star[i].speed;
        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);
        SDL_RenderDrawLine(app.renderer, star[i].x, star[i].y, star[i].x + 3, star[i].y);
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
void drawBackground(void) {
    SDL_Rect dest;

    int x;

    /*
     * Define modulação de cor da textura.
     *
     * RGB(255, 255, 255): AQUI ALTERO A COR DO background.png
     * mantém cores originais da imagem.
     */
    SDL_SetTextureColorMod(background,
                           255,
                           255,
                           255);

    /*
     * Renderiza múltiplas cópias do background
     * para criar efeito contínuo de movimentação.
     */
    for (x = backgroundX;
         x < SCREEN_WIDTH;
         x += SCREEN_WIDTH) {

        /* Define área de destino da renderização */
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        /* Renderiza textura do background */
        SDL_RenderCopy(app.renderer,
                       background,
                       NULL,
                       &dest);
    }
}
