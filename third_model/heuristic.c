
#include "heuristic.h"
//Heuristc

StepQueue *mainQueue;
StepQueue *secondQueue;

//*Globais para testes
bool admissivel = true;
bool consistente = true;
//

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
    int r = 0;
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
    // for(int i=0; i<colorGroups;i++){
    //     for (int j=0; j<gameColors;j++){
    //         if((colors[i]>>j)%2){
    //             r++;
    //         }
    //     }
    // }
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
    static bool first = true;
    int size = b->realSize - 1;
    int n = neighborsCalculator(b, numColors);
    int c = colorsCalculator(b, numColors) - 1;
    int sqrtR = sqrt(size * n);
    DEBUG_H(if (size <= 0 || c <= 0 || n <= 0) {
        if (size != 0 || c != 0 || n != 0)
        {
            printf("\nFalha na heuritica\nsize==%d c==%d n==%d", size, c, n);
        }
    })
    int kN = 4, kSize = 1;

    // *h = n;
    // return 0;
    if (size * c * c > (4*4*200))
    {
        *h = (kN * n + kSize * size) / (kN + kSize);
    }
    else
    {
        if (first)
        first = false;
        if (size * c * c > 1600)
        {
            *h = sqrtR;
        }
        else
        {
            *h = (n);
        }
    }
    if (size < 1000 && c < 8)
    {
            DEBUG_H(fprintf(stderr, "\nNumero de nodos  c<7 : %d", size))
        return 1;
    }
    else
        return 0;
}

int *callback(Step *finalStep)
{
    int *result = calloc(finalStep->f + 1, sizeof(int));
    Step *aux = finalStep;
    int teste = 0;
    float *error = calloc(finalStep->f + 1, sizeof(float));
    float md = 0;
    for (int i = finalStep->f - 1; aux->prevStep != NULL; i--)
    {
        teste++;
        result[i] = aux->colorStep;
        error[i] = aux->f - finalStep->f;
        md += error[i];
        aux = aux->prevStep;
    }
    {
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
    }

    DEBUG(if (finalStep->f < teste) {
        breakDebug();
    })
    return result;
}

void expandNode(Step *eStep, int gameColors)
{
    Step *newStep;
    bool isError = true;
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
                DEBUG(printf("errow\n");
                      if (newBoard != NULL && (newBoard->realSize >= eStep->board->realSize)) {
                          breakDebug();
                      })
                freeFieldList(newBoard); //TODO freeFieldList
            }
        }
    }
    DEBUG(
        if (isError) {
            breakDebug();
        })
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
    fprintf(stderr, "Nodos iniciais: %d", aux->board->realSize);
    expandNode(aux, numColors);
    // printGraph(aux->board);
    bool startSecond = false;

    while (aux->h > 0)
    {
        if (startSecond || secondQueue->size > 10 || mainQueue->size<1)
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
