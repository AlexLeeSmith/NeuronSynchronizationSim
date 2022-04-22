/**
 * @file simulation_driver.c
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief Driver for running the neuron synchronization simulation.
 * @version 0.1
 * @date 2022-02-24
 * 
 * @copyright Copyright (c) 2022
 */

#include "simulation_driver.h"
#include "spike_calculations.h"
#include "differential_equations.h"
#include "numerical_methods.h"
#include "graph_manipulations.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#define FUNC_COUNT 3            // MAKE SURE TO CHANGE THIS BEFORE SWAPPING THE getODEs() FUNCTION.
#define SPIKE_THRESHOLD 0.0

int main(int argc, char *argv[]) {
    double start, elapsed;
    myArgs args;
    EqSolution sol;
    Points *spikes;
    ISI *isis;

    // Read command line parameters.
    args = getArgs(argc, argv);

    // Allocate dynamic memory for spikes.
    if ((spikes = (Points *) malloc(args.graph.vertexCount * sizeof(Points))) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }

    // Allocate dynamic memory for ISIs.
    if ((isis = (ISI *) malloc(args.graph.vertexCount * sizeof(ISI))) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }

    // Run calculations.
    start = getTime();
    sol = runRungeKutta(&getHR, &args.cond, &args.graph, FUNC_COUNT);
    for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
        spikes[neuron] = findSpikes(sol.x, sol.approx[neuron][0], sol.stepCount + 1, args.cond.transient, SPIKE_THRESHOLD);
        isis[neuron] = calcISI(&spikes[neuron]);
    }
    elapsed = getTime() - start;

    // Print results.
    printf("Hindmarsh-Rose (HR) neuronal model:\n");
    printf("\tElapsed: %f seconds\n", elapsed);
    //printf("\tAvg Frequency: %f spikes/sec\n\n", getAveFrequency(spikes.size, args.cond.transient, args.cond.xEnd, 1000.0));

    // Write calculations.
    char filename[20];
    for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
        // Write the neuron approximation.
        sprintf(filename, "Out/approx%d", neuron);
        writeSolution(filename, sol.x, sol.approx[neuron][0], sol.stepCount + 1, args.cond.transient);

        // Write the neuron spikes.
        sprintf(filename, "Out/spikes%d", neuron);
        writePoints(filename, &spikes[neuron]);

        // Write the neuron inter-spike interval.
        sprintf(filename, "Out/ISI%d", neuron);
        writeISI(filename, &isis[neuron]);
    }
    
    // Free heap memory and exit.
    freeArgs(&args);
    freeEqSolution(&sol);
    for (int neuron = 0; neuron < sol.neuronCount; ++neuron) {
        freePoints(&spikes[neuron]);
        freeISI(&isis[neuron]);
    }
    free(spikes);
    free(isis);
    exit(EXIT_SUCCESS);
}

myArgs getArgs(int argc, char *argv[]) {
    myArgs args;

    // Verify the number of arguments.
    if (argc != 6) 
        usage(argv[0]);

    // Get conditions.
    args.cond = initEqConditions(strtod(argv[1], NULL), strtod(argv[2], NULL), strtod(argv[3], NULL), strtod(argv[4], NULL), FUNC_COUNT);
    
    // Get graph.
    args.graph = readGraph(argv[5]);
    
    return args;
} 

void freeArgs(myArgs *args) {
    freeEqConditions(&args->cond);
    freeGraph(&args->graph);
}

void usage(const char *prog_name) {
    fprintf(stderr, "\nUsage: %s [x0] [xEnd] [step] [transient] [graph file path]\n\n", prog_name);
    exit(EXIT_FAILURE);
}

double getTime(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec/1000000.0;
}