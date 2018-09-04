#ifndef __flooditAi__
#define __flooditAi__
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct Game{
    int height, width;
    int numColors;
    StepGraph board;
    
};

typedef struct Board
{
    int height, width;
    int **fields;
    int h;
};

typedef struct Step
{
    Board* board;
    Step* prevStep;
    Step** nextSteps;
    int colorStep;
    int spentStep;
};

typedef struct StepGraph{
    Step root;

};

typedef struct Neighbor
{
    bool *color;
    int size;
};

cvoid expandNode(Step);
int colorsCalculator(Board* b);
int neighborCalculator(Board* b);
int h(Board b);



#endif
