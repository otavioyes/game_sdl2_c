// draw.c
#include "common.h"

//prototipos
static void drawBullets(void);

/*prototipos para inimigos*/
static void drawFighters(void);


//funcao de desenhar a tela-ROXO MEIA NOITE
void prepareScene(void)
{
    SDL_SetRenderDrawColor(app.renderer, 46, 26, 71, 255);//define a cor de fundo
    SDL_RenderClear(app.renderer); //limpa a tela com essa cor
}


//atualiza a janela com tudo que foi desenhado no redenrizador
//e como a cortina de um teatro, quando tudo tiver pronto, a cortina abre.
void presentScene(void)
{
    SDL_RenderPresent(app.renderer);
}


//funcao que chama as funcoes de Stage.c
void draw(void)
{
    drawFighters();
    drawBullets();
}


// carrega uma textura da imagem no arquivo
SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture = IMG_LoadTexture(app.renderer, filename);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_LoadTexture failed: %s", IMG_GetError());
        exit(1);
    }
    return texture;
}


// desenha uma textura escalada
void blit(SDL_Texture *texture, int x, int y, float scale)
{
     if (texture == NULL)
     {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "blit() called with NULL texture");
        return;
     }


    //redenrizando o tamanho da imagem.
    int w, h;

    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Rect dest =
    {
        .x = x,
        .y = y,

        .w = (int)(w * scale),
        .h = (int)(h * scale),
    };

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}


/*a funcao drawFighters percorre todos os lutadores na lista
encadeada e desenha cada um deles, usando o comando blit.
O jogador agora e desenhado como parte da funcao drawFighter.*/
static void drawFighters(void)
{
    Entity *e;

    for(e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        blit(e->texture, e->x, e->y, e->scale);
    }
}


//chama a funcao que definem o tamanho das balas e desenha
static void drawBullets(void)
{
    for (Entity *b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        blit(b->texture, b->x, b->y, b->scale);
    }
}
