/*
 * util.c
 */

#include "common.h"


/*==============================================================================
 * Verifica colisão entre dois retângulos.
 *
 * Responsabilidades:
 * - Comparar duas caixas retangulares
 * - Detectar sobreposição horizontal
 * - Detectar sobreposição vertical
 *
 * Retorno:
 * - 1 : houve colisão
 * - 0 : não houve colisão
 *============================================================================*/
int collision(int x1, int y1,
              int w1, int h1,
              int x2, int y2,
              int w2, int h2)
{
    return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) &&
           (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
}


/*==============================================================================
 * Calcula direção normalizada entre dois pontos.
 *
 * Responsabilidades:
 * - Calcular diferença entre origem e destino
 * - Normalizar deslocamento usando maior eixo
 * - Evitar divisão por zero
 *
 * Parâmetros:
 * - x1, y1 : ponto de destino
 * - x2, y2 : ponto de origem
 * - dx, dy : saída com direção normalizada
 *
 * Observação:
 * - Esta função usa normalização baseada no maior eixo,
 *   útil para movimento simples em estilo arcade.
 *============================================================================*/
void calcSlop(int x1, int y1, int x2, int y2, float *dx, float *dy)
{
    int steps;

    if (dx == NULL || dy == NULL) {
        return;
    }

    steps = MAX(abs(x1 - x2), abs(y1 - y2));

    if (steps == 0) {
        *dx = 0;
        *dy = 0;
        return;
    }

    *dx = (float)(x1 - x2) / steps;
    *dy = (float)(y1 - y2) / steps;
}



/*==============================================================================
 * A função utiliza nossa definição de PI
 *
 *============================================================================*/
float getAngle(int x1, int y1, int x2, int y2)
{
    float angle = -90 + atan2(y1 - y2, x1 - x2) * (180 / PI);
    return angle >= 0 ? angle : 360 + angle;
}