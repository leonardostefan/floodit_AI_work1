#ifndef __flooditAi__
#define __flooditAi__
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define bool int
#define false 0
#define true !false

typedef struct Game{
    int height, width;
    int numColors;de
    StepGraph board;
    
};

typedef struct Board
{   
    int lines, columns;
    int **fields;
};

typedef struct Step
{
    Board* board;
    Step* prevStep;
    Step** nextSteps;
    int colorStep;
    int f, g, h;

};

typedef struct StepGraph{
    Step root;

};

typedef struct Neighbor
{
    int searchColor;
    bool *color;
    int size;
};

void expandNode(Step);
int colorsCalculator(Board* b);
int neighborCalculator(Board* b);
int h(Board b);



#endif
