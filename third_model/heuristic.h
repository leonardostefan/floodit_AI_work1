
#include "floodit.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


#ifndef __heuristc__
#define __heuristc__
#define DEBUG_H(A) A;

#define kG (80/100)
#define warningTime 15

int numProcs;
typedef struct Step
{
    FieldList *board;
    struct Step *prevStep;
    struct Step **nextSteps;
    int colorStep;
    int f, g, h;
} Step;

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
void expandNode(Step *step, int gameColors);
bool enqueueStep(Step *step, StepQueue *q);
Step *dequeueStep(StepQueue *q);
int *callback(Step *finalStep);
int *findSolution(Board *mainBoard, int numColors);
int setH(int *h, FieldList *b, int numColors);
int timeSpent();
#endif