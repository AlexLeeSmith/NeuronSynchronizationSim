/**
 * This file implements the numerical methods header file.
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 1/30/22
 */

/** Preprocessor Directives **/
#include "numerical_methods.h"

#include <stdio.h>      // fprintf(), FILE, fopen(), fclose(), perror()
#include <stdlib.h>     // EXIT_FAILURE
#include <math.h>       // ceil()

/** Functions **/
EqConditions initEqConditions(float x0, float xEnd, float step, float transient, int funcCount) {
    EqConditions cond = {
        .x0 = x0,
        .xEnd = xEnd,
        .step = step,
        .transient = transient
    };

    if ((cond.inits = (float *) malloc(funcCount * sizeof(float))) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < funcCount; ++i) {
        cond.inits[i] = 0.0;
    }

    return cond;
}

EqSolution initEqSolution(float x0, float xEnd, float step, int neuronCount, int funcCount) {
    int stepCount = ceil((xEnd - x0) / step);
    int size = stepCount + 1;
    int numBytes = size * sizeof(float);

    EqSolution sol = {
        .neuronCount = neuronCount,
        .funcCount = funcCount,
        .stepCount = stepCount
    };
    if ((sol.x = (float *) malloc(numBytes)) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }

    if ((sol.approx = (float ***) malloc(neuronCount * sizeof(float **))) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }
    for (int neuron = 0; neuron < neuronCount; ++neuron) {
        if ((sol.approx[neuron] = (float **) malloc(funcCount * sizeof(float *))) == NULL) {
            perror("malloc() failure");
            exit(EXIT_FAILURE);
        }        
        for (int func = 0; func < funcCount; ++func) {
            if ((sol.approx[neuron][func] = (float *) malloc(numBytes)) == NULL) {
                perror("malloc() failure");
                exit(EXIT_FAILURE);
            }            
        }
    }    

    return sol;
}

// /**
//  * @brief Runs Euler's first-order numerical method for approximating ODEs.
//  * 
//  * @param getODEs a pointer to function that returns the result(s) of ODEs with given inputs.
//  * @param cond a structure containing the input conditions.
//  * @param sol a struture where the solution will be stored.
//  */
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

    // for (int i = 0; i < sol.stepCount + 1; i++) {
    //     printf("%f\n", sol.approx[0][0][i]);
    // }

    return sol;
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

void freeEqConditions(EqConditions *cond) {
    free(cond->inits);
}

/**
 * @brief Frees the heap memory allocated to a EqSolution struture.
 * 
 * @param sol the EqSolution struture to be freed.
 */
void freeEqSolution(EqSolution *sol) {
    for (int neuron = 0; neuron < sol->neuronCount; ++neuron) {
        for (int func = 0; func < sol->funcCount; ++func) {
            free(sol->approx[neuron][func]);
        }
        free(sol->approx[neuron]);
    }  
    free(sol->approx);
    free(sol->x);
}