// init.c
#include "common.h"

void initSDL(void) {
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (!app.window) {
        printf("CreateWindow failed: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer) {
        printf("CreateRenderer failed: %s\n", SDL_GetError());
        exit(1);
    }

    int imageFlags = IMG_INIT_PNG | IMG_INIT_JPG;

    if ((IMG_Init(imageFlags) & imageFlags) != imageFlags){
        printf("IMG_Init failed: %s\n", IMG_GetError());
        exit(1);
    }
}

void cleanup(void) {
    if (app.renderer) SDL_DestroyRenderer(app.renderer);
    if (app.window) SDL_DestroyWindow(app.window);
    SDL_Quit();
}
