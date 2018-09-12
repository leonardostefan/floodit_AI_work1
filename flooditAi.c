#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "flooditAi.h"

#define DEBUG 1
#define inlineF
#define MAX_AFFECT_NODE 4

//*Globais para testes
bool admissivel = true;
bool consistente = true;
//

//Function to paint the first field meging the neighbors of the affected nodes(and painted node) to a new root node
FieldListNode *mergeNodes(FieldListNode *root, FieldListNode **affectedNodes, int totalNodes)
{
    FieldListNode *newRoot = calloc(1, sizeof(FieldListNode));
    newRoot->value = calloc(1, sizeof(FieldNode));
    newRoot->value->color = affectedNodes[0]->value->color;
    newRoot->value->neighborsSize = totalNodes;
    newRoot->value->neighbors = calloc(newRoot->value->neighborsSize, sizeof(FieldNode *));
    int realSize = 0;
    for (int i = 0; i < root->value->neighborsSize; i++)
    {
        bool isNotAffected = true;
        for (int j = 0; isNotAffected && (j < MAX_AFFECT_NODE); j++)
        {
            if ((affectedNodes[j] != NULL) && (root->value->neighbors[i] == affectedNodes[j]->value))
            {
                isNotAffected = false;
            }
        }
        if (isNotAffected)
        {
            newRoot->value->neighbors[realSize] = root->value->neighbors[i];
            realSize++;
        }
    }
    for (int i = 0; i < MAX_AFFECT_NODE; i++)
    {
        if (affectedNodes[i] != NULL)
        {
            for (int j = 0; j < affectedNodes[i]->value->neighborsSize; j++)
            {
                if (affectedNodes[i]->value->neighbors[j] != root->value)
                {
                    newRoot->value->neighbors[realSize] = affectedNodes[i]->value->neighbors[j];
                    realSize++;
                }
            }
        }
    }
    newRoot->value->neighbors = realloc(newRoot->value->neighbors, realSize * sizeof(FieldNode *));
    newRoot->value->neighborsSize = realSize;

    return newRoot;
}

//Create and remap an new board field list

FieldList *paintBoard(FieldList *b, int nextColor)
{

    bitColor newColor = 1 << nextColor;
    if (newColor == b->first->value->color)
        return NULL;

    bool changeBoard = false;

    FieldListNode **affectedNodes = calloc(MAX_AFFECT_NODE, sizeof(FieldListNode *));
    int affectedSize = 0;
    for (int i = 0; i < b->first->value->neighborsSize; i++)
    {
        if (b->first->value->neighbors[i]->color == newColor)
        {
            for (FieldListNode *node = b->first; node->next != NULL; node = node->next)
            {
                if (node->value == b->first->value->neighbors[i])
                {
                    affectedNodes[affectedSize] = node;
                    changeBoard = true;
                    affectedSize++;
                }
            }
        }
    }
    if (!changeBoard)
    {
        free(affectedNodes);
        return NULL;
    }
    //Create nodes
    FieldListNode ***nodePairs = calloc(2, sizeof(FieldListNode **));
    FieldListNode *oldPairAux = b->first;
    nodePairs[0][0] = oldPairAux;
    nodePairs[1][0] = mergeNodes(oldPairAux, affectedNodes, b->size);
    for (int i = 1; i < b->size; i++)
    {
        nodePairs[0][i] = oldPairAux;
        oldPairAux = oldPairAux->next;

        bool isNotAffected = true;
        for (int j = 0; j < affectedSize; j++)
        {
            if (nodePairs[0][i] == affectedNodes[j])
            {
                isNotAffected = false;
            }
        }
        if (isNotAffected)
        {

            nodePairs[1][i] = calloc(1, sizeof(FieldNode));
            nodePairs[1][i]->value = calloc(1, sizeof(FieldNode));
            nodePairs[1][i]->value->color = nodePairs[0][i]->value->color;
        }
        else
        {
            nodePairs[1][i] = nodePairs[1][0];
        }
    }
    for (int i = 1; i < b->size; i++)
    {
        int newNeighborsSize = 0;
        FieldNode **remappedNeighbors = calloc(nodePairs[0][i]->value->neighborsSize, sizeof(FieldNode));
        for (int j = 0; j < nodePairs[0][i]->value->neighborsSize; j++)
        {
            bool haveRoot = false;
            for (int k = 0; k < b->size; k++)
            {
                if (nodePairs[0][i]->value->neighbors[j] == nodePairs[0][k]->value)
                {
                    if (nodePairs[1][i] != nodePairs[1][0])
                    {
                        remappedNeighbors[newNeighborsSize] = nodePairs[1][k]->value;
                        newNeighborsSize++;
                    }
                    else
                    {
                        if (!haveRoot)
                        {
                            remappedNeighbors[newNeighborsSize] = nodePairs[1][k]->value;
                            newNeighborsSize++;
                        }
                    }
                }
            }
        }

        nodePairs[1][i] = realloc(remappedNeighbors, newNeighborsSize * sizeof(FieldNode));
    }

    FieldList *newBoard = calloc(1, sizeof(FieldList));

    return newBoard;
}

