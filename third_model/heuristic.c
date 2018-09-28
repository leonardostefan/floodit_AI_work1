
#include "heuristic.h"
//Heuristc

StepQueue *mainQueue;
StepQueue *secondQueue;

bitColor takeNeighbors(FieldNode *node, FieldList *b)
{
    bitColor colors = empty;
    if (node != NULL)
    {
        int id;
        for (int n = 0; n < node->neighborsSize; n++)
        {
            id = (node->neighborsId)[n];
            if (b->list[id] != NULL)
            {

                colors = colors | b->list[id]->color;
            }
        }
    }
    else
        colors = empty;
    return colors;
}
int neighborsCalculator(FieldList *b, int gameColors)
{
    bitColor *colors = calloc(b->realSize, sizeof(colors));
    int colorGroups = 0;

    for (int id = 0; id < b->size; id++)
    {
        bool insert = true;
        bitColor newColors;
        if (b->list[id] != NULL)
        {
            newColors = takeNeighbors(b->list[id], b);
            for (int j = 0; j < colorGroups; j++)
            {
                if ((newColors) == colors[j])
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
        }
        if (insert)
        {
            colors[colorGroups] = newColors;
            colorGroups++;
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
    for (int id = 0; id < fieldList->size; id++)
    {
        if (fieldList->list[id] != NULL)
        {
            colors = colors | fieldList->list[id]->color;
        }
    }
    int result = 0;
    for (int i = 0; i <= gameColorsNumber; i++)
    {
        if ((colors >> (i)) % 2)
        {
            result++;
        }
    }
    return result;
}
int setH(int *h, FieldList *b, int numColors)
{

    int size = b->realSize - 1;
    int n = neighborsCalculator(b, numColors) - 1;
    int c = colorsCalculator(b, numColors) - 1;

    if (size <= 0 || c <= 0 || n <= 0)
    {
        if (size != 0 || c != 0 || n != 0)
        {
            printf("\nFalha na heuritica\nsize==%d c==%d n==%d", size, c, n);
        }
    }

    if (c > 8)
    {
        *h = size;
        return 0;
    }
    else
    {
        if (c > 4)
        {
            *h = n;
            return 1;
        }
        else
        {
            *h = (n);
            return 1;
        }
    }
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

void expandNode(Step *eStep, int gameColors)
{
    Step *newStep;
    bool isError = true;
    for (int i = 0; i < gameColors; i++)
    {
        int colorStep = i + 1;
        fprintf(stderr,"Cor: %d \n", colorStep);
        if (colorStep != eStep->colorStep)
        {

            newStep = calloc(1, sizeof(Step));
            FieldList *newBoard = paintBoard(eStep->board, colorStep);
            fprintf(stderr,"\nTeste modificação...");
            if (newBoard != NULL && (newBoard->realSize < eStep->board->realSize))
            {
                fprintf(stderr,"entrou....\n");
                newStep->board = newBoard;
                newStep->prevStep = eStep;
                newStep->colorStep = colorStep;
                newStep->g = eStep->g + 1;
                int queue = setH(&(newStep->h), newStep->board, gameColors);
                newStep->f = newStep->g + newStep->h;

                fprintf(stderr,"enfileirando...\n");

                if (queue == 0)
                {
                    enqueueStep(newStep, mainQueue);
                    isError = false;
                }
                else
                {
                    enqueueStep(newStep, secondQueue);
                    isError = false;
                }
            }
            else
            {
                printf("errow\n");
                if (newBoard != NULL && (newBoard->realSize >= eStep->board->realSize))
                {
                    breakDebug();
                }
                freeFieldList(newBoard); //TODO freeFieldList
            }
        }
    }
    if (isError)
    {
        breakDebug();
    }
    freeFieldList(eStep->board);
}

int *findSolution(Board *mainBoard, int numColors)
{
    mainQueue = calloc(1, sizeof(StepQueue));
    mainQueue->size = 0;
    secondQueue = calloc(1, sizeof(StepQueue));
    mainQueue->size = 0;

    Step *firstStep = calloc(1, sizeof(Step));
    firstStep->board = convertBoardToGraph(mainBoard);
    // printGraph(firstStep->board);
    firstStep->prevStep = NULL;
    firstStep->h = 1;
    firstStep->f = 0;
    firstStep->g = 0;
    firstStep->colorStep = (mainBoard->fields)[0][0];

    Step *aux = firstStep;
    int expandedNodes = 0;
    expandNode(aux, numColors);
    printGraph(aux->board);

    while (aux->h > 0)
    {
        if (secondQueue->size > 0)
        {
            aux = dequeueStep(secondQueue);
        }
        else
        {
            aux = dequeueStep(mainQueue);
        }
        printGraph(aux->board);
        expandNode(aux, numColors);
        expandedNodes++;
        printf("\nNÓS EXPANDIDOS: %d ", expandedNodes);
    }

    int *result = callback(aux);

    printf("\nnumero de passos: %d\n", aux->f);
    for (int i = 0; i < aux->f; i++)
    {
        printf("%d ", result[i]);
    }
    return result;
}

bool enqueueStep(Step *step, StepQueue *q)
{
    int weight = step->f;
    fprintf(stderr,"É aqui!?...");
    QueueNode *newNode = malloc(1 * sizeof(QueueNode));
    fprintf(stderr,"é não");
    newNode->next=0;
    newNode->value = step;
    if (q->size > 0)
    { //Se n vai ser o primeiro da lista
        if (q->first->value->f <= weight)
        { //se vai ser o ultimo da lista
            if (q->last->value->f <= weight)
            {
                q->last->next = newNode;
                q->last = newNode;
                q->size++;
                return 3;
            }
            //se esta no meio da lista (nem primeiro, nem ultimo)
            else
            {
                for (QueueNode *node = q->first; (node->next != NULL); node = node->next)
                {
                    if ((node->value->f <= weight) && (node->next->value->f > weight))
                    {
                        QueueNode *aux = node->next;
                        node->next = newNode;
                        newNode->next = aux;
                        q->size++;
                        return 2;
                    }
                }
            }
        }
        //se vai ser o primeiro da lista
        else
        {
            newNode->next = q->first;
            q->first = newNode;
            q->size++;
            return 1;
        }
    }
    //se a lista esta vazia
    else
    {
        q->first = newNode;
        q->last = newNode;
        q->size++;
        return -1;
    }
    //se nao enfileirou
    return 0;
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
        else
        {

            return NULL;
        }
    }
}
