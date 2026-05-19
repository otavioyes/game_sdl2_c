/*
 * text.c
 */

#include "common.h"

#include "draw.h"
#include "text.h"

static SDL_Texture *fontTexture;
static char         drawTextBuffer[MAX_LINE_LENGTH];


/*==============================================================================
 * Inicializa o sistema de fontes do jogo.
 *
 * Responsabilidades:
 * - Carregar textura da fonte bitmap
 * - Validar carregamento da textura
 * - Preparar fonte para renderização de textos
 *============================================================================*/
void initFonts(void)
{
    fontTexture = loadTexture("assets/gfx/font.png");

    if (fontTexture == NULL) {
        SDL_Log("Erro ao carregar textura da fonte em initFonts");
        exit(1);
    }
}


/*==============================================================================
 * Renderiza texto na tela utilizando fonte bitmap.
 *
 * Responsabilidades:
 * - Formatar texto usando argumentos variáveis
 * - Aplicar alinhamento horizontal
 * - Aplicar cor na textura da fonte
 * - Renderizar caractere por caractere
 *
 * Parâmetros:
 * - x, y   : posição base do texto
 * - r,g,b  : cor aplicada ao texto
 * - align  : alinhamento horizontal
 * - format : texto formatado no estilo printf
 *============================================================================*/
void drawText(int x, int y, int r, int g, int b, int align, char *format, ...)
{
    int      i;
    int      len;
    int      c;
    SDL_Rect rect;
    va_list  args;

    if (fontTexture == NULL || format == NULL) {
        return;
    }

    memset(drawTextBuffer, '\0', sizeof(drawTextBuffer));

    /*
     * Formata o texto de forma segura, respeitando
     * o limite máximo do buffer.
     */
    va_start(args, format);
    vsnprintf(drawTextBuffer, sizeof(drawTextBuffer), format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    /*
     * Ajusta posição inicial de acordo com o alinhamento.
     */
    switch (align) {
        case TEXT_RIGHT:
            x -= len * GLYPH_WIDTH;
            break;

        case TEXT_CENTER:
            x -= (len * GLYPH_WIDTH) / 2;
            break;

        default:
            break;
    }

    /*
     * Cada caractere da fonte bitmap possui tamanho fixo.
     */
    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    /*
     * Aplica cor ao texto antes de renderizar os caracteres.
     */
    SDL_SetTextureColorMod(fontTexture, r, g, b);

    /*
     * Percorre cada caractere do texto formatado.
     */
    for (i = 0; i < len; i++) {
        c = drawTextBuffer[i];

        /*
         * A fonte bitmap atual contém caracteres entre
         * espaço (' ') e 'Z'.
         */
        if (c >= ' ' && c <= 'Z') {
            rect.x = (c - ' ') * GLYPH_WIDTH;

            blitRect(fontTexture, &rect, x, y);
        }

        /*
         * Avança a posição horizontal mesmo quando o caractere
         * não existe na fonte, mantendo espaçamento consistente.
         */
        x += GLYPH_WIDTH;
    }
}