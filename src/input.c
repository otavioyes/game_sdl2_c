/*
 * input.c
 */

#include "common.h"

#include "input.h"

extern App app;


/*==============================================================================
 * Processa evento de tecla solta.
 *
 * Responsabilidades:
 * - Ignorar eventos repetidos
 * - Validar limite do array de teclas
 * - Marcar tecla como liberada
 *============================================================================*/
static void doKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 &&
        event->keysym.scancode < MAX_KEYBOARD_KEYS) {

        app.keyboard[event->keysym.scancode] = 0;
    }
}


/*==============================================================================
 * Processa evento de tecla pressionada.
 *
 * Responsabilidades:
 * - Ignorar eventos repetidos
 * - Validar limite do array de teclas
 * - Marcar tecla como pressionada
 *============================================================================*/
static void doKeyDown(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 &&
        event->keysym.scancode < MAX_KEYBOARD_KEYS) {

        app.keyboard[event->keysym.scancode] = 1;
    }
}


/*==============================================================================
 * Processa todos os eventos de entrada do frame atual.
 *
 * Responsabilidades:
 * - Limpar texto digitado no início do frame
 * - Processar fechamento da janela
 * - Processar teclas pressionadas
 * - Processar teclas soltas
 * - Capturar texto digitado via SDL_TEXTINPUT
 *============================================================================*/
void doInput(void)
{
    SDL_Event event;

    /*
     * Limpa o buffer de texto a cada frame.
     *
     * app.inputText representa apenas o texto digitado
     * no frame atual, não o histórico completo.
     */
    memset(app.inputText, '\0', MAX_LINE_LENGTH);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
                doKeyDown(&event.key);
                break;

            case SDL_KEYUP:
                doKeyUp(&event.key);
                break;

            case SDL_TEXTINPUT:
                /*
                 * Armazena texto digitado neste frame.
                 *
                 * Usado principalmente por telas que precisam
                 * de entrada textual, como o highscore.
                 */
            STRNCPY(app.inputText, event.text.text, MAX_LINE_LENGTH);
                break;

            default:
                break;
        }
    }

    /*
     * Funcao recebe 2 valores referencias que armazena
     * o estado do mouse e as coodernadas de x y
     */
    SDL_GetMouseState(&app.mouse.x, &app.mouse.y);
}