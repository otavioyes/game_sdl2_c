//input.c

#include "common.h"

void doKeyDown(SDL_KeyboardEvent *event);
void doKeyUp(SDL_KeyboardEvent *event);
void doMouseDown(SDL_MouseButtonEvent *event);
void doMouseUp(SDL_MouseButtonEvent *event);

void doInput(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);

            case SDL_KEYDOWN:
                doKeyDown(&event.key);
                break;

            case SDL_KEYUP:
                doKeyUp(&event.key);
                break;

            case SDL_MOUSEBUTTONDOWN:
                doMouseDown(&event.button);
                break;

            case SDL_MOUSEBUTTONUP:
                doMouseUp(&event.button);
                break;

            default:
                break;
        }
    }
}


void doKeyDown(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 1;
        printf("KEY PRESSED {%s}\n",
               SDL_GetScancodeName(event->keysym.scancode));
    }
}

void doKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 0;
        printf("KEY UNPRESSED {%s}\n", SDL_GetScancodeName(event->keysym.scancode));
    }
}


void doMouseDown(SDL_MouseButtonEvent *event)
{
    if(event->button < MAX_MOUSE_BUTTONS)
    {
        app.mouse[event->button] = 1;
        printf("MOUSE BUTTON PRESSED {%d}\n", event->button);
    }
}

void doMouseUp(SDL_MouseButtonEvent *event)
{
    if(event->button < MAX_MOUSE_BUTTONS)
    {
        app.mouse[event->button] = 0;
        printf("MOUSE BUTTON UNPRESSED {%d}\n", event->button);
    }
}

