/*
 * draw.c
 */

#include <SDL2/SDL_image.h>

#include "common.h"
#include "draw.h"

extern App app;


/*==============================================================================
 * Prepara a cena para um novo frame.
 *
 * Responsabilidades:
 * - Definir cor base do renderer
 * - Limpar o frame anterior
 * - Preparar o buffer para novos desenhos
 *============================================================================*/
void prepareScene(void)
{
    SDL_SetRenderDrawColor(app.renderer, 32, 32, 32, 255);
    SDL_RenderClear(app.renderer);
}


/*==============================================================================
 * Apresenta o frame renderizado na tela.
 *
 * Responsabilidades:
 * - Enviar o back buffer para a janela
 * - Finalizar o ciclo visual do frame atual
 *============================================================================*/
void presentScene(void)
{
    SDL_RenderPresent(app.renderer);
}


/*==============================================================================
 * Busca uma textura previamente carregada no cache.
 *
 * Responsabilidades:
 * - Percorrer a lista de texturas carregadas
 * - Comparar o nome solicitado com os nomes armazenados
 * - Retornar textura existente caso encontrada
 *
 * Retorno:
 * - SDL_Texture* : textura encontrada
 * - NULL         : textura não encontrada no cache
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
 * Adiciona uma textura ao cache global da aplicação.
 *
 * Responsabilidades:
 * - Alocar nó de cache
 * - Armazenar nome da textura
 * - Armazenar ponteiro da SDL_Texture
 * - Inserir textura no final da lista encadeada
 *============================================================================*/
static void addTextureToCache(char *name, SDL_Texture *sdlTexture)
{
    Texture *texture;

    texture = malloc(sizeof(Texture));

    if (texture == NULL) {
        SDL_Log("malloc falhou em addTextureToCache");
        exit(1);
    }

    memset(texture, 0, sizeof(Texture));

    app.textureTail->next = texture;
    app.textureTail = texture;

    STRNCPY(texture->name, name, MAX_NAME_LENGTH);

    /*
     * Armazena a textura carregada.
     *
     * Sem esta atribuição, o cache guarda apenas o nome,
     * mas perde o ponteiro real da SDL_Texture.
     */
    texture->texture = sdlTexture;
}


/*==============================================================================
 * Carrega uma textura a partir de arquivo.
 *
 * Responsabilidades:
 * - Verificar se a textura já existe no cache
 * - Carregar textura com SDL_image quando necessário
 * - Armazenar textura carregada no cache
 * - Retornar ponteiro para uso nos módulos do jogo
 *============================================================================*/
SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture;

    texture = getTexture(filename);

    if (texture == NULL) {
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
    }

    return texture;
}


/*==============================================================================
 * Renderiza uma textura inteira na posição informada.
 *
 * Responsabilidades:
 * - Obter dimensões reais da textura
 * - Configurar retângulo de destino
 * - Renderizar textura sem rotação
 *============================================================================*/
void blit(SDL_Texture *texture, int x, int y)
{
    SDL_Rect dest;

    if (texture == NULL) {
        return;
    }

    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}


/*==============================================================================
 * Renderiza uma região específica de uma textura.
 *
 * Responsabilidades:
 * - Usar um SDL_Rect de origem
 * - Renderizar apenas parte da textura
 * - Manter tamanho original do recorte
 *
 * Usado principalmente para:
 * - debris
 * - spritesheets
 * - animações
 * - tiles
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
 * Responsabilidades:
 * - Obter dimensões reais da textura
 * - Configurar retângulo de destino
 * - Aplicar rotação em graus
 * - Renderizar usando SDL_RenderCopyEx
 *
 * Observação:
 * - A rotação ocorre em torno do centro da textura,
 *   pois o parâmetro center está como NULL.
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

    SDL_RenderCopyEx(app.renderer,
                     texture,
                     NULL,
                     &dest,
                     angle,
                     NULL,
                     SDL_FLIP_NONE);
}