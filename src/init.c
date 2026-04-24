/*
 * init.c
 */

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "common.h"

#include "background.h"
#include "highscore.h"
#include "init.h"
#include "sound.h"
#include "text.h"

extern App app;
extern Stage stage;

void initSDL(void) {
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2,1024) == -1) {
        fprintf(stderr, "Mix_OpenAudio failed: %s\n", Mix_GetError());
        exit(1);
    }
    Mix_AllocateChannels(MAX_SND_CHANNELS);


    app.window = SDL_CreateWindow(
        "Tiro 2D",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        windowFlags
    );
    if (!app.window) {
        fprintf(stderr, "CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }



    /* hint para qualidade de escala - coloque antes do renderer se quiser */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "LINEAR");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    SDL_ShowCursor(0);
}

void initGame(void) {
    initBackground();
    initStarfield();
    initSound(); /*add sound.c*/
    initFonts();
    initHighscoreTable();
    memset(&stage, 0, sizeof(Stage));
    loadMusic("music/Mercury.ogg");
    playerMusic(1);
}

void cleanupGame(void) {
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}