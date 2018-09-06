#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "flooditAi.h"

#define DEBUG 1
#define inlineF

//*Globais para testes
bool admissivel = true;
bool consistente = true;
//

void paint(Board *b, int l, int c, int currentColor, int nextColor)
{
    b->fields[l][c] = nextColor;
    if (l < b->lines - 1 && b->fields[l + 1][c] == currentColor)
        paint(b, l + 1, c, currentColor, nextColor);
    if (l < b->lines - 1 && c < b->columns - 1 && b->fields[l + 1][c + 1] == currentColor)
        paint(b, l + 1, c + 1, currentColor, nextColor);
    if (c < b->columns - 1 && b->fields[l][c + 1] == currentColor)
        paint(b, l, c + 1, currentColor, nextColor);
    if (l > 0 && c < b->columns - 1 && b->fields[l - 1][c + 1] == currentColor)
        paint(b, l - 1, c + 1, currentColor, nextColor);
    if (l > 0 && b->fields[l - 1][c] == currentColor)
        paint(b, l - 1, c, currentColor, nextColor);
    if (l > 0 && c > 0 && b->fields[l - 1][c - 1] == currentColor)
        paint(b, l - 1, c - 1, currentColor, nextColor);
    if (c > 0 && b->fields[l][c - 1] == currentColor)
        paint(b, l, c - 1, currentColor, nextColor);
    if (l < b->lines - 1 && c > 0 && b->fields[l + 1][c - 1] == currentColor)
        paint(b, l + 1, c - 1, currentColor, nextColor);
}

Board *paint_board(Board *b, int nextColor)
{
    if (nextColor == b->fields[0][0])
        return NULL;
    Board *newBoard = calloc(1, sizeof(Board));
    newBoard->columns = b->columns;
    newBoard->lines = b->lines;

    newBoard->fields = (int **)malloc(b->lines * sizeof(int *));
    for (int i = 0; i < b->lines; i++)
    {
        newBoard->fields[i] = (int *)malloc(b->columns * sizeof(int));
        for (int j = 0; j < b->columns; j++)
            newBoard->fields[i][j] = b->fields[i][j];
    }
    paint(newBoard, 0, 0, newBoard->fields[0][0], nextColor);
    return newBoard;
}

//Heuristc
int colorsCalculator(NodeList *b, int gameColors)
{
    bitColor* colors= calloc (1, sizeof(colors));
    colors[1]=  takeNeighbors(b->first);
    int colorGroups =1;
    if(b->first->next!=NULL){
        for (BoardListNode i =b->first->next; i->next!=NULL;  i= i->next){
            bitColor newColors= takeNeighbors(i);
            bool insert =true
            for(int j = 0 ; j<colorGroups; j++){
                if((colors[j]|newColors)==color[j]){
                    insert=false;
                    break;
                }else{
                    if((colors[j]|newColors)==newColors){
                        insert=false;
                        colors[j]=newColors;
                        break;
                    }
                }
            }
            if(insert){
                colorGroups++;
                colors= realloc(colors, colorGroups* sizeof (bitColor));
                colors[colorGroups-1]= newColors;
            }
        }   
    }
// TODO free colors, free bitColor

    return colorGroups;

}

inlineF bitColor takeNeighbors(BoardNode *node)
{
    BoardNode* auxNode= b->first;
    bitColor colors = false;   
    for (int i =0;i< node->neighborsSize; i++){
       colors= colors|node->neighbors[i];
   }
   return colors;
}

else
{
    boolVector colorBit = 1 << (b->fields[line][column]);
    (neighbor->color) = (neighbor->color) | colorBit;
}
}


inlineF int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}
int h(Board *b, int numColors, int currentNumColors)
{
    int n =colorsCalculator(b, numColors);
    int c = currentNumColors - 1;
    return max(n, c);
}
int *callback(Step *finalStep)
{

    int *result = calloc(finalStep->f+1, sizeof(int));
    Step* aux=finalStep;
    for(int i=finalStep->f-1; aux->prevStep!=NULL ; i--){
        result[i]=aux->colorStep;
        aux= aux->prevStep;
    }
    return result;
}
//Structures
void expandNode(Step *step, int gameColors, StepQueue *q)
{
    Step **nextSteps = calloc(gameColors, sizeof(Step *));
    for (int i = 0; i < gameColors; i++)
    {
        int colorStep = i + 1;
        if (colorStep != step->colorStep)
        {
            nextSteps[i] = calloc(1, sizeof(Step));
            nextSteps[i]->board = paint_board(step->board, colorStep);
            nextSteps[i]->prevStep = step;
            nextSteps[i]->colorStep = colorStep;
            nextSteps[i]->g = step->g + 1;
            nextSteps[i]->h = h(nextSteps[i]->board, gameColors, colorsCalculator(nextSteps[i]->board, gameColors));
            nextSteps[i]->f = nextSteps[i]->g + nextSteps[i]->h;
            enqueueStep(nextSteps[i], q);
        }
    }
    freeBoard(step->board);
}
void enqueueStep(Step *step, StepQueue *q)
{
    int weight = step->f;
    QueueNode *newNode = calloc(1, sizeof(QueueNode));
    newNode->value = step;
    if (q->size > 0)
    {
        if (q->first->value->f <= weight)
        {
            if (q->last->value->f <= weight)
            {
                q->last->next = newNode;
                q->last = newNode;
                q->size++;
            }
            else
            {
                bool enqueued;
                for (QueueNode *node = q->first->next; (node->next->next != NULL) && !enqueued; node = node->next)
                {
                    if ((node->value->f < weight) && (node->next->value->f > weight))
                    {
                        QueueNode *aux = node->next;
                        node->next = newNode;
                        newNode->next = aux;
                        q->size++;
                    }
                }
            }
        }
        else
        {
            newNode->next = q->first;
            q->first = newNode;
            q->size++;
        }
    }
    else
    {
        q->first = newNode;
        q->last = newNode;
        q->size++;
    }
}
Step *dequeueStep(StepQueue *q)
{
    Step *result;
    if (q->size > 1)
    {
        result = q->first->value;
        q->first = q->first->next;
        q->size--;

        return result;
    }
    else
    {
        if (q->size == 1)
        {
            result = q->first->value;
            q->first = NULL;
            q->last = NULL;
            q->size--;

            return result;
        }
        return NULL;
    }
}

void freeBoard(Board *b)
{
    // freeMatrix(b->fields, b->lines);
    // free(b);
}
void freeMatrix(void **m, int lines)
{
    // for (int i = 0; i < lines; i++)
    // {
    //     free(m[i]);
    // }
    // free(m);
}