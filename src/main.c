/*
 * main.c
 */


#include "common.h"

#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"
#include "title.h"

App         app;
Highscore   highscore;
Stage stage;

static void capFrameRate(long *then, float *remainder);

int main(int argc, char *argv[])
{

    long then;
    float remainder;

    memset(&app,   0, sizeof(app));
    app.textureTail = &app.textureHead;

    initSDL();
    atexit(cleanup);
    initStage();

    while (1)
{
        prepareScene();
        doInput();
        app.delegate.logic();
        app.delegate.draw();
        presentScene();
        capFrameRate(&then, &remainder);
    }
    return 0;
}

void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime;
    wait = 16 + *remainder;
    *remainder -= (int)*remainder;
    frameTime = SDL_GetTicks() - *then;
    wait -= frameTime;

    if (wait < 1){
        wait = 1;
    }

    SDL_Delay(wait);
    *remainder += 0.667f;
    *then = SDL_GetTicks();
}
