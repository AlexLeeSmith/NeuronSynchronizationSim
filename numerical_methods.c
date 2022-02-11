/**
 * This program implements multiple numerical methods for solving ordinary differential equations.
 * 
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 1/30/22
 */

/** Preprocessor Directives **/
#include <stdio.h>      // printf()
#include <math.h>       // ceil(), exp()
#include <stdlib.h>     // exit(), EXIT_SUCCESS

#define xR -1.56F
#define r 0.006F
#define I 3.1F

/** Global Variables **/
float s = 1.5;

/** Structures **/
typedef struct {
    float x0;
    float xEnd;
    float *inits;
    float step;
} EqConditions;

typedef struct {
    float **approx;
    float *x;
    int funcCount;
    int stepCount;
} EqSolution;

/** Forward Declarations **/
void runEulers(float *(*)(float [], float), EqConditions *, EqSolution *);
void runRungeKutta(float *(*)(float [], float), EqConditions *, EqSolution *);
void printSolution(float [], float [], int);
void freeEqSolution(EqSolution *);
float *getODEs1(float [], float);
float *getODEs2(float [], float);

/** Functions **/
int main(int argc, char const *argv[]) {
    EqConditions cond = {
        .x0 = 0.0, 
        .xEnd = 4.0, 
        .inits = (float[]){0.0, 0.0, 0.0},
        .step = 0.5
    };
    int stepCount = ceil((cond.xEnd - cond.x0) / cond.step);
    int numBytes = stepCount * sizeof(float);
    EqSolution sol1 = {
        .approx = (float *[]){malloc(numBytes), malloc(numBytes), malloc(numBytes)},
        .x = malloc(numBytes),
        .funcCount = 3,
        .stepCount = stepCount
    };
    EqSolution sol2 = {
        .approx = (float *[]){malloc(numBytes), malloc(numBytes), malloc(numBytes)},
        .x = malloc(numBytes),
        .funcCount = 3,
        .stepCount = stepCount
    };
    
    // Run the numerical methods.
    runEulers(&getODEs2, &cond, &sol1);
    runRungeKutta(&getODEs2, &cond, &sol2);

    // Print results.
    printSolution(sol1.approx[0], sol1.x, sol1.stepCount);
    printSolution(sol2.approx[0], sol2.x, sol2.stepCount);

    // Free heap memory and exit.
    freeEqSolution(&sol1);
    freeEqSolution(&sol2);
    exit(EXIT_SUCCESS);
}


void runEulers(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol) {
    // Assign initial values for each function and x.
    float inputs[sol->funcCount];
    for (int i = 0; i < sol->funcCount; ++i) {
        sol->approx[i][0] = cond->inits[i];
        inputs[i] = cond->inits[i];
    }
    sol->x[0] = cond->x0;
    
    // Begin Euler's method.
    float *slopes;
    for (int curStep = 0; curStep < sol->stepCount; ++curStep) {
        slopes = getODEs(inputs, sol->x[curStep]);
        for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
            // Calculate the next step for this function.
            sol->approx[curFunc][curStep + 1] = sol->approx[curFunc][curStep] + cond->step * slopes[curFunc];

            // Update the input for the next step.
            inputs[curFunc] = sol->approx[curFunc][curStep + 1];
        }
        sol->x[curStep + 1] = sol->x[curStep] + cond->step;
    }
}


void runRungeKutta(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol) {
    // Assign initial values for each function and x.
    float inputs[sol->funcCount];
    for (int i = 0; i < sol->funcCount; ++i) {
        sol->approx[i][0] = cond->inits[i];
        inputs[i] = cond->inits[i];
    }
    sol->x[0] = cond->x0;
    
    // Begin Runge-Kutta method.
    float k[4][sol->funcCount], *slopes;
    for (int curStep = 0; curStep < sol->stepCount; ++curStep) {
        // Calculate k1 for each function
        slopes = getODEs(inputs, sol->x[curStep]);
        for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
            k[0][curFunc] = cond->step * slopes[curFunc];
        }

        // Calculate k2 for each function
        for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
            inputs[curFunc] = sol->approx[curFunc][curStep] + 0.5 * k[0][curFunc];
        }
        slopes = getODEs(inputs, sol->x[curStep] + 0.5 * cond->step);
        for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
            k[1][curFunc] = cond->step * slopes[curFunc];
        }

        // Calculate k3 for each function
        for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
            inputs[curFunc] = sol->approx[curFunc][curStep] + 0.5 * k[1][curFunc];
        }
        slopes = getODEs(inputs, sol->x[curStep] + 0.5 * cond->step);
        for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
            k[2][curFunc] = cond->step * slopes[curFunc];
        }

        // Calculate k4 for each function
        for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
            inputs[curFunc] = sol->approx[curFunc][curStep] + k[2][curFunc];
        }
        slopes = getODEs(inputs, sol->x[curStep] + cond->step);
        for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
            k[3][curFunc] = cond->step * slopes[curFunc];
        }

        // Calculate approximation for each function
        for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
            sol->approx[curFunc][curStep + 1] = sol->approx[curFunc][curStep] + (k[0][curFunc] + k[1][curFunc] + k[1][curFunc] + k[2][curFunc] + k[2][curFunc] + k[3][curFunc]) / 6.0;
            
            // Update the input for the next step.
            inputs[curFunc] = sol->approx[curFunc][curStep + 1];
        }
        
        sol->x[curStep + 1] = sol->x[curStep] + cond->step;        
    }
}

void printSolution(float approx[], float x[], int stepCount) {
    for (int i = 0; i <= stepCount; i++) {
        printf("%f\t%f\n", x[i], approx[i]);
    }
    printf("\n");
}

void freeEqSolution(EqSolution *sol) {
    for (int i = 0; i < sol->funcCount; ++i) {
        free(sol->approx[i]);
    }
    free(sol->x);
}

float *getODEs1(float inputs[], float curX) {
    static float slopes[1];
    float x = curX;

    slopes[0] = exp(x);

    return slopes;
}

float *getODEs2(float inputs[], float curX) {
    static float slopes[3];
    float x = inputs[0];    // Voltage
    float y = inputs[1];    // Spiking
    float z = inputs[2];    // Bursting

    slopes[0] = y - (x*x*x) + (3*x*x) - z + I;
    slopes[1] = 1 - (5*x*x) - y;
    slopes[2] = r * (s * (x - xR) - z);

    return slopes;
}