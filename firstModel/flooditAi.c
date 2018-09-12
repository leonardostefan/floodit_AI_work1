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

bool paint(Board *b, int l, int c, int currentColor, int nextColor, bool **painted)
{
    bool hasChanges = false;
    painted[l][c] = true;
    b->fields[l][c] = nextColor;

    for (int i = 1; i >= -1; i--)
    {
        if ((l + i >= 0) && (l + i < b->lines))
        {
            for (int j = 1; j >= -1; j--)
            {
                if (((c + j >= 0) && (c + j < b->columns)) && !(j == 0 && i == 0))
                {
                    if (b->fields[l + i][c + j] == currentColor)
                    {
                        hasChanges = hasChanges || paint(b, l + i, c + j, currentColor, nextColor, painted);
                    }
                    if (b->fields[l + i][c + j] == nextColor && !painted[l + i][c + j])
                    {
                        hasChanges = true;
                    }
                }
            }
        }
    }

    return hasChanges;
}

Board *paint_board(Board *b, int nextColor)
{
    bool **painted;

    painted = calloc(b->lines + 1, sizeof(bool *));
    for (int i = 0; i < b->lines; i++)
    {
        painted[i] = calloc(b->columns + 1, sizeof(bool));
        for (int j = 0; j < b->columns; j++)
        {
            painted[i][j] = false;
        }
    }
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
    bool hasChanges = paint(newBoard, 0, 0, newBoard->fields[0][0], nextColor, painted);
    if (hasChanges)
    {
        return newBoard;
    }
    else
    {
        return NULL;
    }
}

//Heuristc
int colorsCalculator(Board *b, int gameColors)
{
    int *colors = calloc(gameColors, sizeof(int));
    for (int i = 0; i < b->lines; i++)
    {
        for (int j = 0; j < b->columns; j++)
        {
            colors[b->fields[i][j] - 1] = 1;
        }
    }
    int r = 0;
    for (int i = 0; i < gameColors; i++)
    {
        if (colors[i] != 0)
            r++;
    }

    free(colors);
    return r;
}

inlineF Neighbor *takeNeighbors(Board *b, int line, int column, bool **checkedField, int numColors)
{

    Neighbor *neighbor = calloc(1, sizeof(Neighbor));
    neighbor->color = false;
    neighbor->searchColor = b->fields[line][column];
    neighbor->setCounted = false;
    if (checkedField[line][column] == 0)
    {
        searchField(neighbor, b, line, column, checkedField);
    }

    return neighbor;
}

void searchField(Neighbor *neighbor, Board *b, int line, int column, bool **checkedField)
{
    if (neighbor->searchColor == b->fields[line][column])
    {
        checkedField[line][column] = true;

        for (int i = 1; i >= -1; i--)
        {
            if ((line + i >= 0) && (line + i < b->lines))
            {
                for (int j = 1; j >= -1; j--)
                {
                    if (((column + j >= 0) && (column + j < b->columns)) && !(j == 0 && i == 0))
                    {
                        if (!checkedField[line + i][column + j])
                        {
                            searchField(neighbor, b, line + i, column + j, checkedField);
                        }
                    }
                }
            }
        }
    }
    else
    {
        boolVector colorBit = 1 << (b->fields[line][column]);
        (neighbor->color) = (neighbor->color) | colorBit;
    }
}

int neighborCalculator(Board *b, int numColors, int currentNumColors)
{
    //TODO free  Neighbor **neighbors
    bool **checkedField;

    checkedField = calloc(b->lines + 1, sizeof(bool *));
    for (int i = 0; i < b->lines; i++)
    {
        checkedField[i] = calloc(b->columns + 1, sizeof(bool));
        for (int j = 0; j < b->columns; j++)
        {
            checkedField[i][j] = false;
        }
    }
    Neighbor **neighbors = calloc(1, sizeof(Neighbor *));
    int neighborsAmount = 0;

    for (int i = 0; i < b->columns; i++)
    {
        for (int j = 0; j < b->columns; j++)
        {
            if (!checkedField[i][j])
            {
                neighbors[neighborsAmount] = takeNeighbors(b, i, j, checkedField, numColors);
                neighborsAmount++;
                neighbors = realloc(neighbors, (neighborsAmount + 1) * sizeof(Neighbor *));
            }
        }
    }
    int result = 0;
    for (int i = 0; i < neighborsAmount; i++)
    {
        bool isSubSet = false;

        for (int j = 0; (j < neighborsAmount) && !isSubSet; j++)
        {
            boolVector colorUnion = neighbors[i]->color | neighbors[j]->color;

            if ((colorUnion == neighbors[j]->color) && (neighbors[i]->color != neighbors[j]->color))
            {
                isSubSet = true;
            }
            else
            {
                if ((neighbors[i]->color == neighbors[j]->color) && (neighbors[i]->setCounted || neighbors[j]->setCounted))
                {
                    isSubSet = true;
                }
            }
        }
        if (!isSubSet)
        {
            result++;
            // boolVector actualSet = neighbors[i]->color;
            // for (int i = 0; i < numColors + 2; i++)
            // {
            //     if (((actualSet >> i) % 2))
            //     {
            //         result++;
            //     }
            // }
        }

        neighbors[i]->setCounted = true;
    }
    freeMatrix(neighbors, 1);    // boolVector rootColors = neighbors[0]->color;
    // for (int i = 0; i < numColors + 2; i++)
    // {
    //     if (((rootColors >> i) % 2))
    //     {
    //         result--;
    //     }
    // }
    freeMatrix(checkedField, b->lines);

    // boolVector rootColors = neighbors[0]->color;
    // for (int i = 0; i < numColors + 2; i++)
    // {
    //     if (((rootColors >> i) % 2))
    //     {
    //         result--;
    //     }
    // }

    return (result + currentNumColors -2);
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
    int n;
    n = neighborCalculator(b, numColors, currentNumColors);
    int c = currentNumColors - 1;

    IF_DEBUG
    printf("\nn = %d\n", n);
    printf("c = %d\n", c);
    END_IF_DEBUG;
    if(c==0 && n>0){
        printf("ERRO, heuristica inadimissivel");
        return 0;
    }
    return max(max(n, c), 0);
}
int *callback(Step *finalStep)
{

    int *result = calloc(finalStep->f + 1, sizeof(int));
    Step *aux = finalStep;
    for (int i = finalStep->f - 1; aux->prevStep != NULL; i--)
    {
        result[i] = aux->colorStep;
        aux = aux->prevStep;
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
            Board *newBoard = paint_board(step->board, colorStep);
            if (newBoard != NULL)
            {
                nextSteps[i]->board = newBoard;
                nextSteps[i]->prevStep = step;
                nextSteps[i]->colorStep = colorStep;
                nextSteps[i]->g = step->g + 1;
                nextSteps[i]->h = h(nextSteps[i]->board, gameColors, colorsCalculator(nextSteps[i]->board, gameColors));
                nextSteps[i]->f = nextSteps[i]->g + nextSteps[i]->h;
                enqueueStep(nextSteps[i], q);
            }
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
                for (QueueNode *node = q->first->next; (node->next != NULL) && !enqueued; node = node->next)
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
    freeMatrix(b->fields, b->lines);
    free(b);
}
void freeMatrix(void **m, int lines)
{
    // for (int i = 0; i < lines; i++)
    // {
    //     free(m[i]);
    // }
    // free(m);
}