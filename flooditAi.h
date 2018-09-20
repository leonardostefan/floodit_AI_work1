#ifndef __flooditAi__
#define __flooditAi__
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
    struct FieldNode **neighbors;
    int neighborsSize;
} FieldNode;

typedef struct FieldListNode
{
    struct FieldListNode *next;
    FieldNode *value;
} FieldListNode;

typedef struct FieldList
{
    int size;
    FieldListNode *first;

} FieldList;

typedef struct Step
{
    FieldList *board;
    struct Step *prevStep;
    struct Step **nextSteps;
    int colorStep;
    int f, g, h;
} Step;

typedef struct Board
{
    int lines, columns;
    int **fields;
} Board;

// typedef struct Neighbor
// {
//     int searchColor;
//     boolVector color;
// } Neighbor;

typedef struct QueueNode
{
    struct QueueNode *next;
    Step *value;
} QueueNode;

typedef struct StepQueue
{
    QueueNode *first;
    QueueNode *last;
    int size;
} StepQueue;

//Structures manipulation
void expandNode(Step *step, int gameColors, StepQueue *q);
void enqueueStep(Step *step, StepQueue *q);
Step *dequeueStep(StepQueue *q);
FieldListNode *mergeNodes(FieldListNode *root, FieldListNode **affectedNodes, int affectedSize, int totalNodes);
// Setup inical dos nós
void searchNodes(int line, int column, FieldNode *groupNode, FieldNode ***board);
void linkNeighbors(FieldNode *searchNode, FieldNode *toLinkNode);
FieldList *convertBoardToGraph(Board *boartM);

void freeFieldList(FieldList *b);

int *callback(Step *finalStep);

// void freeMatrix(void **m, int lines);
// //Find result
// int colorsCalculator(BoardList *b, int gameColors);
// int neighborCalculator(BoardList *b, int numColors);
// void searchField(Neighbor *neighbor, Board *b, int line, int column, bool **checkedField);
// int h(Board *b, int numColors, int currentNumColors);
// int *callback(Step *finalStep);

#endif
