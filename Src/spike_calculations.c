/**
 * This file implements the spike calculations header file.
 * 
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 2/24/22
 */

/** Preprocessor Directives **/
#include "spike_calculations.h"

#include <stdio.h>      // fprintf(), FILE, fopen(), fclose(), perror()
#include <stdlib.h>     // EXIT_FAILURE

/** Functions **/
Points initPoints(int size) {
    int numBytes = size * sizeof(float);

    Points points = { 
        .x = (float *) malloc(numBytes),
        .y = (float *) malloc(numBytes),
        .size = size
    };

    return points;
}

/**
 * @brief Finds the spikes within the given set of points.
 * 
 * @param x an array of x coordinates.
 * @param y an array of y coordinates.
 * @param size the size of the x and y arrays (must be at least 3).
 */
Points findSpikes(float x[], float y[], int size, float transient) {
    Points spikes = initPoints(size);
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
            spikes.x[spikeCount] = x[i+1];
            spikes.y[spikeCount] = y[j+1];
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
    spikes.size = spikeCount;

    return spikes;
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

/**
 * @brief Finds the inter-spike intervals within the given set of spikes.
 * 
 * @param spikes the Points struture where the spikes are stored.
 * @param intervals the buffer array to store the intervals in.
 * 
 * @return the amount of intervals found.
 */
int getInterSpikeIntervals(Points *spikes, float intervals[]) {
    // Find the difference between each spike's x value.
    for (int i = 0; i < spikes->size - 1; ++i) {
        intervals[i] = spikes->x[i+1] - spikes->x[i];
    }

    return spikes->size - 1;
}

/**
 * @brief Writes a set of points to a file.
 * 
 * @param filename the name of the file to write to.
 * @param points the Points struture to be written.
 */
void writePoints(char filename[], Points *points) {
    // Open output file for writing.
    FILE *outfile;
    if ((outfile = fopen(filename, "w")) == NULL) {
        perror("Write Points");
        exit(EXIT_FAILURE);
    }
    
    // Begin writing.
    for (int i = 0; i < points->size; i++) {
        fprintf(outfile, "%f\t%f\n", points->x[i], points->y[i]);
    }

    // Close ouput file.
    fclose(outfile);
}

/**
 * @brief Writes an array of intervals to a file.
 * 
 * @param filename the name of the file to write to.
 * @param intervals the array of intervals to be written.
 * @param size the size of the intervals array.
 */
void writeInterSpikeIntervals(char filename[], float intervals[], int size) {
    // Open output file for writing.
    FILE *outfile;
    if ((outfile = fopen(filename, "w")) == NULL) {
        perror("Write InterSpike Intervals");
        exit(EXIT_FAILURE);
    }

    // Begin writing.
    for (int i = 0; i < size; i++) {
        fprintf(outfile, "%f\n", intervals[i]);
    }  

    // Close ouput file.
    fclose(outfile);
}

/**
 * @brief Frees the heap memory allocated to a Points struture.
 * 
 * @param points the Points struture to be freed.
 */
void freePoints(Points *points) {
    free(points->x);
    free(points->y);
}
