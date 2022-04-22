/**
 * @file simulation_driver.h
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief Header file for a program that runs a neuron synchronization simulation.
 * @version 0.1
 * @date 2022-02-24
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef SIMULATION_DRIVER
#define SIMULATION_DRIVER

#include "numerical_methods.h"
#include "graph_manipulations.h"

/**
 * @brief A structure to capture all necessary command-line arguments. 
 */
typedef struct {
    /**
     * @brief The conditions to be used to run the simulation.
     */
    EqConditions cond;

    /**
     * @brief The graph to be used to run the simulation.
     */
    Graph graph;
} myArgs;

/**
 * @brief Runs the neuron synchronization simulation.
 * 
 * @param argc the command-line argument count.
 * @param argv the command-line argument values.
 * @return int - the exit status of the simulation.
 */
int main(int argc, char *argv[]);

/**
 * @brief Get the command-line arguments.
 * 
 * @param argc the number of arguments.
 * @param argv the array of arguments.
 * @return myArgs - the command line arguments in their correct data types.
 */
myArgs getArgs(int, char *[]);

/**
 * @brief Frees the dynamic/heap memory allocated to an Args structure.
 * 
 * @param args the Args to be freed.
 */
void freeArgs(myArgs *args);

/**
 * @brief Prints a message to stderr explaining how to run the program.
 * 
 * @param prog_name the name of the executable file.
 */
void usage(const char *);

/**
 * @brief Gets the current time in seconds.
 * 
 * @return double - the current time in seconds.
 */
double getTime();

#endif