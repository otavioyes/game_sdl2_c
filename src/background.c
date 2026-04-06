/*
 *background.c
 */

#include "common.h"

#include "background.h"
#include "draw.h"

extern App app;

static int          backgroundX;
static Star         star[MAX_STARS];
static SDL_Texture  *background;

void initBackground(void) {
    background = loadTexture("background.png");
    backgroundX = 0;
}

void initStarfield(void) {
    int i;
    for (i = 0; i < MAX_STARS; i++) {
        star[i].x = rand() % SCREEN_WIDTH;
        star[i].y = rand() % SCREEN_HEIGHT;
        star[i].speed = 1 + rand() % 8;
    }
}

void doBackground(void) {
    if (--backgroundX < -SCREEN_WIDTH) {
        backgroundX = 0;
    }
}

void doStarfield(void) {
    int i;
    for (i = 0; i < MAX_STARS; i++) {
        star[i].x -= star[i].speed;
        if (star[i].x < 0) {
            star[i].x = SCREEN_WIDTH - star[i].x;
        }
    }
}

void drawBackground(void) {
    int i, c;
    for (i = 0; i < MAX_STARS; i++) {
        c = 32 * star[i].speed;
        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);
        SDL_RenderDrawLine(app.renderer, star[i].x, star[i].y, star[i].x + 3, star[i].y);
    }
}

void drawBackground2(void) {
    SDL_Rect dest;
    int x;

    for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH) {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }
}

