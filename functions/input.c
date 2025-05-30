//input.c

#include "common.h"

void doKeyDown(SDL_KeyboardEvent *event);
void doKeyUp(SDL_KeyboardEvent *event);

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
            default:
                break;
        }
    }
}

void doKeyUp(SDL_KeyboardEvent *event) {
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
        app.keyboard[event->keysym.scancode] = 0;
        printf("BUTTON UNPRESSED {%s}\n",
               SDL_GetScancodeName(event->keysym.scancode));
    }
}

void doKeyDown(SDL_KeyboardEvent *event) {
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
        app.keyboard[event->keysym.scancode] = 1;
        printf("BUTTON PRESSED {%s}\n",
               SDL_GetScancodeName(event->keysym.scancode));
    }
}
