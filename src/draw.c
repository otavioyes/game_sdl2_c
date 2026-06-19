/*
 * draw.c
 *
 * Funções auxiliares de renderização e carregamento de texturas.
 */

#include <SDL2/SDL_image.h>

#include "common.h"
#include "draw.h"

extern App app;

/*==============================================================================
 * Prepara o renderer para um novo frame.
 *============================================================================*/
void prepareScene(void)
{
    SDL_SetRenderDrawColor(app.renderer, 32, 32, 32, 255);
    SDL_RenderClear(app.renderer);
}

/*==============================================================================
 * Apresenta o frame renderizado na janela.
 *============================================================================*/
void presentScene(void)
{
    SDL_RenderPresent(app.renderer);
}

/*==============================================================================
 * Busca uma textura já carregada no cache.
 *============================================================================*/
static SDL_Texture *getTexture(char *name)
{
    Texture *t;

    for (t = app.textureHead.next; t != NULL; t = t->next) {
        if (strcmp(t->name, name) == 0) {
            return t->texture;
        }
    }

    return NULL;
}

/*==============================================================================
 * Adiciona uma textura carregada ao cache.
 *============================================================================*/
static void addTextureToCache(char *name, SDL_Texture *sdlTexture)
{
    Texture *texture;

    if (sdlTexture == NULL) {
        return;
    }

    texture = malloc(sizeof(Texture));

    if (texture == NULL) {
        SDL_Log("malloc falhou em addTextureToCache");
        exit(1);
    }

    memset(texture, 0, sizeof(Texture));

    STRNCPY(texture->name, name, MAX_NAME_LENGTH);
    texture->texture = sdlTexture;

    app.textureTail->next = texture;
    app.textureTail = texture;
}

/*==============================================================================
 * Carrega uma textura de arquivo ou retorna uma versão já cacheada.
 *============================================================================*/
SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture;

    texture = getTexture(filename);

    if (texture != NULL) {
        return texture;
    }

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                   SDL_LOG_PRIORITY_INFO,
                   "Loading texture: %s", filename);

    texture = IMG_LoadTexture(app.renderer, filename);

    if (texture == NULL) {
        SDL_Log("IMG_LoadTexture falhou para '%s': %s",
                filename,
                IMG_GetError());
        return NULL;
    }

    addTextureToCache(filename, texture);

    return texture;
}

/*==============================================================================
 * Renderiza uma textura inteira.
 *
 * Se center for verdadeiro, x/y representam o centro da textura.
 * Caso contrário, x/y representam o canto superior esquerdo.
 *============================================================================*/
void blit(SDL_Texture *texture, int x, int y, int center)
{
    SDL_Rect dest;

    if (texture == NULL) {
        return;
    }

    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    if (center) {
        dest.x -= dest.w / 2;
        dest.y -= dest.h / 2;
    }

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

/*==============================================================================
 * Renderiza uma região específica de uma textura.
 *============================================================================*/
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
{
    SDL_Rect dest;

    if (texture == NULL || src == NULL) {
        return;
    }

    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;

    SDL_RenderCopy(app.renderer, texture, src, &dest);
}

/*==============================================================================
 * Renderiza uma textura inteira com rotação.
 *
 * A rotação ocorre em torno do centro da textura.
 *============================================================================*/
void blitRotated(SDL_Texture *texture, float x, float y, float angle)
{
    SDL_Rect dest;

    if (texture == NULL) {
        return;
    }

    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    dest.x -= dest.w / 2;
    dest.y -= dest.h / 2;

    SDL_RenderCopyEx(app.renderer,
                     texture,
                     NULL,
                     &dest,
                     angle,
                     NULL,
                     SDL_FLIP_NONE);
}