/**
 * This file implements the numerical methods header file.
 * 
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 1/30/22
 */

/** Preprocessor Directives **/
#include "numerical_methods.h"

#include <stdio.h>      // fprintf(), FILE, fopen(), fclose(), perror()
#include <stdlib.h>     // EXIT_FAILURE

/** Functions **/
/**
 * @brief Runs Euler's first-order numerical method for approximating ODEs.
 * 
 * @param getODEs a pointer to function that returns the result(s) of ODEs with given inputs.
 * @param cond a structure containing the input conditions.
 * @param sol a struture where the solution will be stored.
 */
// void runEulers(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol) {
//     // Assign initial values for each function and x.
//     float inputs[sol->funcCount];
//     for (int i = 0; i < sol->funcCount; ++i) {
//         sol->approx[i][0] = cond->inits[i];
//         inputs[i] = cond->inits[i];
//     }
//     sol->x[0] = cond->x0;
    
//     // Begin Euler's method.
//     float *slopes;
//     for (int curStep = 0; curStep < sol->stepCount; ++curStep) {
//         slopes = getODEs(inputs, sol->x[curStep]);
//         for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
//             // Calculate the next step for this function.
//             sol->approx[curFunc][curStep + 1] = sol->approx[curFunc][curStep] + cond->step * slopes[curFunc];

//             // Update the input for the next step.
//             inputs[curFunc] = sol->approx[curFunc][curStep + 1];
//         }
//         sol->x[curStep + 1] = sol->x[curStep] + cond->step;
//     }
// }

/**
 * @brief Runs the fourth-order Runge-Kutta numerical method for approximating ODEs.
 * 
 * @param getODEs a pointer to function that returns the result(s) of ODEs with given inputs.
 * @param cond a structure containing the input conditions.
 * @param sol a struture where the solution will be stored.
 */
void runRungeKutta(float *(*getODEs)(float [], float), EqConditions *cond, EqSolution *sol) {
    // Assign initial values for each function and x.
    float inputs[2][sol->funcCount + 2]; // plus 2 to add coupling and different s
    for (int i = 0; i < sol->funcCount; ++i) {
        sol->approx[0][i][0] = cond->inits[i];
        sol->approx[1][i][0] = cond->inits[i];
        inputs[0][i] = cond->inits[i];
        inputs[1][i] = cond->inits[i];
    }
    inputs[0][sol->funcCount] = 3.6;
    inputs[1][sol->funcCount] = 4.2;
    inputs[0][sol->funcCount + 1] = 0.0;
    inputs[1][sol->funcCount + 1] = 0.0;
    sol->x[0] = cond->x0;
    
    // Begin Runge-Kutta method.
    float k[4][2][sol->funcCount], *slopes[2];
    for (int curStep = 0; curStep < sol->stepCount; ++curStep) {
        for (int neuron = 0; neuron < 2; ++neuron) {
            // Calculate k1-4 for each function.
            for (int curK = 0; curK < 4; ++curK) {
                // Calculate inputs.
                for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
                    switch (curK) {
                        case 1:
                            inputs[neuron][curFunc] = sol->approx[neuron][curFunc][curStep] + 0.5 * k[0][neuron][curFunc];
                            break;
                        case 2:
                            inputs[neuron][curFunc] = sol->approx[neuron][curFunc][curStep] + 0.5 * k[1][neuron][curFunc];
                            break;
                        case 3:
                            inputs[neuron][curFunc] = sol->approx[neuron][curFunc][curStep] + k[2][neuron][curFunc];
                            break;
                    }
                }                      
                
                // Calculate curX.
                float curX = sol->x[curStep];
                switch (curK) {
                    case 1:
                    case 2:
                        curX += 0.5 * cond->step;
                        break;
                    case 3:
                        curX += cond->step;
                        break;
                }

                // Calculate slopes.
                slopes[neuron] = getODEs(inputs[neuron], curX);

                // Calculate curK.
                for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
                    k[curK][neuron][curFunc] = cond->step * slopes[neuron][curFunc];
                }
            }

            // Calculate approximation for each function.
            for (int curFunc = 0; curFunc < sol->funcCount; ++curFunc) {
                sol->approx[neuron][curFunc][curStep + 1] = sol->approx[neuron][curFunc][curStep] + (k[0][neuron][curFunc] + k[1][neuron][curFunc] + k[1][neuron][curFunc] + k[2][neuron][curFunc] + k[2][neuron][curFunc] + k[3][neuron][curFunc]) / 6.0;

                // Update the input for the next step.
                inputs[neuron][curFunc] = sol->approx[neuron][curFunc][curStep + 1];
            }
        }        
        inputs[0][sol->funcCount + 1] = sol->approx[1][0][curStep];
        inputs[1][sol->funcCount + 1] = sol->approx[0][0][curStep];
        
        // Calculate next step in the x direction.
        sol->x[curStep + 1] = sol->x[curStep] + cond->step;        
    }
}

/**
 * @brief writes the ODE approximation for each step to a file. 
 * 
 * @param filename the name of the file to write to.
 * @param x an array of steps.
 * @param approx an array of approximations.
 * @param size the size of the x and approx arrays.
 * @param transient the x value to begin printing from.
 */
void writeSolution(char filename[], float x[], float approx[], int size, float transient) {
    // Find the point to start printing from.
    int start = 0;
    for (int i = 0; i < size && !start; ++i) {
        if (x[i] >= transient) {
            start = i;
        }        
    }

    // Open output file for writing.
    FILE *outfile;
    if ((outfile = fopen(filename, "w")) == NULL) {
        perror("Write Solution");
        exit(EXIT_FAILURE);
    }

    // Begin writing.
    for (int i = 0, j = start; j < size; ++i, ++j) {
        fprintf(outfile, "%f\t%f\n", x[i], approx[j]);   
    }

    // Close ouput file.
    fclose(outfile);
}

/**
 * @brief Frees the heap memory allocated to a EqSolution struture.
 * 
 * @param sol the EqSolution struture to be freed.
 */
void freeEqSolution(EqSolution *sol) {
    for (int i = 0; i < sol->funcCount; ++i) {
        free(sol->approx[0][i]);
        free(sol->approx[1][i]);
    }
    free(sol->x);
}