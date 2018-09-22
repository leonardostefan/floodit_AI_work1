#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "./flooditAi.h"

/* Mesmas funções utilizadas no exemplo do professor, 
* apenas algumas adaptações para utilizar as funções e estruturas criadas para a solução
*/

void createBoard(Board *b, int numColors)
{
    int i, j;
    srand(time(NULL));

    b->fields = (int **)malloc(b->lines * sizeof(int *));
    for (i = 0; i < b->lines; i++)
    {
        b->fields[i] = (int *)malloc(b->columns * sizeof(int));
        for (j = 0; j < b->columns; j++)
            b->fields[i][j] = 1 + rand() % numColors;
    }
}


void loadBoard(Board *b, int* numColors)
{
    int i, j;

    scanf("%d", &(b->lines));
    scanf("%d", &(b->columns));
    scanf("%d", numColors);
    b->fields = (int **)malloc(b->lines * sizeof(int *));
    for (i = 0; i < b->lines; i++)
    {
        b->fields[i] = (int *)malloc(b->columns * sizeof(int));
        for (j = 0; j < b->columns; j++)
            scanf("%d", &(b->fields[i][j]));
    }
}



void mostra_mapa(Board* b, int numColors)
{
    int i, j;

    printf("%d %d %d\n", b->lines, b->columns, numColors);
    for (i = 0; i < b->lines; i++)
    {
        for (j = 0; j < b->columns; j++)
            if (numColors > 10)
                printf("%02d ", b->fields[i][j]);
            else
                printf("%d ", b->fields[i][j]);
        printf("\n");
    }
}

void mostra_mapa_cor(Board* b, int numColors)
{
    int i, j;
    char *cor_ansi[] = {"\x1b[0m",
                        "\x1b[31m", "\x1b[32m", "\x1b[33m",
                        "\x1b[34m", "\x1b[35m", "\x1b[36m",
                        "\x1b[37m", "\x1b[30;1m", "\x1b[31;1m",
                        "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m",
                        "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m"};

    if (numColors > 15)
    {
        mostra_mapa(b, numColors);
        return;
    }
    printf("\n%d %d %d\n", b->lines, b->columns, numColors);
    for (i = 0; i < b->lines; i++)
    {
        for (j = 0; j < b->columns; j++)
            if (numColors > 10)
                printf("%s%02d%s ", cor_ansi[b->fields[i][j]], b->fields[i][j], cor_ansi[0]);
            else
                printf("%s%d%s ", cor_ansi[b->fields[i][j]], b->fields[i][j], cor_ansi[0]);
        printf("\n");
    }
}

void pinta(Board* b, int l, int c, int fundo, int cor)
{
    b->fields[l][c] = cor;
    if (l < b->lines - 1 && b->fields[l + 1][c] == fundo)
        pinta(b, l + 1, c, fundo, cor);
    if (l < b->lines - 1 && c < b->columns - 1 && b->fields[l + 1][c + 1] == fundo)
        pinta(b, l + 1, c + 1, fundo, cor);
    if (c < b->columns - 1 && b->fields[l][c + 1] == fundo)
        pinta(b, l, c + 1, fundo, cor);
    if (l > 0 && c < b->columns - 1 && b->fields[l - 1][c + 1] == fundo)
        pinta(b, l - 1, c + 1, fundo, cor);
    if (l > 0 && b->fields[l - 1][c] == fundo)
        pinta(b, l - 1, c, fundo, cor);
    if (l > 0 && c > 0 && b->fields[l - 1][c - 1] == fundo)
        pinta(b, l - 1, c - 1, fundo, cor);
    if (c > 0 && b->fields[l][c - 1] == fundo)
        pinta(b, l, c - 1, fundo, cor);
    if (l < b->lines - 1 && c > 0 && b->fields[l + 1][c - 1] == fundo)
        pinta(b, l + 1, c - 1, fundo, cor);
}

void pinta_mapa(Board* b, int numColors, int cor)
{
    if (cor == b->fields[0][0])
        return;
    pinta(b, 0, 0, b->fields[0][0], cor);
}

int main(int argc, char **argv)
{
    int num;
    int steps = 0;
    int parada = 3;

    Board* firstBoard= calloc(1, sizeof(Board));
    int numColors;
    if (argc != 1)
    {

        if ((argc < 4 || argc > 4))
        {
            printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> ", argv[0]);
            exit(1);
        }

        firstBoard->lines = atoi(argv[1]);
        firstBoard->columns = atoi(argv[2]);
        numColors = atoi(argv[3]);
        createBoard(firstBoard, numColors);
    }else{
        loadBoard(firstBoard, &numColors);
    }

    mostra_mapa_cor(firstBoard, numColors);


    StepQueue* stepQueue= calloc(1, sizeof(StepQueue));
    stepQueue->size = 0;
    Step* firstStep= calloc (1, sizeof(Step));
    firstStep->board = convertBoardToGraph(firstBoard);
    // printGraph(firstStep->board); 
    firstStep->prevStep= NULL;
    firstStep->h=1;
    firstStep->f=1;
    firstStep->g=1;
    firstStep->colorStep= (firstBoard->fields)[0][0];
    
    Step* aux= firstStep;
    int tester =0; 
    char c;
    while (aux->h > 0){
        tester++;
        printf("\nNÓS EXPANDIDOS: %d ", tester);

        expandNode(aux, numColors, stepQueue);
        aux = dequeueStep( stepQueue);

    }

    int* result = callback(aux);


    printf("\nnumero de passos: %d\n", aux->f);
    for (int i=0; i < aux->f; i++){
        printf("%d ",result[i]);
    }

    mostra_mapa_cor(firstBoard, numColors);
    return 0;
}