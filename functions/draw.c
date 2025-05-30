// draw.c
#include "common.h"

static void drawPlayer(void);
static void drawBullets(void);

void prepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 46, 26, 71, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void) {
    SDL_RenderPresent(app.renderer);
}

void draw(void) {
    drawPlayer();
    drawBullets();
}

// carrega uma textura da imagem no arquivo
SDL_Texture *loadTexture(char *filename) {
    SDL_Texture *texture = IMG_LoadTexture(app.renderer, filename);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_LoadTexture failed: %s", IMG_GetError());
        exit(1);
    }
    return texture;
}

// desenha uma textura escalada
void blit(SDL_Texture *texture, int x, int y, float scale) {
    if (texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "blit() called with NULL texture");
        return;
    }
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Rect dest = {
        .x = x,
        .y = y,
        .w = (int)(w * scale),
        .h = (int)(h * scale),
    };

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

static void drawPlayer(void) {
    blit(player->texture, player->x, player->y, 1.0f);
}

static void drawBullets(void) {
    for (Entity *b = stage.bulletHead.next; b != NULL; b = b->next) {
        blit(b->texture, b->x, b->y, 1.0f);
    }
}
