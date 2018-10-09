#ifndef __floodit__
#define __floodit__
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define bitColor unsigned long
#define bool char
#define false 0
#define true !false
#define DEBUG(A) ;

#define empty 0

typedef struct FieldNode
{
    bitColor color;
    int *neighborsId;
    int neighborsSize;
    int id;
} FieldNode;

typedef struct FieldList
{
    int size;
    int realSize;
    FieldNode **list;

} FieldList;

typedef struct Board
{
    int lines, columns;
    int **fields;
} Board;

//Pintura dos nós
FieldList *paintBoard(FieldList *b, int nextColor);
//União/pintura do primeiro nó com nós afetados
FieldNode *mergeNodes(FieldNode *root, FieldNode **affectedNodes, int affectedSize, int totalNodes);

// Setup inical dos nós
FieldList *convertBoardToGraph(Board *boartM); //Gera grafo a partir da matriz
void searchNodes(int line, int column, FieldNode *groupNode, FieldNode ***board);
void linkNeighbors(FieldNode *searchNode, FieldNode *toLinkNode, int totalNodes);

//Redução da memória gasta para representar o mesmo grafo
FieldList *reduceGraph(FieldList *b);

void freeFieldList(FieldList *b);

int breakDebug(); //para debug

#endif
