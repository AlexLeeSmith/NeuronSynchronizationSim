/**
 * This program implements multiple numerical methods for solving ordinary differential equations (ODE).
 * 
 * Usage: numerical_methods [Method]
 * Methods: Eulers, RungeKutta
 * 
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 1/30/22
 */

/** Preprocessor Directives **/
#include <stdio.h>      // printf()
#include <math.h>       // ceil(), exp()
#include <stdlib.h>     // exit(), EXIT_SUCCESS, EXIT_FAILURE, malloc(), free()
#include <string.h>     // strcmp()

#define xR -1.56F
#define r 0.006F
#define I 3.1F

/** Global Variables **/
float s = 4.2;

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

typedef struct {
    void (*numericalMethod)(float *(*)(float [], float), EqConditions *, EqSolution *);
} myArgs;

/** Forward Declarations **/
myArgs getArgs(int, char const *[]);
void usage(const char *);
void runEulers(float *(*)(float [], float), EqConditions *, EqSolution *);
void runRungeKutta(float *(*)(float [], float), EqConditions *, EqSolution *);
void printSolution(float [], float [], int);
void freeEqSolution(EqSolution *);
float *getExp(float [], float);
float *getHR(float [], float);

/** Functions **/
int main(int argc, char const *argv[]) {
    myArgs args;

    // Read command line parameters.
    args = getArgs(argc, argv);

    // Initialize condition and solution structures.
    EqConditions cond = {
        .x0 = 0.0, 
        .xEnd = 500.0, 
        .inits = (float[]){0.0, 0.0, 0.0},
        .step = 0.1
    };
    int stepCount = ceil((cond.xEnd - cond.x0) / cond.step);
    int numBytes = stepCount * sizeof(float);
    EqSolution sol = {
        .approx = (float *[]){malloc(numBytes), malloc(numBytes), malloc(numBytes)},
        .x = malloc(numBytes),
        .funcCount = 3,
        .stepCount = stepCount
    };
    
    // Run the numerical method.
    args.numericalMethod(&getHR, &cond, &sol);

    // Print results.
    printSolution(sol.approx[0], sol.x, sol.stepCount);

    // Free heap memory and exit.
    freeEqSolution(&sol);
    exit(EXIT_SUCCESS);
}

/**
 * Get the command line arguments.
 * 
 * @param argc the number of arguments
 * @param argv the array of arguments
 * @return the command line arguments in their correct data types
 */
myArgs getArgs(int argc, char const *argv[]) {
    myArgs args;

    // Verify the number of arguments.
    if (argc != 2) 
        usage(argv[0]);
    
    // Verify the numerical method.
    if (strcmp(argv[1], "eulers") == 0 || strcmp(argv[1], "Eulers") == 0) {
        args.numericalMethod = &runEulers;
    }
    else if (strcmp(argv[1], "rungekutta") == 0 || strcmp(argv[1], "RungeKutta") == 0) {
        args.numericalMethod = &runRungeKutta;
    }
    else {
        usage(argv[0]);
    }     
    
    return args;
} 

/**
 * Prints a message to stderr explaining how to run the program.
 * 
 * @param prog_name the name of the executable file
 */
void usage(const char *prog_name) {
    fprintf(stderr, "\nUsage: %s [Method]\n", prog_name);
    fprintf(stderr, "\tMethods: Eulers, RungeKutta\n\n");
    exit(EXIT_FAILURE);
}

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
 * @param approx an array of approximations
 * @param x an array of steps
 * @param stepCount the number of steps (i.e., size of the two prior arrays - 1)
 */
void printSolution(float approx[], float x[], int stepCount) {
    for (int i = 0; i <= stepCount; i++) {
        printf("%f\t%f\n", x[i], approx[i]);
    }
    printf("\n");
}

/**
 * @brief releases the heap memory allocated to a solution struture.
 * 
 * @param sol the solution struture to be freed.
 */
void freeEqSolution(EqSolution *sol) {
    for (int i = 0; i < sol->funcCount; ++i) {
        free(sol->approx[i]);
    }
    free(sol->x);
}

/**
 * @brief ODE containing an exponential.
 * 
 * @param inputs an array of inputs used to evaluate the ODEs.
 * @param curX the current x coordinate.
 * @return float* a static array of results from evaluating the ODEs.
 */
float *getExp(float inputs[], float curX) {
    static float slopes[1];

    slopes[0] = exp(curX);

    return slopes;
}

/**
 * @brief Hindmarsh-Rose (HR) neuronal model.
 * 
 * @param inputs an array of inputs used to evaluate the ODEs.
 * @param curX the current x coordinate.
 * @return float* a static array of results from evaluating the ODEs.
 */
float *getHR(float inputs[], float curX) {
    static float slopes[3];
    float x = inputs[0];    // Voltage
    float y = inputs[1];    // Spiking
    float z = inputs[2];    // Bursting

    slopes[0] = y - (x*x*x) + (3*x*x) - z + I;
    slopes[1] = 1 - (5*x*x) - y;
    slopes[2] = r * (s * (x - xR) - z);

    return slopes;
}