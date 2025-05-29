// draw.c
#include "common.h"

void prepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 46, 26, 71, 255);//r, g, b, a (alpha/opaco)
    SDL_RenderClear(app.renderer);
}

void presentScene(void) {
    SDL_RenderPresent(app.renderer);
}

/* a funcao SDL_Texture e blit carregam uma imagem e exibe */
SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    /*verificacao de erro ao carregar a textura*/
    texture = IMG_LoadTexture(app.renderer, filename);
    if(!texture){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Error to open image: %s", IMG_GetError());
        }

 return texture;

}

/* a funcao blit simplismente desenha a textura especificada na tela nas coodernadas
x, y e scale  */
void blit(SDL_Texture *texture, int x, int y, float scale)
{
     if (texture == NULL) {
        SDL_Log("Tentou desenhar uma textura NULL!");
        return;
    }

    int origW, origH;
    int bulletW, bulletH;
    SDL_QueryTexture(texture, NULL, NULL, &origW, &origH);
    SDL_QueryTexture(texture, NULL, NULL, &bulletW, &bulletH);

    /*calcula o retangulo de desino ja escalado*/
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = (int)(origW * scale);
    dest.h = (int)(origH * scale);

     SDL_Log("Desenhando textura com tamanho: %d x %d", dest.w, dest.h);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}
