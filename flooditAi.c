#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "flooditAi.h"

#define DEBUG 1



void pinta(Board *b, int l, int c, int fundo, int cor)
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

Board* pinta_mapa(Board *b, int cor)
{
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
    if (cor == b->fields[0][0])
        return;
    pinta(b, 0, 0, newBoard->fields[0][0], cor);
    return newBoard;
}

//Heuristc
int colorsCalculator(Board *b, int gameColors)
{
    int *colors = calloc(gameColors, sizeof(int));
    for (int i = 0; i < b->lines; i++)
    {
        for (int j = 0; j < b->columns; j++)
        {
            colors[b->fields[i][j]] = 1;
            return 0;
        }
    }
    int r;
    for (int i = 0; i < gameColors; i++)
    {
        if (colors[i] != 0)
            r++;
    }

    return r;
}

inline Neighbor *takeNeighbors(Board *b, int line, int column, bool **checkedField, int numColors)
{
    boolVector *colors = calloc(numColors, sizeof(boolVector));

    Neighbor *neighbor = calloc(1, sizeof(Neighbor));
    neighbor->color = colors;
    neighbor->searchColor = b->fields[line][column];

    if (checkedField[line][column] != 0)
    {
        searchField(neighbor, b, line, column, checkedField);
    }

    return neighbor;
}

void searchField(Neighbor *neighbor, Board *b, int line, int column, bool **checkedField)
{
    if (neighbor->searchColor == b->fields[line][column])
    {
        if (!checkedField[line][column])
        {
            checkedField[line][column] = true;

            for (int i = -1; i < 2; i++)
            {
                if ((line + i >= 0) && (line + i <= b->lines))
                    for (int j = -1; j < 2; j++)
                    {
                        if ((column + j >= 0) && (column + j <= b->columns))
                        {
                            searchField(neighbor, b, line + i, column + j, checkedField);
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

int neighborCalculator(Board *b, int numColors)
{
    //TODO
    bool **checkedField;

    checkedField = calloc(b->lines, sizeof(bool));
    for (int i = 0; i < b->columns; i++)
    {
        checkedField[i] = calloc(b->columns, sizeof(bool));
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
            if ((colorUnion == neighbors[j]->color))
            {
                isSubSet = true;
            }
        }
        if (!isSubSet)
            result++;
    }
    return (result - 1);
}
inline int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}
int h(Board *b, int numColors, int currentNumColors)
{
    int n = neighborCalculator(b, numColors);
    max(n, currentNumColors - 1);
}
//Structures
void expandNode(Step *step, int gameColors, StepQueue *q)
{
    Step **nextSteps = calloc(gameColors, sizeof(Step *));
    for (int i = 0; i < gameColors; i++)
    {
        if (i != step->colorStep)
        {
            nextSteps[i] = calloc(1, sizeof(Step));
            nextSteps[i]->board = pinta_mapa(step->board, i);
            nextSteps[i]->prevStep = step;
            nextSteps[i]->colorStep = i;
            nextSteps[i]->g = step->g++;
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
}
dequeueStep(Step *step, StepQueue *q)
{
    if (q->size != 0)
    {
        step = q->first->value;
        q->first = q->first->next;
        q->size--;
    }
    else
        step = NULL;
}
