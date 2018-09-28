#ifndef __floodit__
#define __floodit__
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define bitColor unsigned long
#define bool char
#define false 0
#define true !false

#define empty 0
#define toBitColor(A) \
    1 << A;           \
    ;

typedef struct FieldNode
{
    bitColor color;
    int *neighborsId;
    int neighborsSize;
    int id;
} FieldNode;

typedef struct FieldList
{
    int size, realSize;
    FieldNode **list;

} FieldList;

typedef struct Board
{
    int lines, columns;
    int **fields;
} Board;

FieldNode *mergeNodes(FieldNode *root, FieldNode **affectedNodes, int affectedSize, int totalNodes);
FieldList *paintBoard(FieldList *b, int nextColor);

// Setup inical dos nós
void searchNodes(int line, int column, FieldNode *groupNode, FieldNode ***board);
void linkNeighbors(FieldNode *searchNode, FieldNode *toLinkNode, int totalNodes);
FieldList *convertBoardToGraph(Board *boartM);

void freeFieldList(FieldList *b);
int breakDebug();

// void freeMatrix(void **m, int lines);
// //Find result
// int colorsCalculator(BoardList *b, int gameColors);
// int neighborCalculator(BoardList *b, int numColors);
// void searchField(Neighbor *neighbor, Board *b, int line, int column, bool **checkedField);
// int h(Board *b, int numColors, int currentNumColors);
// int *callback(Step *finalStep);

#endif
