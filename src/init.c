/*
 * init.c
 *
 * Responsável pela inicialização e finalização
 * dos sistemas principais da aplicação.
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

/*
 * Inicializa os subsistemas principais da SDL,
 * incluindo:
 *
 * - vídeo
 * - timer
 * - renderer
 * - janela
 * - SDL_image
 * - SDL_mixer
 */
void initSDL(void)
{
    int rendererFlags;
    int windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    /*
     * Inicializa os módulos básicos da SDL.
     */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }

    /*
     * Inicializa o sistema de áudio.
     */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        fprintf(stderr, "Mix_OpenAudio failed: %s\n", Mix_GetError());
        exit(1);
    }

    /*
     * Define quantidade máxima de canais simultâneos.
     */
    Mix_AllocateChannels(MAX_SND_CHANNELS);

    /*
     * Cria a janela principal da aplicação.
     */
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

    /*
     * Define qualidade de escala para texturas.
     *
     * LINEAR suaviza imagens quando ampliadas.
     */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "LINEAR");

    /*
     * Cria o renderer responsável pelo desenho na tela.
     */
    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());

        SDL_DestroyWindow(app.window);
        SDL_Quit();

        exit(1);
    }

    /*
     * Inicializa suporte a imagens PNG e JPG.
     * Verifica erro.
     */
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
        fprintf(stderr, "IMG_Init failed: %s\n", IMG_GetError());
        exit(1);
    }

    /*
     * Oculta cursor do mouse durante o jogo.
     */
    SDL_ShowCursor(0);
}

/*
 * Inicializa os sistemas do jogo.
 *
 * Aqui são carregados:
 * - background
 * - starfield
 * - áudio
 * - fontes
 * - highscores
 * - música principal
 */
void initGame(void)
{
    initBackground();
    initStarfield();

    initSounds();
    initFonts();

    initHighscoreTable();

    /*
     * Limpa completamente o estado atual da fase.
     */
    memset(&stage, 0, sizeof(Stage));

    loadMusic("music/Mercury.ogg");
    playMusic(1);
}

/*
 * Libera os recursos utilizados pela aplicação.
 *
 * Deve ser executado ao encerrar o jogo.
 */
void cleanup(void)
{
    SDL_Quit();
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}