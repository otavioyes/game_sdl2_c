/*
 * main.c
 *
 * DESCOBRIR DE ONDE ESTA SAINDO A MIRA DO MOUSE
 * Continuar tutorial da SDL2 
 */

#include "common.h"

#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"
#include "title.h"

App        app;
Highscores highscores;
Stage      stage;

static void capFrameRate(long *then, float *remainder);

int main(int argc, char *argv[])
{
    long then;
    float remainder;

    (void) argc;
    (void) argv;

    memset(&app, 0, sizeof(app));

    app.textureTail = &app.textureHead;

    initSDL();

    atexit(cleanup);

    initGame();

    initTitle();

    then = SDL_GetTicks();
    remainder = 0;

    while (1) {
        prepareScene();

        doInput();

        app.delegate.logic();

        app.delegate.draw();

        presentScene();

        capFrameRate(&then, &remainder);
    }

    return 0;
}

static void capFrameRate(long *then, float *remainder)
{
    long wait;
    long frameTime;

    /*
     * Tempo base aproximado para 60 FPS.
     * 1000 / 60 = 16.667ms
     */
    wait = 16 + *remainder;

    /*
     * Remove a parte inteira acumulada,
     * mantendo apenas a fração restante.
     */
    *remainder -= (int) *remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1) {
        wait = 1;
    }

    SDL_Delay(wait);

    /*
     * Compensa a diferença entre 16ms
     * e 16.667ms para manter média próxima de 60 FPS.
     */
    *remainder += 0.667f;

    *then = SDL_GetTicks();
}