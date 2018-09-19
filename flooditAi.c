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

Board *initaMatrixBoard;

//Function to paint the first field meging the neighbors of the affected nodes(and painted node) to a new root node
FieldListNode *mergeNodes(FieldListNode *root, FieldListNode **affectedNodes, int totalNodes)
{
    FieldListNode *newRoot = calloc(1, sizeof(FieldListNode));
    newRoot->next=NULL;
    newRoot->value = calloc(1, sizeof(FieldNode));
    newRoot->value->color = affectedNodes[0]->value->color;
    newRoot->value->neighborsSize = totalNodes;
    FieldNode** newNeighbors=calloc(totalNodes+3, sizeof(FieldNode*));
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
       
                newNeighbors[realSize] = root->value->neighbors[i];
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
                    bool contain = false;
                    for (int n = 0; n < realSize; n++)
                    {
                        if (affectedNodes[i]->value->neighbors[j] == newNeighbors[n])
                        {
                            printf("node:%04X \n color: %d\n", newNeighbors[n], newNeighbors[n]->color );
                            contain = true;
                        }
                    }
                    if (!contain)
                    {
                        newNeighbors[realSize] = affectedNodes[i]->value->neighbors[j];
                        realSize++;
                    }
                }
            }
        }
    }
    // newNeighbors= reallocarray(newNeighbors, realSize , sizeof(FieldNode *));
    newRoot->value->neighbors =newNeighbors;
    newRoot->value->neighborsSize = realSize;

    return newRoot;
}

//Create and remap an new board field list

FieldList *paintBoard(FieldList *b, int nextColor)
{

    bitColor newColor = (1 << nextColor);
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
        if (affectedNodes!=NULL){
            // free(affectedNodes);
        }
        return NULL;
    }
    //Create nodes
    FieldListNode ***nodePairs = calloc(2, sizeof(FieldListNode **));
    nodePairs[0] = calloc(b->size, sizeof(FieldListNode *));
    nodePairs[1] = calloc(b->size, sizeof(FieldListNode *));
    FieldListNode *oldPairAux = b->first;
    nodePairs[0][0] = oldPairAux;
    nodePairs[1][0] = mergeNodes(oldPairAux, affectedNodes, b->size);
    for (int i = 1; i < b->size; i++)
    {
        oldPairAux = oldPairAux->next;
        nodePairs[0][i] = oldPairAux;

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

            nodePairs[1][i] = calloc(1, sizeof(FieldListNode));
            nodePairs[1][i]->next=NULL;
            nodePairs[1][i]->value = calloc(1, sizeof(FieldNode));
            nodePairs[1][i]->value->color = nodePairs[0][i]->value->color;
        }
        else
        {
            nodePairs[1][i] = nodePairs[1][0];
        }
    }
