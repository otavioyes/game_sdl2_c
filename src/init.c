// init.c
#include "common.h"

// Inicializa a SDL, cria a janela e o renderizador, e inicializa suporte a imagens
void initSDL(void) {
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;

    // Inicializa o subsistema de vídeo da SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }

    // Cria a janela do aplicativo com o título "Shooter 01"
    app.window = SDL_CreateWindow(
        "Shooter 01",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        windowFlags
    );

    // Verifica se a janela foi criada com sucesso
    if (!app.window) {
        printf("CreateWindow failed: %s\n", SDL_GetError());
        exit(1);
    }

    // Define a dica para suavizar a escala de renderização (melhora qualidade de imagens redimensionadas)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    // Cria o renderizador associado à janela
    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    // Verifica se o renderizador foi criado com sucesso
    if (!app.renderer) {
        printf("CreateRenderer failed: %s\n", SDL_GetError());
        exit(1);
    }

    // Inicializa suporte a imagens PNG e JPG
    int imageFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imageFlags) & imageFlags) != imageFlags) {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        exit(1);
    }
}

// Libera os recursos usados pela SDL antes de encerrar o programa
void cleanup(void) {
    if (app.renderer) SDL_DestroyRenderer(app.renderer); // Destroi o renderizador
    if (app.window) SDL_DestroyWindow(app.window); // Destroi a janela
    IMG_Quit();  // Finaliza o subsistema de imagens
    SDL_Quit(); // Finaliza todos os subsistemas da SDL
}
