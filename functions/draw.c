// draw.c
#include "common.h"

//prototipos
static void drawBullets(void);

/*prototipos para inimigos*/
static void drawFighters(void);


// Função que prepara a cena antes de desenhar os elementos gráficos
void prepareScene(void)
{
    // Define a cor de fundo da renderização (R=46, G=26, B=71, A=255)
    SDL_SetRenderDrawColor(app.renderer, 46, 26, 71, 255);

    // Limpa a tela com a cor definida acima
    SDL_RenderClear(app.renderer);
}


//atualiza a janela com tudo que foi desenhado no redenrizador
//e como a cortina de um teatro, quando tudo tiver pronto, a cortina abre.
void presentScene(void)
{
    SDL_RenderPresent(app.renderer);
}


// Função responsável por desenhar os lutadores e projéteis, chamando funções de Stage.c
void draw(void)
{
    drawFighters();
    drawBullets();
}


// Carrega uma imagem a partir de um arquivo e retorna uma textura pronta para renderização
SDL_Texture *loadTexture(char *filename)
{
    SDL_Texture *texture = IMG_LoadTexture(app.renderer, filename);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_LoadTexture failed: %s", IMG_GetError());
        exit(1);
    }
    return texture;
}


// Renderiza uma textura na tela com escala e blending, usando coordenadas e tamanho ajustados
void blit(SDL_Texture *texture, int x, int y, float scale)
{
     if (texture == NULL)
     {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "blit() called with NULL texture");
        return;
     }

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


/*
 A função drawFighters percorre todos os lutadores na lista encadeada
 e desenha cada um deles usando o comando blit().
 O jogador agora é desenhado como parte dessa função.
*/
static void drawFighters(void)
{
    Entity *e;

    for(e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        blit(e->texture, e->x, e->y, e->scale);
    }
}


// A função drawBullets percorre a lista encadeada de balas e desenha cada uma com sua textura e escala
static void drawBullets(void)
{
    for (Entity *b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        blit(b->texture, b->x, b->y, b->scale);
    }
}
