/**
 * @file differential_equations.h
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief Header file for a program that implements differential equations.
 * @version 0.1
 * @date 2022-04-21
 * 
 * @copyright Copyright (c) 2022 
 */

#ifndef DIFFERENTIAL_EQUATIONS
#define DIFFERENTIAL_EQUATIONS

/**
 * @brief The Hindmarsh-Rose (HR) neuronal model.
 * 
 * @param neuronCount the number of neurons in the graph.
 * @param inputs the inputs for each function for every neuron. Access using inputs[functionNum][neuronNum].
 * @param curX the current x position.
 * @param weights the weights/edges between myNeuron and all other neurons. Access using weights[neuronNum].
 * @param myNeuron the number of the current neuron.
 * @param result the calculated values for each function with the given inputs. Access using result[functionNum].
 */
void getHR(int neuronCount, float inputs[][neuronCount], float curX, float weights[], int myNeuron, float result[]);

#endif