/**
 * @file differential_equations.c
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief This file implements the differential equations header file.
 * @version 0.1
 * @date 2022-04-21
 * 
 * @copyright Copyright (c) 2022
 */

#define xR -1.56F
#define r 0.006F
#define I 3.1F

/**
 * @brief Calculates the synchronization factor of this neuron to all other neurons.
 * 
 * @param inputs the inputs for each neuron. Access using inputs[neuronNum].
 * @param weights the weights/edges between myNeuron and all other neurons. Access using weights[neuronNum].
 * @param neuronCount the number of neurons in the graph.
 * @param myNeuron the number of the current neuron.
 * @return float - the calculated synchronization factor.
 */
float calcSyncFactor(float inputs[], float weights[], int neuronCount, int myNeuron) {
    float factor = 0.0;

    // Calculate the synchronization factor of each neuron.
    for (int neuron = 0; neuron < neuronCount; ++neuron) {
        if (neuron != myNeuron) {
            // This factor = connectivity * difference
            factor += weights[neuron] * (inputs[myNeuron] - inputs[neuron]);
        }
    }
    
    return factor;
}

void getHR(int neuronCount, float inputs[][neuronCount], float curX, float weights[], int myNeuron, float result[]) {
    float x = inputs[0][myNeuron];    // Voltage
    float y = inputs[1][myNeuron];    // Spiking
    float z = inputs[2][myNeuron];    // Bursting
    float s = weights[myNeuron];

    result[0] = y - (x*x*x) + (3*x*x) - z + I - calcSyncFactor(inputs[0], weights, neuronCount, myNeuron);
    result[1] = 1 - (5*x*x) - y;
    result[2] = r * (s * (x - xR) - z);
}