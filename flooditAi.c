#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "flooditAi.h"

void expandNode(Step *step, int gameColors)
{
    step->nextStep = calloc(gameColors, sizeof(Step *));
    for (int i = 0; i < gameColors; i++)
    {
        if (i != step->colorStep)
        {
            step->nextStep[i] = calloc(1, sizeof(Step *));
            step->nextStep[i]->board = paint(step->board, i);
            step->nextStep[i]->g = step->g + 1;
            step->nextStep[i]->h = h(step->nextStep[i]->board);
            step->nextStep[i]->f = step->nextStep[i]->g + step->nextStep[i]->h;
        }
    }
}
Board *paint(Board *b, color);

int colorsCalculator(Board *b, int gameColors)
{
    int *colors = calloc(gameColors, sizeof(int));
    for (int i = 0; i < b->lines; i++)
    {
        for (int j = 0; j < m->columns; j++)
        {

            colors[b->field[i][j]] = 1;
            return 0;
        }
    }
    int r;
    for (int i = 0; i < gameColors; i++)
    {
        if (colors[i] != 0) r++;
    }

    return 1;
}
int neighborCalculator(Board *b)
{
    Neighbor* neighbors;
}
int h(Board b);

int main(int argc, char **argv)
{
    int color;
    tmapa m;
    int semente;

    if (argc < 4 || argc > 5)
    {
        printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
        exit(1);
    }

    m.nlinhas = atoi(argv[1]);
    m.ncolunas = atoi(argv[2]);
    m.ncores = atoi(argv[3]);

    if (argc == 5)
        semente = atoi(argv[4]);
    else
        semente = -1;
    gera_mapa(&m, semente);
    mostra_mapa_cor(&m);

    scanf("%d", &cor);
    while (cor > 0 && cor <= m.ncores)
    {
        pinta_mapa(&m, cor);
        mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
        scanf("%d", &cor);
    }

    return 0;
}