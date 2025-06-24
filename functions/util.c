//util.c

#include "common.h"


int collision(  int x1, int y1,
                int w1, int h1,
                int x2, int y2,
                int w2, int h2)
{
    return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2))
        && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
}

/*
Esta função recebe seis argumentos: o x e o y de uma coordenada de
origem, o x e o y de uma coordenada de destino e duas referências a
pontos flutuantes. O que esta função faz é calcular o passo
normalizado de um ponto a outro. O dx ou dy será sempre 1 (ou -1),
enquanto o outro pode ser qualquer valor entre -1 e 1. Isso será
usado para indicar às balas do alienígena em que direção elas
precisam ir para atingir o alvo
*/
void calcSlop(int x1, int y1, int x2, int y2, float *dx, float *dy)
{
    int steps = MAX(abs(x1 - x2), abs(y1 - y2));

    if(steps == 0)
    {
        *dx = *dy = 0;
        return;
    }

    *dx = (x1 - x2);
    *dy /= steps;

    *dy = (y1 - y2);
    *dy /= steps;
}
