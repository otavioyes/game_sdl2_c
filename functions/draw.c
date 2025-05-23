// draw.c
#include "common.h"

void prepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 10, 13, 25, 25);
    SDL_RenderClear(app.renderer);
}

void presentScene(void) {
    SDL_RenderPresent(app.renderer);
}
