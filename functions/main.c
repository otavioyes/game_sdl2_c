// main.c
#include "common.h"

App         app;
Stage       stage;
Entity     *player;
SDL_Texture *bulletTexture;

int main(int argc, char *argv[]) {
    long then      = SDL_GetTicks();
    float remainder = 0.0f;

    memset(&app,   0, sizeof(app));
    memset(&stage, 0, sizeof(stage));

    initSDL();
    atexit(cleanup);
    initStage();

    while (1) {
        prepareScene();
        doInput();
        logic();
        draw();
        presentScene();
        capFrameRate(&then, &remainder);
    }
    return 0;
}

void capFrameRate(long *then, float *remainder) {
    long wait, frameTime;
    wait = 16 + *remainder;
    *remainder -= (int)*remainder;
    frameTime = SDL_GetTicks() - *then;
    wait -= frameTime;
    if (wait < 1) wait = 1;
    SDL_Delay(wait);
    *remainder += 0.667f;
    *then = SDL_GetTicks();
}
