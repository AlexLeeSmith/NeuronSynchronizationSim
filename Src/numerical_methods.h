/**
 * Header file for a program that implements multiple numerical methods for solving ordinary differential equations (ODE).
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 1/30/22
 */

#ifndef NUMERICAL_METHODS
#define NUMERICAL_METHODS

#include "graph_manipulations.h"

//* Structures **/
typedef struct {
    float x0;
    float xEnd;
    float *inits;
    float step;
    float transient;
} EqConditions;

typedef struct {
    float *x;
    float ***approx;
    int neuronCount;
    int funcCount;
    int stepCount;
} EqSolution;

/** Forward Declarations **/
EqConditions initEqConditions(float x0, float xEnd, float step, float transient, int funcCount);
EqSolution initEqSolution(float x0, float xEnd, float step, int neuronCount, int funcCount);
// void runEulers(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol);
EqSolution runRungeKutta(void (*getODEs)(int neuronCount, float inputs[][neuronCount], float curX, float weights[], int myNeuron, float result[]), EqConditions *cond, Graph *graph, int funcCount);
void writeSolution(char filename[], float x[], float approx[], int size, float transient);
void freeEqConditions(EqConditions *cond);
void freeEqSolution(EqSolution *sol);

#endif