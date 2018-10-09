
#include "floodit.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef __heuristc__
#define __heuristc__
#define DEBUG_H(A) ; // definição para ativar debugs

#define warningTime 15 //tempo limite para usar a heuristica "boa"

//---Fila de prioridade de nós/passos não expandidos---
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
//------

//Expande um nó aberto
void expandNode(Step *step, int gameColors);

//Manipulação das fila de nós aberto
bool enqueueStep(Step *step, StepQueue *q);
Step *dequeueStep(StepQueue *q);

int *findSolution(Board *mainBoard, int numColors);
int *callback(Step *finalStep);

//Heuristica usada
int setH(int *h, FieldList *b, int numColors);

//Verificação do tempo gasto
int timeSpent();
#endif