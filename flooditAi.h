#ifndef __flooditAi__
#define __flooditAi__
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define bitColor long int
#define bool char
#define false 0
#define true !false

#define empty 0 
#define toBitColor(A) 1<<A;;


typedef struct BoardNode
{
    bitColor color;
    BoardNode **neighbors;
    int neighborsSize;
} BoardNode;

 
typedef struct NodeList
{
    int size;
    BoardListNode* first;       
} NodeList;

typedef struct BoardListNode
    struct BoardListNode *next;
    BoardNode* value;
}BoardListNode;


typedef struct Step
{
    BoardList *board;
    struct Step *prevStep;
    struct Step **nextSteps;
    int colorStep;
    int f, g, h;
} Step;

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
void enqueueStep(Step *step, StepQueue* q);
Step* dequeueStep(StepQueue* q);

void freeBoard(BoardList *b);
void freeMatrix(void** m, int lines );
//Find result
int colorsCalculator(BoardList *b, int gameColors);
int neighborCalculator(BoardList *b, int numColors);
void searchField(Neighbor *neighbor, Board *b, int line, int column, bool **checkedField);
int h(Board *b, int numColors, int currentNumColors);
int *callback(Step *finalStep);

#endif
