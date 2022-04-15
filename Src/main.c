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
#include "graph_manipulations.h"

#include <stdio.h>      // printf(), fprintf()
#include <math.h>       // ceil(), exp()
#include <stdlib.h>     // exit(), EXIT_SUCCESS, EXIT_FAILURE, malloc(), free(), strtod()
#include <sys/time.h>   // timeval, gettimeofday()
#include <string.h>     // strstr()

#define FUNC_COUNT 3    // MAKE SURE TO CHANGE THIS BEFORE SWAPPING THE getODEs() FUNCTION.
#define SPIKE_THRESHOLD 0.0
#define xR -1.56F
#define r 0.006F
#define I 3.1F

/** Structures **/
typedef struct {
    float x0;
    float xEnd;
    float step;
    float transient;
    char *graphFile;
} myArgs;

/** Forward Declarations **/
myArgs getArgs(int, char *[]);
void usage(const char *);
double getTime();
float *getExp(float [], float);
void getHR(int neuronCount, float inputs[][neuronCount], float curX, float weights[], int myNeuron, float result[]);

/** Functions **/
int main(int argc, char *argv[]) {
    double start, elapsed;
    myArgs args;
    EqConditions cond;
    EqSolution sol;
    Graph graph;
    Points *spikes;

    // Read command line parameters.
    args = getArgs(argc, argv);

    // Initialize Conditions and Solution structures.
    cond = initEqConditions(args.x0, args.xEnd, args.step, args.transient, FUNC_COUNT);
    graph = readGraph(args.graphFile);
    if ((spikes = (Points *) malloc(graph.vertexCount * sizeof(Points))) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }

    // Allocate intervals array.
    //float *intervals = (float *) malloc(numBytes);

    // Run calculations.
    start = getTime();
    sol = runRungeKutta(&getHR, &cond, &graph, FUNC_COUNT);
    for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
        spikes[neuron] = findSpikes(sol.x, sol.approx[neuron][0], sol.stepCount + 1, cond.transient, SPIKE_THRESHOLD);
    }
    //int isiCount = getInterSpikeIntervals(&spikes, intervals);
    elapsed = getTime() - start;

    // Print results.
    printf("Hindmarsh-Rose (HR) neuronal model:\n");
    printf("\tElapsed: %f seconds\n", elapsed);
    //printf("\tAvg Frequency: %f spikes/sec\n\n", getAveFrequency(spikes.size, args.cond.transient, args.cond.xEnd, 1000.0));

    // for (int i = 0; i < sol.stepCount + 1; i++) {
    //     printf("%f\n", sol.approx[0][0][i]);
    // }

    // Write calculations.
    char filename[20];
    for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
        sprintf(filename, "Out/approx%d", neuron);
        writeSolution(filename, sol.x, sol.approx[neuron][0], sol.stepCount + 1, cond.transient);
        sprintf(filename, "Out/spikes%d", neuron);
        writePoints(filename, &spikes[neuron]);
        //writeInterSpikeIntervals("Out/ISIs", intervals, isiCount);
    }
    
    // Free heap memory and exit.
    freeEqConditions(&cond);
    freeEqSolution(&sol);
    for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
        freePoints(&spikes[neuron]);
    }
    free(spikes);
    freeGraph(&graph);
    //free(intervals);
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
myArgs getArgs(int argc, char *argv[]) {
    myArgs args;

    // Verify the number of arguments.
    if (argc != 6) 
        usage(argv[0]);

    // Get conditions.
    args.x0 = strtod(argv[1], NULL);
    args.xEnd = strtod(argv[2], NULL);
    args.step = strtod(argv[3], NULL);
    args.transient = strtod(argv[4], NULL);
    args.graphFile = argv[5];
    
    return args;
} 

/**
 * @brief Prints a message to stderr explaining how to run the program.
 * 
 * @param prog_name the name of the executable file.
 */
void usage(const char *prog_name) {
    fprintf(stderr, "\nUsage: %s [x0] [xEnd] [step] [transient] [graph file]\n\n", prog_name);
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
 * @return a static array of results from evaluating the ODEs.
 */
float *getExp(float inputs[], float curX) {
    static float slopes[1];

    slopes[0] = exp(curX);

    return slopes;
}

float calcSyncFactor(float inputs[], float weights[], int neuronCount, int myNeuron) {
    float factor = 0.0;

    // Caclulate the synchronization factor of each neuron.
    for (int neuron = 0; neuron < neuronCount; ++neuron) {
        if (neuron != myNeuron) {
            factor += weights[neuron] * (inputs[myNeuron] - inputs[neuron]);
        }
    }
    
    return factor;
}

/**
 * @brief Hindmarsh-Rose (HR) neuronal model.
 * 
 * @param inputs an array of inputs used to evaluate the ODEs.
 * @param curX the current x coordinate.
 * 
 * @return a static array of results from evaluating the ODEs.
 */
void getHR(int neuronCount, float inputs[][neuronCount], float curX, float weights[], int myNeuron, float result[]) {
    float x = inputs[0][myNeuron];    // Voltage
    float y = inputs[1][myNeuron];    // Spiking
    float z = inputs[2][myNeuron];    // Bursting
    float s = weights[myNeuron];

    // result[0] = y - (x*x*x) + (3*x*x) - z + I - gc * (x - x2);
    result[0] = y - (x*x*x) + (3*x*x) - z + I - calcSyncFactor(inputs[0], weights, neuronCount, myNeuron);
    result[1] = 1 - (5*x*x) - y;
    result[2] = r * (s * (x - xR) - z);
}