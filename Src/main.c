/**
 * Main file for running the numerical_methods and spike_cacluations files.
 * 
 * Usage: ./Bin/main [x0] [xEnd] [step] [transient] [Opt: Moderators]
 *      Moderators: s=
 * 
 * Example: ./Bin/main 0 2000 .1 500 s=4.8
 * 
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 2/24/22
 */

/** Preprocessor Directives **/
#include "numerical_methods.h"
#include "spike_calculations.h"

#include <stdio.h>      // printf(), fprintf()
#include <math.h>       // ceil(), exp()
#include <stdlib.h>     // exit(), EXIT_SUCCESS, EXIT_FAILURE, malloc(), free(), strtod()
#include <sys/time.h>   // timeval, gettimeofday()
#include <string.h>     // strcmp(), strstr()

#define FUNC_COUNT 3    // MAKE SURE TO CHANGE THIS BEFORE SWAPPING THE getODEs() FUNCTION.
#define xR -1.56F
#define r 0.006F
#define I 3.1F

/** Global Variables **/
float s = 3.6;          // Default value.

/** Structures **/
typedef struct {
    EqConditions cond;
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

    // Initialize EqSolution structure.
    int stepCount = ceil((args.cond.xEnd - args.cond.x0) / args.cond.step);
    int size = stepCount + 1;
    int numBytes = size * sizeof(float);
    EqSolution sol = {
        .x = (float *) malloc(numBytes),
        .funcCount = FUNC_COUNT,
        .stepCount = stepCount
    };
    float *temp[FUNC_COUNT];
    for (int i = 0; i < FUNC_COUNT; i++) {
        temp[i] = (float *) malloc(numBytes);
    }
    sol.approx = temp;
    
    // Initialize Points structure.
    Points spikes = { 
        .x = (float *) malloc(numBytes),
        .y = (float *) malloc(numBytes)
    };

    // Allocate intervals array.
    float *intervals = (float *) malloc(numBytes);
    
    // Run calculations.
    start = getTime();
    runRungeKutta(&getHR, &args.cond, &sol);
    findSpikes(&spikes, sol.x, sol.approx[0], size, args.cond.transient);
    int isiCount = getInterSpikeIntervals(&spikes, intervals);
    elapsed = getTime() - start;

    // Print results.
    printf("Hindmarsh-Rose (HR) neuronal model (s=%g):\n", s);
    printf("\tElapsed: %f seconds\n", elapsed);
    printf("\tAvg Frequency: %f spikes/sec\n\n", getAveFrequency(spikes.size, args.cond.transient, args.cond.xEnd, 1000.0));

    // Write calculations.
    writeSolution("Out/approx", sol.x, sol.approx[0], size, args.cond.transient);
    writePoints("Out/spikes", &spikes);
    writeInterSpikeIntervals("Out/ISIs", intervals, isiCount);

    // Free heap memory and exit.
    freeEqSolution(&sol);
    freePoints(&spikes);
    free(intervals);
    exit(EXIT_SUCCESS);
}

/**
 * @brief Get the command line arguments.
 * 
 * @param argc the number of arguments.
 * @param argv the array of arguments.
 * 
 * @return the command line arguments in their correct data types.
 */
myArgs getArgs(int argc, char const *argv[]) {
    myArgs args;

    // Verify the number of arguments.
    if (argc != 5 && argc != 6) 
        usage(argv[0]);

    // Get conditions.
    args.cond.x0 = strtod(argv[1], NULL);
    args.cond.xEnd = strtod(argv[2], NULL);
    args.cond.step = strtod(argv[3], NULL);
    args.cond.transient = strtod(argv[4], NULL);

    float temp[FUNC_COUNT];
    for (int i = 0; i < FUNC_COUNT; i++) {
        temp[i] = 0.0;
    }
    args.cond.inits = temp;

    // Get moderators.
    if (argc == 6) {
        char *p = strstr(argv[5], "s=");
        if (p) {
            s = strtod(&p[2], NULL);
        }
        else {
            fprintf(stderr, "\nModerator s not found.\n\n");
            exit(EXIT_FAILURE);
        }
    }    
    
    return args;
} 

/**
 * @brief Prints a message to stderr explaining how to run the program.
 * 
 * @param prog_name the name of the executable file.
 */
void usage(const char *prog_name) {
    fprintf(stderr, "\nUsage: %s [x0] [xEnd] [step] [transient] [Opt: Moderators]\n", prog_name);
    fprintf(stderr, "\tModerators: s=\n\n");
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
 * @brief ODE containing an exponential.
 * 
 * @param inputs an array of inputs used to evaluate the ODEs.
 * @param curX the current x coordinate.
 * 
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
 * 
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