//Heuristc

bitColor takeNeighbors(FieldNode *node)
{
    bitColor colors = empty;
    if (node != NULL)
    {
        for (int i = 0; i < node->neighborsSize; i++)
        {
            colors = colors | (node->neighbors)[i]->color;
        }
    }
    else
        colors = empty;
    return colors;
}
int neighborsCalculator(FieldList *b, int gameColors)
{
    bitColor *colors = calloc(1, sizeof(colors));
    colors[0] = takeNeighbors(b->first->value);
    int colorGroups = 1;
    if (b->first->next != NULL)
    {
        for (FieldListNode *i = (b->first->next); i->next != NULL; i = i->next)
        {
            bitColor newColors = takeNeighbors(i->value);
            bool insert = true;
            for (int j = 0; j < colorGroups; j++)
            {
                if ((colors[j] | newColors) == colors[j])
                {
                    insert = false;
                    break;
                }
                else
                {
                    if ((colors[j] | newColors) == newColors)
                    {
                        insert = false;
                        colors[j] = newColors;
                        break;
                    }
                }
            }
            if (insert)
            {
                colorGroups++;
                colors = realloc(colors, colorGroups * sizeof(bitColor));
                colors[colorGroups - 1] = newColors;
            }
        }
    }
    // TODO free colors, free bitColor

    return colorGroups;
}

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}
int colorsCalculator(FieldList *fieldList, int gameColorsNumber)
{
    bitColor colors = empty;
    for (FieldListNode *node = fieldList->first; node->next != NULL; node = node->next)
    {
        colors = colors | node->value->color;
    }
    int result = 0;
    for (int i = 0; i < gameColorsNumber; i++)
    {
        if ((colors >> i) % 2)
        {
            result++;
        }
    }
    return result;
}
int h(FieldList *b, int numColors, int currentNumColors)
{
    int n = neighborsCalculator(b, numColors);
    int c = currentNumColors - 1;
    return max(n, c);
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
            FieldList *newBoard = paintBoard(step->board, colorStep);
            if (newBoard->size < step->board->size)
            {
                nextSteps[i]->board = newBoard;
                nextSteps[i]->prevStep = step;
                nextSteps[i]->colorStep = colorStep;
                nextSteps[i]->g = step->g + 1;
                nextSteps[i]->h = h(nextSteps[i]->board, gameColors, colorsCalculator(nextSteps[i]->board, gameColors));
                nextSteps[i]->f = nextSteps[i]->g + nextSteps[i]->h;
                enqueueStep(nextSteps[i], q);
            }
            else
            {
                freeFieldList(newBoard); //TODO freeFieldList
            }
        }
    }
    freeFieldList(step->board);
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
                bool enqueued = false;
                for (QueueNode *node = q->first->next; (node->next->next != NULL) && !enqueued; node = node->next)
                {
                    if ((node->value->f < weight) && (node->next->value->f > weight))
                    {
                        QueueNode *aux = node->next;
                        node->next = newNode;
                        newNode->next = aux;
                        q->size++;
                        enqueued = true;
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
FieldList* convertBoardToGraph(int **boartM)
{
 FieldList* convertedBoard= calloc(1, sizeOf(FieldList));
 convertedBoard-
}
FieldNode** searchField(int line, int column,int prevColor, FieldNode ***checkedField)
{
    if (neighbor->searchColor == b->fields[line][column])
    {
        if (checkedField[line][column]!=NULL)
        {
            checkedField[line][column] = true;

            int neighborSize=0;
            for (int i = -1; i < 2; i++)
            {
                if ((line + i >= 0) && (line + i < b->lines))
                {
                    for (int j = -1; j < 2; j++)
                    {
                        if ((column + j >= 0) && (column + j < b->columns))
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