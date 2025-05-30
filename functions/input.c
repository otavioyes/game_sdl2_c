// input.c
#include "common.h"


void doKeyDown(SDL_KeyboardEvent *event);
void doKeyUp(SDL_KeyboardEvent *event);
void doMouseDown(SDL_MouseButtonEvent  *event);
void doMouseUp(SDL_MouseButtonEvent *event);



void doInput(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT: exit(0); break;
            case SDL_KEYDOWN: doKeyDown(&event.key); break;
            case SDL_KEYUP: doKeyUp(&event.key); break;
            case SDL_MOUSEBUTTONDOWN:doMouseDown(&event.button); break;
            case SDL_MOUSEBUTTONUP: doMouseUp(&event.button); break;
            default: break;
        }
    }
}

void doKeyDown(SDL_KeyboardEvent *event)
{
    if(event->repeat == 0)
    {
        if(event->keysym.scancode == SDL_SCANCODE_W)
        {
            app.up = 1;
            printf("KEY PRESSED:{%s}\n", SDL_GetScancodeName(event->keysym.scancode));
        }

        if(event->keysym.scancode == SDL_SCANCODE_S)
        {
            app.down = 1;
            printf("KEY PRESSED:{%s}\n", SDL_GetScancodeName(event->keysym.scancode));
        }

        if(event-> keysym.scancode == SDL_SCANCODE_A)
        {
            app.left = 1;
            printf("KEY PRESSED:{%s}\n", SDL_GetScancodeName(event->keysym.scancode));
        }

        if(event-> keysym.scancode == SDL_SCANCODE_D)
        {
            app.right = 1;
            printf("KEY PRESSED:{%s}\n", SDL_GetScancodeName(event->keysym.scancode));
        }
    }
}


void doKeyUp(SDL_KeyboardEvent *event)
{
    if(event->repeat == 0)
    {
        if(event-> keysym.scancode == SDL_SCANCODE_W)
        {
            app.up = 0;
        }
        if(event-> keysym.scancode == SDL_SCANCODE_S)
        {
            app.down = 0;
        }
        if(event-> keysym.scancode == SDL_SCANCODE_A)
        {
            app.left = 0;
        }
        if(event-> keysym.scancode == SDL_SCANCODE_D)
        {
            app.right = 0;
        }

    }
}

void doMouseDown(SDL_MouseButtonEvent  *event)
{
    if(event-> button == SDL_BUTTON_LEFT) { app.fire = 1; printf("BUTTON PRESSED {%d}\n", event-> button); }
}

void doMouseUp(SDL_MouseButtonEvent *event)
{
    if(event-> button == SDL_BUTTON_LEFT) { app.fire = 0; printf("BUTTON UNPRESSED {%d}\n", event-> button); }
}
