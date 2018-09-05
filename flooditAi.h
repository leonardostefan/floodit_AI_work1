#ifndef __flooditAi__
#define __flooditAi__
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define boolVector long int
#define bool char
#define false 0
#define true !false

typedef struct Board
{
    int lines, columns;
    int **fields;
} Board;

typedef struct Step
{
    Board *board;
    struct Step *prevStep;
    struct Step **nextSteps;
    int colorStep;
    int f, g, h;
} Step;

typedef struct Neighbor
{
    int searchColor;
    boolVector color;
} Neighbor;

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
void enqueueStep(Step *step, StepQueue* q);
void dequeueStep(Step *step, StepQueue* q);
void freeBoard(Board *b);

//Find result
int colorsCalculator(Board *b, int gameColors);
int neighborCalculator(Board *b, int numColors);
void searchField(Neighbor *neighbor, Board *b, int line, int column, bool **checkedField);
int h(Board *b);
int *callback(Step *finalStep);

#endif
