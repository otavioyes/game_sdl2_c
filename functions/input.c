// input.c
#include "common.h"

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
            printf("Tecla pressionada:{%s}\n", SDL_GetScancodeName(event->keysym.scancode));
        }

        if(event->keysym.scancode == SDL_SCANCODE_S)
        {
            app.down = 1;
            printf("Tecla pressionada:{%s}\n", SDL_GetScancodeName(event->keysym.scancode));
        }

        if(event-> keysym.scancode == SDL_SCANCODE_A)
        {
            app.left = 1;
            printf("Tecla pressionada:{%s}\n", SDL_GetScancodeName(event->keysym.scancode));
        }

        if(event-> keysym.scancode == SDL_SCANCODE_D)
        {
            app.right = 1;
            printf("Tecla pressionada:{%s}\n", SDL_GetScancodeName(event->keysym.scancode));
        }
        if(event-> keysym.scancode == SDL_SCANCODE_LCTRL)
        {
            app.fire = 1;
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
        if(event-> keysym.scancode == SDL_SCANCODE_LCTRL)
        {
            app.fire = 0;
        }

    }
}

