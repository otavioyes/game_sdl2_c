//init.c

#include "common.h"

void initSDL(void)
{
    int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    int windowFlags   = SDL_WINDOW_SHOWN;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow(
        "Shooter 01",
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
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);
    if (!app.renderer) {
        fprintf(stderr, "CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    /* habilita blending alpha por padrão (importante para sprites PNG) */
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);

    /* usa tamanho lógico para manter coordenadas consistentes */
    SDL_RenderSetLogicalSize(app.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    /* Inicializa suporte a PNG (mais seguro pedir só o que usa) */
    int imageFlags = IMG_INIT_PNG;
    if ((IMG_Init(imageFlags) & imageFlags) != imageFlags) {
        fprintf(stderr, "IMG_Init failed (PNG): %s\n", IMG_GetError());
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    /* seed do RNG para spawns aleatorios */
    srand((unsigned)time(NULL));
}


/* liberação de recursos - DEFINIÇÃO GLOBAL (não static!) */
void cleanup(void)
{
    if (app.renderer) {
        SDL_DestroyRenderer(app.renderer);
        app.renderer = NULL;
    }
    if (app.window) {
        SDL_DestroyWindow(app.window);
        app.window = NULL;
    }

    IMG_Quit();
    SDL_Quit();
}
