/**
 * Main file for running the numerical_methods and spike_cacluations files.
 * 
 * Usage: ./main [Method]
 * Methods: Eulers, RungeKutta
 * 
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 2/24/22
 */

/** Preprocessor Directives **/
#include "numerical_methods.h"
#include "spike_calculations.h"

#include <stdio.h>      // printf()
#include <math.h>       // ceil(), exp()
#include <stdlib.h>     // exit(), EXIT_SUCCESS, EXIT_FAILURE, malloc()
#include <sys/time.h>   // timeval, gettimeofday()
#include <string.h>     // strcmp()

#define xR -1.56F
#define r 0.006F
#define I 3.1F

/** Global Variables **/
float s = 3.6;

/** Structures **/
typedef struct {
    void (*numericalMethod)(float *(*)(float [], float), EqConditions *, EqSolution *);
} myArgs;

/** Forward Declarations **/
myArgs getArgs(int, char const *[]);
void usage(const char *);
double getTime();
float *getExp(float [], float);
float *getHR(float [], float);

/** Functions **/
int main(int argc, char const *argv[]) {
    double start, elapsed;
    myArgs args;

    // Read command line parameters.
    args = getArgs(argc, argv);

    // Initialize condition and solution structures.
    EqConditions cond = {
        .x0 = 0.0, 
        .xEnd = 2000.0, 
        .inits = (float[]){0.0, 0.0, 0.0},
        .step = 0.1,
        .transient = 900.0
    };
    int stepCount = ceil((cond.xEnd - cond.x0) / cond.step);
    int size = stepCount + 1;
    int numBytes = size * sizeof(float);
    EqSolution sol = {
        .approx = (float *[]){malloc(numBytes), malloc(numBytes), malloc(numBytes)},
        .x = malloc(numBytes),
        .funcCount = 3,
        .stepCount = stepCount
    };
    Points spikes = { 
        .x = malloc(numBytes),
        .y = malloc(numBytes)
    };
    float *intervals = (float *) malloc(numBytes);
    
    // Run the numerical method.
    start = getTime();
    args.numericalMethod(&getHR, &cond, &sol);
    elapsed = getTime() - start;

    // Print timing and approximations.
    fprintf(stderr, "Elapsed: %f seconds\n", elapsed);
    //printSolution(sol.x, sol.approx[0], size, cond.transient);

    findSpikes(&spikes, sol.x, sol.approx[0], size, cond.transient);
    printf("avefq: %f\n", getAveFrequency(spikes.size, cond.transient, cond.xEnd, 1000.0));
    // for (int i = 0; i < spikes.size; i++) {
    //     printf("%f\t%f\n", spikes.x[i], spikes.y[i]);
    // }

    getInterSpikeIntervals(&spikes, intervals);
    // intervals = realloc(intervals, spikes.size - 1);
    // for (int i = 0; i < spikes.size - 1; i++) {
    //     printf("%f\n", intervals[i]);
    // }    

    // Free heap memory and exit.
    freeEqSolution(&sol);
    freePoints(&spikes);
    free(intervals);
    exit(EXIT_SUCCESS);
}

/**
 * @brief Get the command line arguments.
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
 * @brief Prints a message to stderr explaining how to run the program.
 * 
 * @param prog_name the name of the executable file
 */
void usage(const char *prog_name) {
    fprintf(stderr, "\nUsage: %s [Method]\n", prog_name);
    fprintf(stderr, "\tMethods: Eulers, RungeKutta\n\n");
    exit(EXIT_FAILURE);
}

/**
 * @brief Gets the current time in seconds.
 */
double getTime(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec/1000000.0;
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