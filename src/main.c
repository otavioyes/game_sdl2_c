/*
 * main.c
 */

/*
 * PRECISA AJUSTAR AS BALAS
 * RODA NO MAC - 14/06/206
 */

#include "common.h"

#include "draw.h"
#include "init.h"
#include "input.h"
#include "main.h"
#include "title.h"


App          app;
Highscores   highscores;
Stage        stage;


/*==============================================================================
 * Limita taxa de atualização do jogo (frame rate cap).
 *
 * Responsabilidades:
 * - Controlar tempo entre frames
 * - Manter FPS estável
 * - Compensar erro fracionário de tempo
 * - Evitar consumo excessivo de CPU
 *============================================================================*/
static void capFrameRate(long *then, float *remainder);



/*==============================================================================
 * Função principal da aplicação.
 *
 * Responsabilidades:
 * - Inicializar sistemas principais
 * - Configurar SDL
 * - Inicializar estados do jogo
 * - Executar game loop principal
 * - Atualizar lógica e renderização
 * - Controlar frame rate
 *============================================================================*/
int main(int argc, char *argv[])
{
    long then;

    float remainder;

    /* Marca parâmetros como intencionalmente não utilizados */
    (void)argc;
    (void)argv;

    /* Inicializa estrutura global da aplicação */
    memset(&app, 0, sizeof(app));

    /* Inicializa ponteiro final da lista de texturas */
    app.textureTail = &app.textureHead;

    /* Inicializa SDL e subsistemas do jogo */
    initSDL();

    /* Registra função de limpeza automática ao encerrar aplicação */
    atexit(cleanup);

    /* Inicializa recursos principais do jogo */
    initGame();

    /* Inicializa tela de título */
    initTitle();

    /* Captura tempo inicial */
    then = SDL_GetTicks();

    /* Inicializa compensação fracionária */
    remainder = 0;

    /*==============================================================================
     * Game Loop Principal
     *============================================================================*/
    while (1) {

        /* Prepara frame atual */
        prepareScene();

        /* Processa entrada do usuário */
        doInput();

        /* Atualiza lógica do estado atual */
        app.delegate.logic();

        /* Renderiza frame atual */
        app.delegate.draw();

        /* Apresenta frame renderizado na tela */
        presentScene();

        /* Mantém taxa de FPS estável */
        capFrameRate(&then, &remainder);
    }

    return 0;
}



/*==============================================================================
 * Controla limite de FPS da aplicação.
 *
 * Responsabilidades:
 * - Calcular duração do frame atual
 * - Aplicar atraso necessário
 * - Corrigir erro acumulado de tempo
 * - Manter sincronização estável do loop
 *============================================================================*/
static void capFrameRate(long *then, float *remainder)
{
    long wait;

    long frameTime;

    /*
     * Tempo alvo para ~60 FPS.
     *
     * 16ms ≈ 60 FPS
     */
    wait = 16 + *remainder;

    /*
     * Remove parte inteira já utilizada
     * da compensação fracionária.
     */
    *remainder -= (int)*remainder;

    /* Calcula duração do frame atual */
    frameTime = SDL_GetTicks() - *then;

    /* Ajusta tempo restante necessário */
    wait -= frameTime;

    /*
     * Garante tempo mínimo de espera
     * para evitar uso excessivo de CPU.
     */
    if (wait < 1) {
        wait = 1;
    }

    /* Suspende execução temporariamente */
    SDL_Delay(wait);

    /*
     * Compensa diferença de precisão
     * entre 16ms e 16.667ms (60 FPS real).
     */
    *remainder += 0.667f;

    /* Atualiza timestamp do frame atual */
    *then = SDL_GetTicks();
}
