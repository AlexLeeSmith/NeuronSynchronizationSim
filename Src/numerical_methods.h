/**
 * @file numerical_methods.h
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief Header file for a program that implements multiple numerical methods for solving ordinary differential equations (ODE).
 * @version 0.1
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef NUMERICAL_METHODS
#define NUMERICAL_METHODS

#include "graph_manipulations.h"

/**
 * @brief A conditions structure which specifies the bounds of the approximation.
 */
typedef struct {
    /**
     * @brief The starting x position.
     */
    float x0;

    /**
     * @brief The final x position.
     */
    float xEnd;

    /**
     * @brief The initial values to start approximating each function.
     */
    float *inits;

    /**
     * @brief The size of each step.
     */
    float step;

    /**
     * @brief The x position in which the differential equation starts exhibiting its normal behavior.
     */
    float transient;
} EqConditions;

/**
 * @brief A solution structure which stores the results of the approximation.
 */
typedef struct {
    /**
     * @brief The array of x values.
     */
    float *x;

    /**
     * @brief The 3D array of approximations. Access using approx[neuronNum][functionNum][stepNum].
     */
    float ***approx;

    /**
     * @brief The number of neurons in the approximation.
     */
    int neuronCount;

    /**
     * @brief The number of functions in the approximation.
     */
    int funcCount;

    /**
     * @brief The number of steps in the approximation.
     */
    int stepCount;
} EqSolution;

/**
 * @brief Initializes and allocates memory for a conditions struture.
 * 
 * @param x0 the starting x position.
 * @param xEnd the final x position.
 * @param step the size of each step.
 * @param transient the x position in which the differential equation starts exhibiting its normal behavior.
 * @param funcCount the number of functions to be approximated within getODEs().
 * @return EqConditions - the initialized conditions structure.
 */
EqConditions initEqConditions(float x0, float xEnd, float step, float transient, int funcCount);

/**
 * @brief Initializes and allocates memory for a solution struture.
 * 
 * @param x0 the starting x position.
 * @param xEnd the final x position.
 * @param step the size of each step.
 * @param neuronCount The number of neurons in the approximation.
 * @param funcCount the number of functions to be approximated within getODEs().
 * @return EqSolution - the initialized solution structure.
 */
EqSolution initEqSolution(float x0, float xEnd, float step, int neuronCount, int funcCount);

// /**
//  * @brief Runs Euler's first-order numerical method for approximating ODEs.
//  * 
//  * @param getODEs a pointer to function that returns the result(s) of ODEs with given inputs.
//  * @param cond a structure containing the input conditions.
//  * @param sol a struture where the solution will be stored.
//  */
// void runEulers(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol);

/**
 * @brief Runs the fourth-order Runge-Kutta method for numerically approximating ordinary differential equations.
 * 
 * @param getODEs a pointer to function that returns the result(s) of ODEs with given inputs.
 * @param cond the input conditions.
 * @param graph the input graph.
 * @param funcCount the number of functions to be approximated within getODEs().
 * @return EqSolution - the approximation with the giving inputs.
 */
EqSolution runRungeKutta(void (*getODEs)(int neuronCount, float inputs[][neuronCount], float curX, float weights[], int myNeuron, float result[]), EqConditions *cond, Graph *graph, int funcCount);

/**
 * @brief writes the ODE approximation for each step to a file. 
 * 
 * @param filename the name of the file to write to.
 * @param x an array of steps.
 * @param approx an array of approximations.
 * @param size the size of the x and approx arrays.
 * @param transient the x value to begin printing from.
 */
void writeSolution(char *filename, float x[], float approx[], int size, float transient);

/**
 * @brief Frees the dynamic/heap memory allocated to a conditions structure.
 * 
 * @param cond the conditions to be freed.
 */
void freeEqConditions(EqConditions *cond);

/**
 * @brief Frees the dynamic/heap memory allocated to a solution structure.
 * 
 * @param sol the solution to be freed.
 */
void freeEqSolution(EqSolution *sol);

#endif