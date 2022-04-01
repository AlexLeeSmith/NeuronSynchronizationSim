/**
 * Header file for a program that implements multiple numerical methods for solving ordinary differential equations (ODE).
 */

#ifndef NUMERICAL_METHODS
#define NUMERICAL_METHODS

#include "graph_manipulations.h"

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
    float ***approx; // TODO: make a triple pointer and add neuron count
    int neuronCount;
    int funcCount;  // TODO: add an adjacency matrix, will probably need to be read from a file
    int stepCount;
} EqSolution;

/** Forward Declarations **/
EqConditions initEqConditions(float x0, float xEnd, float step, float transient, int funcCount);
EqSolution initEqSolution(float x0, float xEnd, float step, int neuronCount, int funcCount);
void runEulers(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol);
void runRungeKutta(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol); // TODO: make this pass adjacent neuron values as input
void writeSolution(char filename[], float x[], float approx[], int size, float transient);
void freeEqConditions(EqConditions *cond);
void freeEqSolution(EqSolution *sol);   // TODO: update this to free each neuron

#endif