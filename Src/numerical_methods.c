/**
 * @file numerical_methods.c
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief This file implements the numerical methods header file.
 * @version 0.1
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 */

#include "numerical_methods.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

EqConditions initEqConditions(float x0, float xEnd, float step, float transient, int funcCount) {
    EqConditions cond = {
        .x0 = x0,
        .xEnd = xEnd,
        .step = step,
        .transient = transient
    };

    // Allocate heap memory for the initial values array.
    if ((cond.inits = (float *) malloc(funcCount * sizeof(float))) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }

    // Set the initial values to 0.
    for (int i = 0; i < funcCount; ++i) {
        cond.inits[i] = 0.0;
    }

    return cond;
}

EqSolution initEqSolution(float x0, float xEnd, float step, int neuronCount, int funcCount) {
    // Calculate the step count and the number of bytes required.
    int stepCount = ceil((xEnd - x0) / step);
    int size = stepCount + 1;
    int numBytes = size * sizeof(float);

    EqSolution sol = {
        .neuronCount = neuronCount,
        .funcCount = funcCount,
        .stepCount = stepCount
    };

    // Allocate heap memory for the x array.
    if ((sol.x = (float *) malloc(numBytes)) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }

    // Allocate heap memory for the approximation of each neuron.
    if ((sol.approx = (float ***) malloc(neuronCount * sizeof(float **))) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }
    for (int neuron = 0; neuron < neuronCount; ++neuron) {
        // Allocate heap memory for the approximation of each function.
        if ((sol.approx[neuron] = (float **) malloc(funcCount * sizeof(float *))) == NULL) {
            perror("malloc() failure");
            exit(EXIT_FAILURE);
        }        
        for (int func = 0; func < funcCount; ++func) {
            // Allocate heap memory for the approximation of each step + 1 (for the initial position).
            if ((sol.approx[neuron][func] = (float *) malloc(numBytes)) == NULL) {
                perror("malloc() failure");
                exit(EXIT_FAILURE);
            }            
        }
    }    

    return sol;
}

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

EqSolution runRungeKutta(void (*getODEs)(int neuronCount, float inputs[][neuronCount], float curX, float weights[], int myNeuron, float result[]), EqConditions *cond, Graph *graph, int funcCount) {
    EqSolution sol = initEqSolution(cond->x0, cond->xEnd, cond->step, graph->vertexCount, funcCount);

    // Assign initial values for each function of each neuron and x.
    float inputs[funcCount][sol.neuronCount]; 
    for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
        for (int curFunc = 0; curFunc < funcCount; ++curFunc) {
            sol.approx[neuron][curFunc][0] = cond->inits[curFunc];
        }
    }
    sol.x[0] = cond->x0;

    // Begin Runge-Kutta method.
    float k[4][sol.neuronCount][funcCount], slopes[funcCount];
    for (int curStep = 0; curStep < sol.stepCount; ++curStep) {
        // Calculate k1-4 for each function.
        for (int curK = 0; curK < 4; ++curK) {
            // Calculate inputs of each neuron for the current k.
            for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
                for (int curFunc = 0; curFunc < funcCount; ++curFunc) {
                    switch (curK) {
                        case 0:
                            inputs[curFunc][neuron] = sol.approx[neuron][curFunc][curStep];
                            break;
                        case 1:
                            inputs[curFunc][neuron] = sol.approx[neuron][curFunc][curStep] + 0.5 * k[0][neuron][curFunc];
                            break;
                        case 2:
                            inputs[curFunc][neuron] = sol.approx[neuron][curFunc][curStep] + 0.5 * k[1][neuron][curFunc];
                            break;
                        case 3:
                            inputs[curFunc][neuron] = sol.approx[neuron][curFunc][curStep] + k[2][neuron][curFunc];
                            break;
                    }
                }
            }                                
            
            // Calculate curX.
            float curX = sol.x[curStep];
            switch (curK) {
                case 1:
                case 2:
                    curX += 0.5 * cond->step;
                    break;
                case 3:
                    curX += cond->step;
                    break;
            }

            for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
                // Calculate slopes.
                getODEs(sol.neuronCount, inputs, curX, graph->adjMatrix[neuron], neuron, slopes);

                // Calculate curK.
                for (int curFunc = 0; curFunc < funcCount; ++curFunc) {
                    k[curK][neuron][curFunc] = cond->step * slopes[curFunc];
                }
            }            
        }

        // Calculate approximation for each function of each neuron.
        for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
            for (int curFunc = 0; curFunc < funcCount; ++curFunc) {
                sol.approx[neuron][curFunc][curStep + 1] = sol.approx[neuron][curFunc][curStep] + (k[0][neuron][curFunc] + k[1][neuron][curFunc] + k[1][neuron][curFunc] + k[2][neuron][curFunc] + k[2][neuron][curFunc] + k[3][neuron][curFunc]) / 6.0;
            }
        }
        
        // Calculate next step in the x direction.
        sol.x[curStep + 1] = sol.x[curStep] + cond->step;        
    }

    return sol;
}

void writeSolution(char *filename, float x[], float approx[], int size, float transient) {
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

void freeEqConditions(EqConditions *cond) {
    // Free the initial values array.
    free(cond->inits);
}

void freeEqSolution(EqSolution *sol) {
    for (int neuron = 0; neuron < sol->neuronCount; ++neuron) {
        for (int func = 0; func < sol->funcCount; ++func) {
            // Free the steps arrays.
            free(sol->approx[neuron][func]);
        }

        // Free the function arrays.
        free(sol->approx[neuron]);
    }  
    
    // Free the neuron and x arrays.
    free(sol->approx);
    free(sol->x);
}