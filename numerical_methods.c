/**
 * This file implements the numerical methods header file.
 * 
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 1/30/22
 */

/** Preprocessor Directives **/
#include "numerical_methods.h"

#include <stdio.h>      // printf()
#include <stdlib.h>     // free()

/** Functions **/
/**
 * @brief Runs Euler's first-order numerical method for approximating ODEs.
 * 
 * @param getODEs a pointer to function that returns the result(s) of ODEs with given inputs.
 * @param cond a structure containing the input conditions.
 * @param sol a struture where the solution will be stored.
 */
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

/**
 * @brief Runs the fourth-order Runge-Kutta numerical method for approximating ODEs.
 * 
 * @param getODEs a pointer to function that returns the result(s) of ODEs with given inputs.
 * @param cond a structure containing the input conditions.
 * @param sol a struture where the solution will be stored.
 */
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
        
        // Calculate next step in the x direction.
        sol->x[curStep + 1] = sol->x[curStep] + cond->step;        
    }
}

/**
 * @brief prints the ODE approximation for each step. 
 * 
 * @param approx an array of approximations.
 * @param x an array of steps.
 * @param stepCount the number of steps (i.e., size of the two prior arrays - 1).
 * @param transient the x value to begin printing from.
 */
void printSolution(float approx[], float x[], int stepCount, float transient) {
    // Find the point to start printing from.
    int start = 0;
    for (int i = 0; i <= stepCount && !start; ++i) {
        if (x[i] >= transient) {
            start = i;
        }        
    }

    // Begin printing.
    for (int i = 0, j = start; j <= stepCount; ++i, ++j) {
        printf("%f\t%f\n", x[i], approx[j]);   
    }
    printf("\n");
}