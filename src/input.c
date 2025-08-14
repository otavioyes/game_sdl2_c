//input.c

#include "common.h"

// Declaração das funções de tratamento de entrada
void doKeyDown(SDL_KeyboardEvent *event);
void doKeyUp(SDL_KeyboardEvent *event);
void doMouseDown(SDL_MouseButtonEvent *event);
void doMouseUp(SDL_MouseButtonEvent *event);

// Função principal de entrada; processa todos os eventos da SDL (teclado, mouse, saída, etc.)
void doInput(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            // Evento de fechamento da janela
            case SDL_QUIT:
                exit(0);

            // Tecla pressionada
            case SDL_KEYDOWN:
                doKeyDown(&event.key);
                break;

            // Tecla liberada
            case SDL_KEYUP:
                doKeyUp(&event.key);
                break;

            // Botao do mouse pressioando
            case SDL_MOUSEBUTTONDOWN:
                doMouseDown(&event.button);
                break;

            // Botao do mouse liberado
            case SDL_MOUSEBUTTONUP:
                doMouseUp(&event.button);
                break;

            default:
                break; // Outros eventos sao ignorados.
        }
    }
}

// Função chamada quando uma tecla é pressionada
void doKeyDown(SDL_KeyboardEvent *event)
{
    int sc = (int) event->keysym.scancode;            /* cast seguro do enum para int */
    if (event->repeat == 0 && sc >= 0 && sc < (int)MAX_KEYBOARD_KEYS)
    {
        app.keyboard[sc] = 1; // Marca tecla como pressionada
        printf("KEY PRESSED {%s}\n", SDL_GetScancodeName(event->keysym.scancode));
    }
}

// Função chamada quando uma tecla é liberada
void doKeyUp(SDL_KeyboardEvent *event)
{
    int sc = (int) event->keysym.scancode;            /* cast seguro do enum para int */
    if (event->repeat == 0 && sc >= 0 && sc < (int)MAX_KEYBOARD_KEYS)
    {
        app.keyboard[sc] = 0; // Marca o teclado como liberado
        printf("KEY UNPRESSED {%s}\n", SDL_GetScancodeName(event->keysym.scancode));
    }
}



// Função chamada quando um botão do mouse é pressionado
void doMouseDown(SDL_MouseButtonEvent *event)
{
    if(event->button < MAX_MOUSE_BUTTONS)
    {
        app.mouse[event->button] = 1;// Marca botao como pressioando
        printf("MOUSE BUTTON PRESSED {%d}\n", event->button);
    }
}

// Função chamada quando um botão do mouse é liberado
void doMouseUp(SDL_MouseButtonEvent *event)
{
    if(event->button < MAX_MOUSE_BUTTONS)
    {
        app.mouse[event->button] = 0;// Marca o botao como como liberado
        printf("MOUSE BUTTON UNPRESSED {%d}\n", event->button); // Log
    }
}


