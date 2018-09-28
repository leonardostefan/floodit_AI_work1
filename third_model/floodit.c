#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "floodit.h"

#define DEBUG 1
#define inlineF
// #define MAX_AFFECT_NODE 5

//*Globais para testes
bool admissivel = true;
bool consistente = true;
//

Board *initaMatrixBoard;

//Function to paint the first field meging the neighbors of the affected nodes(and painted node) to a new root node
FieldNode *mergeNodes(FieldNode *root, FieldNode **affectedNodes, int affectedSize, int totalNodes)
{
    FieldNode *newRoot = calloc(1, sizeof(FieldNode));
    newRoot->color = affectedNodes[0]->color;
    newRoot->neighborsSize = totalNodes;
    int *newNeighbors = calloc(totalNodes, sizeof(int));
    int realSize = 0;
    for (int i = 0; i < root->neighborsSize; i++)
    {
        bool isNotAffected = true;
        for (int j = 0; isNotAffected && (j < affectedSize); j++)
        {
            if ((affectedNodes[j] != NULL) && (root->neighborsId[i] == affectedNodes[j]->id))
            {
                isNotAffected = false;
            }
        }
        if (isNotAffected)
        {
            newNeighbors[realSize] = root->neighborsId[i];
            realSize++;
        }
    }
    for (int i = 0; i < affectedSize; i++)
    {
        if (affectedNodes[i] != NULL)
        {
            for (int j = 0; j < affectedNodes[i]->neighborsSize; j++)
            {
                if (affectedNodes[i]->neighborsId[j] != root->id)
                {
                    bool contain = false;
                    for (int n = 0; n < realSize; n++)
                    {
                        if (affectedNodes[i]->neighborsId[j] == newNeighbors[n])
                        {
                            contain = true;
                        }
                    }
                    if (!contain)
                    {
                        newNeighbors[realSize] = affectedNodes[i]->neighborsId[j];
                        realSize++;
                    }
                }
            }
        }
    }
    // newNeighbors= reallocarray(newNeighbors, realSize , sizeof(FieldNode *));
    if (totalNodes < realSize)
    {
        breakDebug();
    }

    newRoot->neighborsId = newNeighbors;
    newRoot->neighborsSize = realSize;

    return newRoot;
}

//Create and remap an new board field list

FieldList *paintBoard(FieldList *b, int nextColor)
{

    bitColor newColor = (1 << nextColor);
    if (newColor == b->list[0]->color)
    {
        return NULL;
    }
    printf("\nColorindo\n");

    bool changeBoard = false;
    printf("Verificando nós afetados\n");
    FieldNode **affectedNodes = calloc(b->realSize, sizeof(FieldNode *));
    int affectedSize = 0;
    for (int i = 0; i < b->list[0]->neighborsSize; i++)
    {
        int id = b->list[0]->neighborsId[i];
        if (b->list[id] != NULL && b->list[id]->color == newColor)
        {
            affectedNodes[affectedSize] = b->list[id];
            changeBoard = true;
            affectedSize++;
        }
    }

    if (!changeBoard)
    {
        if (affectedNodes != NULL)
        {
            // free(affectedNodes);
        }
        printf("Nop...\n");
        return NULL;
    }
    printf("Remap\n");
    //Create nodes
    FieldList *newList = calloc(1, sizeof(FieldList));
    newList->list = calloc(b->size, sizeof(FieldNode *));
    newList->size = b->size;

    newList->list[0] = mergeNodes(b->list[0], affectedNodes, affectedSize, b->realSize);
    newList->realSize = 1;

    for (int id = 1; id < b->size; id++)
    {
        bool isNotAffected = true;
        for (int i = 0; i < affectedSize; i++)
        {
            if (b->list[id] == affectedNodes[i])
            {
                isNotAffected = false;
            }
        }
        if (isNotAffected && (b->list[id] != NULL))
        {

            newList->list[id] = calloc(1, sizeof(FieldNode));
            newList->list[id]->color = b->list[id]->color;
            newList->realSize++;
        }

        else
        {
            newList->list[id] = NULL;
        }
    }
    //remmap
    for (int id = 1; id < b->size; id++)
    {
        if (newList->list[id] != NULL)
        {
            newList->list[id]->neighborsId = calloc(b->list[id]->neighborsSize + 1, sizeof(int));
            newList->list[id]->neighborsSize = 0;
            bool haveRoot = false;
            for (int n = 0; n < b->list[id]->neighborsSize; n++)
            {
                for (int a = 0; a < affectedSize; a++)
                {
                    if ((b->list[id]->neighborsId[n] == affectedNodes[a]->id || b->list[id]->neighborsId[n] == 0))
                    {
                        if (!haveRoot)
                        {
                            newList->list[id]->neighborsId[newList->list[id]->neighborsSize] = 0;
                            newList->list[id]->neighborsSize++;
                            haveRoot = true;
                        }
                    }
                    else
                    {
                        newList->list[id]->neighborsId[newList->list[id]->neighborsSize] = b->list[id]->neighborsId[n];
                        newList->list[id]->neighborsSize++;
                    }
                }
            }
            if (newList->list[id]->neighborsSize > b->list[id]->neighborsSize )
            {
                breakDebug();
                printf("Eita =(");
            }
        }
    }
    printf("COLORIUS\n");
    return newList;
}

