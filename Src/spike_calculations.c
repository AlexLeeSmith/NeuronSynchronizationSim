/**
 * This file implements the spike calculations header file.
 * 
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 2/24/22
 */

/** Preprocessor Directives **/
#include "spike_calculations.h"

#include <stdlib.h>     // free()

/** Functions **/
/**
 * @brief Finds the spikes within the given set of points.
 * 
 * @param spikes the Points struture to store the found spikes in.
 * @param x an array of x coordinates.
 * @param y an array of y coordinates.
 * @param size the size of the x and y arrays (must be at least 3).
 */
void findSpikes(Points *spikes, float x[], float y[], int size, float transient) {
    int spikeCount = 0;

    // Find the point to start searching from.
    int start = 0;
    for (int i = 0; i < size && !start; ++i) {
        if (x[i] >= transient) {
            start = i;
        }        
    }

    // Look for the spike tips (the first point cannot be a spike).
    for (int i = 0, j = start; j < size - 2; ++i, ++j) {   
        if (y[j] < y[j+1] && y[j+1] > y[j+2]) {
            spikes->x[spikeCount] = x[i+1];
            spikes->y[spikeCount] = y[j+1];
            ++spikeCount;
        }
    }

    // Look for the first point above a specifed threshold.
    // int found = 0;
    // for (int i = 0; i < size ; ++i) {
    //     if (y[i] >= threshold) {
    //         if (!found) {
    //             spikes->x[spikeCount] = x[i];
    //             spikes->y[spikeCount] = y[i];
    //             ++spikeCount;
    //             found = 1;
    //         }
    //     }
    //     else {
    //         found = 0;
    //     }
    // }

    // Update the spike count.
    spikes->size = spikeCount;
}

/**
 * @brief Calculates the average frequency.
 * 
 * @param spikeCount the number of spikes.
 * @param size the number of points.
 * @param scale the factor to scale the result by.
 */
double getAveFrequency(int spikeCount, int transient, int xEnd, float scale) {
    return ((double) spikeCount / (xEnd - transient)) * scale;
}

void getInterSpikeIntervals(Points *spikes, float intervals[]) {
    for (int i = 0; i < spikes->size - 1; ++i) {
        intervals[i] = spikes->x[i+1] - spikes->x[i];
    }
}

void freePoints(Points *spikes) {
    free(spikes->x);
    free(spikes->y);
}