//Remmap new neighbors
    for (int n =0; n< nodePairs[1][0]->value->neighborsSize; n++){
        for (int p=0 ;p<b->size;p++){
            if(nodePairs[1][0]->value->neighbors[n]==nodePairs[0][p]->value){
                nodePairs[1][0]->value->neighbors[n]=nodePairs[1][p]->value;
            }
        }
    }


    
    
    for (int i = 1; i < b->size; i++)
    {
        if (nodePairs[1][i] != nodePairs[1][0]){


            int newNeighborsSize = 0;
            FieldNode **remappedNeighbors = calloc(nodePairs[0][i]->value->neighborsSize, sizeof(FieldNode*));
            for (int neigh = 0; neigh < nodePairs[0][i]->value->neighborsSize; neigh++)
            {
                bool haveRoot = false;
                for (int pairN = 0; pairN < b->size; pairN++)
                {
                    if (nodePairs[0][i]->value->neighbors[neigh] == nodePairs[0][pairN]->value)
                    {
                        if (nodePairs[1][pairN] != nodePairs[1][0])
                        {
                            remappedNeighbors[newNeighborsSize] = nodePairs[1][pairN]->value;
                            newNeighborsSize++;
                        }
                        else
                        {
                            if (!haveRoot)
                            {
                                remappedNeighbors[newNeighborsSize] = nodePairs[1][0]->value;
                                newNeighborsSize++;
                                haveRoot=true;
                            }
                        }
                    }
                }
            }

            nodePairs[1][i]->value->neighbors = remappedNeighbors;
            nodePairs[1][i]->value->neighborsSize=newNeighborsSize;
        }
    }

    FieldList *newBoard = calloc(1, sizeof(FieldList));
    newBoard->first = nodePairs[1][0];
    newBoard->size = 1;
    for (int i = 1; i < b->size; i++)
    {
        if (nodePairs[1][i] != newBoard->first)
        {
            FieldListNode *last;
            for (last = newBoard->first; last->next != NULL; last = last->next)
                ;
            last->next = nodePairs[1][i];
            newBoard->size++;
        }

    }
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
                //colors = reallocarray(colors, colorGroups , sizeof(bitColor));
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
            if (newBoard != NULL && (newBoard->size < step->board->size))
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
                    if ((node->value->f <= weight) && (node->next->value->f > weight))
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
        }else{

        return NULL;
        }
    }
}
FieldList *convertBoardToGraph(Board *boartM)
{
    initaMatrixBoard = boartM;

    FieldList *convertedBoard = calloc(1, sizeof(FieldList));
    FieldListNode *newNode = NULL;
    FieldListNode *lastNode = NULL;
    FieldNode ***nodeBoard;
    nodeBoard = calloc(initaMatrixBoard->lines, sizeof(FieldNode **));
    for (int i = 0; i < initaMatrixBoard->lines; i++)
    {
        nodeBoard[i] = calloc(initaMatrixBoard->columns, sizeof(FieldNode *));
    }

    for (int i = 0; i < initaMatrixBoard->lines; i++)
    {
        for (int j = 0; j < initaMatrixBoard->columns; j++)
        {
            if (nodeBoard[i][j] == NULL)
            {
                nodeBoard[i][j] = calloc(1, sizeof(FieldNode));
                nodeBoard[i][j]->color = 1 << initaMatrixBoard->fields[i][j];
                nodeBoard[i][j]->neighbors=NULL;
                if (newNode == NULL)
                {
                    newNode = calloc(1, sizeof(FieldListNode));
                    newNode->next=NULL;
                    convertedBoard->first = newNode;
                    convertedBoard->size = 1;
                }
                else
                {
                    newNode = calloc(1, sizeof(FieldListNode));

                    lastNode->next = newNode;
                    convertedBoard->size++;
                }
                newNode->value = nodeBoard[i][j];
                lastNode = newNode;

                for (int k = -1; k <= 1; k++)
                {
                    if ((i + k >= 0) && (i + k < initaMatrixBoard->lines))
                    {
                        for (int l = -1; l <= 1; l++)
                        {
                            if ((j + l >= 0) && (j + l < initaMatrixBoard->columns))
                            {
                                int lineR = i + k, colmunR = j + l;
                                searchNodes(lineR, colmunR, nodeBoard[i][j], nodeBoard);
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < initaMatrixBoard->lines; i++)
    {
        for (int j = 0; j < initaMatrixBoard->columns; j++)
        {

            for (int k = -1; k <= 1; k++)
            {
                if ((i + k >= 0) && (i + k < initaMatrixBoard->lines))
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        if ((j + l >= 0) && (j + l < initaMatrixBoard->columns))
                        {
                            printf("linkando: [%d][%d] [%d][%d] \n", i, j, i + k, j + l);
                            linkNeighbors(nodeBoard[i][j], nodeBoard[i + k][j + l]);
                        }
                    }
                }
            }
            printf("\n");
        }
    }
    printNodeMatrix(nodeBoard);
    return convertedBoard;
}
void searchNodes(int line, int column, FieldNode *groupNode, FieldNode ***board)
{
    if (groupNode != NULL && board[line][column] == NULL)
    {
        if (groupNode->color == (1 << initaMatrixBoard->fields[line][column]))
        {
            board[line][column] = groupNode;

            for (int i = -1; i <= 1; i++)
            {
                if ((line + i >= 0) && (line + i < initaMatrixBoard->lines))
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        if ((column + j >= 0) && (column + j < initaMatrixBoard->columns))
                        {

                            searchNodes(line + i, column + j, groupNode, board);
                        }
                    }
                }
            }
        }
    }
}
void linkNeighbors(FieldNode *searchNode, FieldNode *toLinkNode)
{
    if (toLinkNode != searchNode)
    {
        if (searchNode->neighbors != NULL)
        {

            bool inserted = false;
            for (int i = 0; (i < searchNode->neighborsSize) && (!inserted); i++)
            {
                if (searchNode->neighbors[i] == toLinkNode)
                {
                    inserted = true;
                }
            }

            if (!inserted)
            {
                // searchNode->neighbors = reallocarray(searchNode->neighbors, searchNode->neighborsSize , sizeof(FieldNode *));
                searchNode->neighbors[searchNode->neighborsSize] = toLinkNode;
                searchNode->neighborsSize++;
                printf("%p -> %p \n", searchNode, toLinkNode );
            }
        }
        else
        {
            int size= initaMatrixBoard->columns*initaMatrixBoard->lines;
            searchNode->neighborsSize = 1;
            searchNode->neighbors = calloc(size, sizeof(FieldNode *));
            searchNode->neighbors[0] = toLinkNode;
            printf("%p -> %p \n", searchNode, toLinkNode );
        }
    }
}

void freeFieldList(FieldList *b)
{
    // if (b != NULL)
    // {

    //     if (b->first != NULL)
    //     {
    //         FieldListNode *lastNode = NULL;
    //         for (FieldListNode *node = b->first; node->next != NULL; node = node->next)
    //         {
    //             if (lastNode != NULL)
    //             {
    //                 if (lastNode->value->neighbors != NULL)

    //                     free(lastNode->value->neighbors);
    //                 free(lastNode->value);
    //                 free(lastNode);
    //             }
    //             lastNode = node;
    //         }
    //     }
    // }
}
void printGraph(FieldList *grafo){
    FieldListNode *nodo = grafo->first;
    while(nodo){
        //printa
        fprintf(stderr, "%p[%d]:", nodo->value, nodo->value->color);
        for(int i = 0; i < nodo->value->neighborsSize; i++)
            fprintf(stderr, " %p[%d]", nodo->value->neighbors[i], nodo->value->neighbors[i]->color);
        nodo = nodo->next;
        fprintf(stderr, "\n");
    }
    printf("IMPRESSO\n");
}

void printNodeMatrix(FieldNode ***board){
    for (int i= 0; i< initaMatrixBoard->lines; i++){
        for (int j= 0; j< initaMatrixBoard->columns; j++){
           printf(" %p", board[i][j]);
        } 
        printf("\n");
    }
}