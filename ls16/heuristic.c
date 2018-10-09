
#include "heuristic.h"
//Heuristc

StepQueue *mainQueue;
StepQueue *secondQueue;

time_t initalTime;
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
    for (int id = 1; id < b->size; id++)
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
                    bitColor unionColor = colors[j] | newColors;
                    if (unionColor == newColors)
                    {
                        insert = false;
                        colors[j] = newColors;
                        break;
                    }
                    else
                    {
                        if (unionColor == colors[j])
                        {
                            insert = false;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            insert = false;
        }
        DEBUG(if (b->realSize < colorGroups) {
            breakDebug();
        })

        if (insert)
        {
            colors[colorGroups] = newColors;
            colorGroups++;
        }
    }
    free(colors);
    return (colorGroups);
}

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}
int min(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}
int dijkstra(FieldList *b)
{
    int result = 0;
    int *distances1 = malloc(b->size * sizeof(int));
    int *distances2 = malloc(b->size * sizeof(int));
    distances1[0] = 0;
    distances2[0] = -1;
    for (int i = 1; i < b->size; i++)
    {
        distances1[i] = __INT_MAX__;
        distances2[i] = -1;
    }
    int openNodes = b->realSize;
    if (openNodes > 1)
    {
        while (openNodes)
        {
            int min = __INT_MAX__, minId = 0;
            for (int i = 0; i < b->size; i++)
            {
                if (b->list[i] != NULL && distances1[i] < min && distances2[i] == -1)
                {
                    min = distances1[i];
                    minId = i;
                }
            }
            FieldNode *selecetdNode = b->list[minId];
            for (int n = 0; n < selecetdNode->neighborsSize; n++)
            {
                int id = selecetdNode->neighborsId[n];
                if (b->list[id] != NULL)
                {
                    if (distances1[id] > distances1[minId] + 1)
                    {
                        distances1[id] = distances1[minId] + 1;
                    }
                }
            }
            distances2[minId] = distances1[minId];
            openNodes--;
        }
        int maxD = 0, maxId = 0;
        for (int i = 0; i < b->size; i++)
        {
            if (b->list[i] != NULL && distances2[i] > maxD)
            {
                maxD = distances2[i];
                maxId = i;
            }
        }
        result = distances2[maxId];
    }
    if (distances1 != NULL)
        free(distances1);
    if (distances2 != NULL)
        free(distances2);
    return result;
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
    int c = colorsCalculator(b, numColors) - 1;

    if (timeSpent() > warningTime)
    {
        *h = sqrt(size * c);
    }
    else
    {
        int d = dijkstra(b);
        int adm = max(d, c);
        int sqrtR = sqrt(size * adm);
        if(size<6000){

        *h = sqrt(sqrtR * adm);
        }else{
            *h = sqrtR;
        }
    }
    if (timeSpent() > warningTime || (size * c < 20))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int *callback(Step *finalStep)
{
    int *result = calloc(finalStep->f + 1, sizeof(int));
    Step *aux = finalStep;
    float *error = calloc(finalStep->f + 1, sizeof(float));
    DEBUG_H(float md = 0;)
    for (int i = finalStep->f - 1; aux->prevStep != NULL; i--)
    {
        result[i] = aux->colorStep;
        aux = aux->prevStep;
        DEBUG_H(
            {error[i] = aux->f - finalStep->f;
        md += error[i]; })
    }
    /* Calculo estatstico para verificar a proximidade 
    *da heuristica de H* e a estabilidade dela 
   DEBUG_H( {
        int minE = __INT_MAX__;
        int maxE = (-__INT_MAX__);
        float dp = 0;
        int n = finalStep->f;
        md = md / n;
        for (int i = n - 1; i >= 0; i--)
        {
            maxE = max(maxE, error[i]);
            minE = min(minE, error[i]);
            dp = (md - error[i]) * (md - error[i]);
        }
        for (int i = n - 1; i >= 0; i--)
        {
        }
        dp = sqrt(dp / n);
        printf("\nCallback: \nMaior erro: %d\nMenor: %d\nMédia: %f\nDesvio: %f\n", maxE, minE, md, dp);
    })
*/

    return result;
}

void expandNode(Step *eStep, int gameColors)
{

    Step *newStep;
    for (int i = 0; i < gameColors; i++)
    {
        int colorStep = i + 1;
        DEBUG(fprintf(stderr, "Cor: %d \n", colorStep);)
        if (colorStep != eStep->colorStep)
        {

            newStep = calloc(1, sizeof(Step));
            FieldList *newBoard = paintBoard(eStep->board, colorStep);
            DEBUG(fprintf(stderr, "\nTeste modificação...");)

            if (newBoard != NULL && (newBoard->realSize < eStep->board->realSize))
            {
                if ((timeSpent() < warningTime) && ((eStep->board->size) / (eStep->board->realSize)) > 1.2 && eStep->board->size > 100)
                {
                    DEBUG({fprintf(stderr, "\nredução   %d->%d \n", (eStep->board->size), (eStep->board->realSize));})
                    newBoard = reduceGraph(newBoard);
                }
                DEBUG(fprintf(stderr, "entrou....\n");)
                newStep->board = newBoard;
                newStep->prevStep = eStep;
                newStep->colorStep = colorStep;
                newStep->g = eStep->g + 1;
                int queue = setH(&(newStep->h), newStep->board, gameColors);
                newStep->f = newStep->g + newStep->h;

                DEBUG(fprintf(stderr, "enfileirando...\n");)

                if (queue == 0)
                {
                    enqueueStep(newStep, mainQueue);
                }
                else
                {
                    if (timeSpent() < warningTime)
                    {
                        enqueueStep(newStep, mainQueue);
                    }
                    enqueueStep(newStep, secondQueue);
                }
            }
            else
            {
                DEBUG(printf("errow\n");
                      if (newBoard != NULL && (newBoard->realSize >= eStep->board->realSize)) {
                          breakDebug();
                      })
                freeFieldList(newBoard);
            }
        }
    }
    
    freeFieldList(eStep->board);
}

int *findSolution(Board *mainBoard, int numColors)
{
    initalTime = time(NULL);
    mainQueue = calloc(1, sizeof(StepQueue));
    mainQueue->size = 0;
    secondQueue = calloc(1, sizeof(StepQueue));
    mainQueue->size = 0;

    Step *firstStep = calloc(1, sizeof(Step));
    firstStep->board = convertBoardToGraph(mainBoard);
    firstStep->prevStep = NULL;
    firstStep->h = 1;
    firstStep->f = 0;
    firstStep->g = 0;
    firstStep->colorStep = (mainBoard->fields)[0][0];

    Step *aux = firstStep;
    int expandedNodes = 0;
    DEBUG({fprintf(stderr, "Nodos iniciais: %d\n", aux->board->realSize);})
    expandNode(aux, numColors);

    while (aux->h > 0)
    {
        if ((timeSpent() > warningTime && secondQueue->size > 0))
        {
            aux = dequeueStep(secondQueue);
        }
        else
        {
            aux = dequeueStep(mainQueue);
        }
        DEBUG(printGraph(aux->board);)
        expandNode(aux, numColors);
        expandedNodes++;
        DEBUG(printf("\nNÓS EXPANDIDOS: %d ", expandedNodes);)
    }

    
    int *result = callback(aux);
    DEBUG({printf("\nnumero de passos: ");})
    printf("%d\n", aux->f);
    for (int i = 0; i < aux->f; i++)
    {
        printf("%d ", result[i]);
    }
    return result;
}

bool enqueueStep(Step *step, StepQueue *q)
{
    int weight = step->f;
    DEBUG(fprintf(stderr, "É aqui!?...");)
    QueueNode *newNode = malloc(1 * sizeof(QueueNode));
    DEBUG(fprintf(stderr, "é não");)
    newNode->next = 0;
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
int timeSpent()
{
    return difftime(time(NULL), initalTime);
}
