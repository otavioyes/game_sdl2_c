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

extern App   app;
extern Stage stage;


/*==============================================================================
 * Inicializa os subsistemas principais da aplicação.
 *
 * Responsabilidades:
 * - Inicializar SDL
 * - Inicializar sistema de áudio
 * - Criar janela principal
 * - Criar renderer
 * - Inicializar SDL_image
 * - Ocultar cursor do mouse
 *============================================================================*/
void initSDL(void)
{
    int rendererFlags;
    int windowFlags;
    int imgFlags;
    int imgInitialized;

    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    /* Inicializa vídeo e timer da SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }

    /*
     * Inicializa sistema de áudio.
     *
     * Parâmetros:
     * - 44100 Hz
     * - formato padrão
     * - stereo
     * - buffer de 1024 samples
     */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        fprintf(stderr, "Mix_OpenAudio failed: %s\n", Mix_GetError());
        SDL_Quit();
        exit(1);
    }

    /* Define quantidade máxima de canais simultâneos de áudio */
    Mix_AllocateChannels(MAX_SND_CHANNELS);

    /* Cria janela principal do jogo */
    app.window = SDL_CreateWindow(
            "Tiro 2D",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            windowFlags
    );

    if (app.window == NULL) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());

        Mix_CloseAudio();
        SDL_Quit();

        exit(1);
    }

    /*
     * Define qualidade de escala das texturas.
     *
     * LINEAR suaviza imagens quando escaladas.
     */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "LINEAR");

    /* Cria renderer acelerado por hardware */
    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (app.renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());

        SDL_DestroyWindow(app.window);
        Mix_CloseAudio();
        SDL_Quit();

        exit(1);
    }

    /*
     * Inicializa suporte a imagens PNG e JPG.
     */
    imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    imgInitialized = IMG_Init(imgFlags);

    if ((imgInitialized & imgFlags) != imgFlags) {
        fprintf(stderr, "IMG_Init failed: %s\n", IMG_GetError());

        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        Mix_CloseAudio();
        SDL_Quit();

        exit(1);
    }

    /* Oculta cursor do mouse durante o jogo */
    SDL_ShowCursor(0);
}


/*==============================================================================
 * Inicializa os sistemas internos do jogo.
 *
 * Responsabilidades:
 * - Inicializar background
 * - Inicializar starfield
 * - Carregar efeitos sonoros
 * - Carregar fonte bitmap
 * - Inicializar tabela de highscores
 * - Limpar estado inicial da fase
 * - Carregar e tocar música principal
 *============================================================================*/
void initGame(void)
{
    initBackground();
    initStarfield();

    initSounds();
    initFonts();

    initHighscoreTable();

    /* Limpa completamente o estado inicial da fase */
    memset(&stage, 0, sizeof(Stage));

    loadMusic("music/Mercury.ogg");
    playMusic(1);
}


/*==============================================================================
 * Libera recursos principais da aplicação.
 *
 * Responsabilidades:
 * - Destruir renderer
 * - Destruir janela
 * - Finalizar SDL_mixer
 * - Finalizar SDL_image
 * - Finalizar SDL
 *============================================================================*/
void cleanup(void)
{
    if (app.renderer != NULL) {
        SDL_DestroyRenderer(app.renderer);
        app.renderer = NULL;
    }

    if (app.window != NULL) {
        SDL_DestroyWindow(app.window);
        app.window = NULL;
    }

    Mix_CloseAudio();
    Mix_Quit();

    IMG_Quit();
    SDL_Quit();
}