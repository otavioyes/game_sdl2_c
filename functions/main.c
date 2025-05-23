// main.c
#include "common.h"

App app;

int main(int argc, char *argv[]) {
    memset(&app, 0, sizeof(App));

    initSDL();
    atexit(cleanup);

    while (1) {
        doInput();
        prepareScene();
        presentScene();
        SDL_Delay(16);  // ~60 FPS
    }

    return 0;
}
