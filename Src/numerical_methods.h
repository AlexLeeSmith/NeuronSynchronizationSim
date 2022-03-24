/**
 * Header file for a program that implements multiple numerical methods for solving ordinary differential equations (ODE).
 */

#ifndef NUMERICAL_METHODS
#define NUMERICAL_METHODS

/** Structures **/
typedef struct {
    float x0;
    float xEnd;
    float *inits;
    float step;
    float transient;
} EqConditions;

typedef struct {
    float *x;
    float **approx;
    int funcCount;
    int stepCount;
} EqSolution;

/** Forward Declarations **/
void runEulers(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol);
void runRungeKutta(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol);
void writeSolution(char filename[], float x[], float approx[], int size, float transient);
void freeEqSolution(EqSolution *sol);

#endif