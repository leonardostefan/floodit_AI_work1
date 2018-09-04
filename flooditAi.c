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

    return r;
}



inline Neighbor* takeNeighbors( Board *b, int line,int column, bool** checkedField, int numColors){
    bool* colors = calloc(numColors,sizeof (bool) );
    
    Neighbor neighbor= calloc(1, sizeof(Neighbor));
    neighbor->color= colors;
    neighbor->searchColor=b->field[line][column];
    if (checkedField[line][column]!=0){
        searchField(neighbor,b,line,column,checkedField);
    }     
    neighbor->size=0
    for (int i = 0; i < numColors; ++i)
    {
        if (neighbor->color[i]){
            (neighbor->size)++;
        }
    }
    return neighbor;
}

void searchField(Neighbor* neighbor, Board* b, int line,int column, bool** checkedField){
    if(neighbor->searchColor == b->field[line][column]){
        if(!checkedField[line][column]){
            checkedField[line][column]= true;
            if (i!=0 && (i!=b->lines-1) &&  j!=0 && (j!= b->column-1)){
               searchField(neighbor,b, line-1,column-1,checkedField);
               searchField(neighbor,b, line-1,column,checkedField);
               searchField(neighbor,b, line-1,column+1,checkedField);
               searchField(neighbor,b, line,column-1,checkedField);
               searchField(neighbor,b, line,column+1,checkedField);
               searchField(neighbor,b, line+1,column-1,checkedField);
               searchField(neighbor,b, line+1,column,checkedField);
               searchField(neighbor,b, line+1,column+1,checkedField);
           }
             //TODO: tratar laterais 
       }
   }else{
     neighbor->color[b->field[line][column]]=true;
 }
}

int neighborCalculator(int line,int column, Board *b, int numColors)
{
//TODO
    bool** checkedField

    checkedField = calloc(b->lines, bool*);
    for (int i = 0; i< b->columns;i++){
        checkedField[i]= calloc(b->columns, bool);
    }
    Neighbor* neighbors;

    for (int i =0 ;i< b->columns; i++){
        for (int j =0 ;j< b->columns; j++){
            if (!checkedField[i][j]){







            }
        }
    }





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