//Structures

FieldList *convertBoardToGraph(Board *boartM)
{
    initaMatrixBoard = boartM;
    FieldNode ***nodeBoard;
    int totalNodes = 0;
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
                nodeBoard[i][j]->neighborsId = NULL;
                nodeBoard[i][j]->id = totalNodes;
                totalNodes++;

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
                            linkNeighbors(nodeBoard[i][j], nodeBoard[i + k][j + l], totalNodes);
                        }
                    }
                }
            }
        }
    }
    FieldList *resizedBoard = calloc(1, sizeof(FieldList));
    resizedBoard->list = calloc(totalNodes, sizeof(FieldNode));
    resizedBoard->size = totalNodes;
    resizedBoard->realSize = totalNodes;
    for (int id = 0; id < totalNodes; id++)
    {
        bool notInserted = true;
        for (int i = 0; i < initaMatrixBoard->lines && notInserted; i++)
        {
            for (int j = 0; j < initaMatrixBoard->columns && notInserted; j++)
            {
                if (nodeBoard[i][j]->id == id)
                {
                    resizedBoard->list[id] = nodeBoard[i][j];
                }
            }
        }
    }
    printNodeMatrix(nodeBoard);
    printGraph(resizedBoard);
    return resizedBoard;
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
void linkNeighbors(FieldNode *searchNode, FieldNode *toLinkNode, int totalNodes)
{
    if (toLinkNode != searchNode)
    {
        if (searchNode->neighborsId != NULL)
        {

            bool inserted = false;
            for (int i = 0; (i < searchNode->neighborsSize) && (!inserted); i++)
            {
                if (searchNode->neighborsId[i] == toLinkNode->id)
                {
                    inserted = true;
                }
            }

            if (!inserted)
            {
                searchNode->neighborsId[searchNode->neighborsSize] = toLinkNode->id;
                searchNode->neighborsSize++;
                // printf("%p -> %p \n", searchNode, toLinkNode );
            }
        }
        else
        {
            searchNode->neighborsSize = 1;
            searchNode->neighborsId = calloc(totalNodes, sizeof(FieldNode *));
            searchNode->neighborsId[0] = toLinkNode->id;
            // printf("%p -> %p \n", searchNode, toLinkNode );
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

//Debug functions
void printGraph(FieldList *grafo)
{
    printf("\n");

    char *cor_ansi[] = {"\x1b[0m",
                        "\x1b[31m", "\x1b[32m", "\x1b[33m",
                        "\x1b[34m", "\x1b[35m", "\x1b[36m",
                        "\x1b[37m", "\x1b[30;1m", "\x1b[31;1m",
                        "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m",
                        "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m"};

    for (int id = 0; id < grafo->size; id++)
        if (grafo->list[id] != NULL)
        {
            int color = log(grafo->list[id]->color) / log(2);

            printf("%s %x %s :{", cor_ansi[color], grafo->list[id], cor_ansi[0]);
            for (int jd = 0; jd < grafo->list[id]->neighborsSize; jd++)
            {
                int n = grafo->list[id]->neighborsId[jd];
                int nColor = log(grafo->list[n]->color) / log(2);
                printf("%s %x %s", cor_ansi[nColor], grafo->list[n], cor_ansi[0]);
            }
            printf("}\n");
        }
    printf("IMPRESSO\n");
}

void printNodeMatrix(FieldNode ***board)
{
    printf("\n");

    char *cor_ansi[] = {"\x1b[0m",
                        "\x1b[31m", "\x1b[32m", "\x1b[33m",
                        "\x1b[34m", "\x1b[35m", "\x1b[36m",
                        "\x1b[37m", "\x1b[30;1m", "\x1b[31;1m",
                        "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m",
                        "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m"};

    for (int i = 0; i < initaMatrixBoard->lines; i++)
    {
        for (int j = 0; j < initaMatrixBoard->columns; j++)
        {
            int color = log(board[i][j]->color) / log(2);
            printf("%s %x %s", cor_ansi[color], board[i][j], cor_ansi[0]);
        }
        printf("\n");
    }
}
int breakDebug()
{
    static int i = 0;
    printf(" DEU RUIM AQUI CARAIO\n");
    i++;
    return i;
}