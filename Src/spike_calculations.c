/**
 * @file spike_calculations.c
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief This file implements the spike calculations header file.
 * @version 0.1
 * @date 2022-02-22
 * 
 * @copyright Copyright (c) 2022
 */

#include "spike_calculations.h"

#include <stdio.h>
#include <stdlib.h>

Points initPoints(int size) {
    Points points = {
        .size = size
    };

    // Calculate the number of bytes to be allocated for each array.
    int numBytes = size * sizeof(float);
    
    // Allocate heap memory for the x array.
    if ((points.x = (float *) malloc(numBytes)) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }

    // Allocate heap memory for the y array.
    if ((points.y = (float *) malloc(numBytes)) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }

    return points;
}

ISI initISI(int size) {
    ISI isi = {
        .size = size
    };

    // Calculate the number of bytes to be allocated for the array.
    int numBytes = size * sizeof(float);

    // Allocate heap memory for the intervals array.
    if ((isi.intervals = (float *) malloc(numBytes)) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }

    return isi;
}

Points findSpikes(float x[], float y[], int size, float transient, float threshold) {
    Points spikes = initPoints(size);
    int spikeCount = 0;

    // Find the point to start searching from.
    int start = 0;
    for (int i = 0; i < size && !start; ++i) {
        if (x[i] >= transient) {
            start = i;
        }        
    }

    // // Look for the spike tips (the first point cannot be a spike).
    // for (int i = 0, j = start; j < size - 2; ++i, ++j) {   
    //     if (y[j] <= y[j+1] && y[j+1] >= y[j+2]) {
    //         spikes.x[spikeCount] = x[i+1];
    //         spikes.y[spikeCount] = y[j+1];
    //         ++spikeCount;
    //     }
    // }

    // // Look for the first point above a specifed threshold.
    // int found = 0;
    // for (int i = 0, j = start; j < size; ++i, ++j) {
    //     if (y[j] >= threshold) {
    //         if (!found) {
    //             spikes.x[spikeCount] = x[i];
    //             spikes.y[spikeCount] = y[j];
    //             ++spikeCount;
    //             found = 1;
    //         }
    //     }
    //     else {
    //         found = 0;
    //     }
    // }

    // Look for peaks that appear above the specifed threshold
    int found = 0;
    for (int i = 0, j = start; j < size - 2; ++i, ++j) {
        // Check if the middle point is above the threshold.
        if (y[j+1] >= threshold) {
            // Check if this point is a peak.
            if (!found && y[j] <= y[j+1] && y[j+1] >= y[j+2]) {
                spikes.x[spikeCount] = x[i+1];
                spikes.y[spikeCount] = y[j+1];
                ++spikeCount;
                found = 1;
            }
        }
        else {
            found = 0;
        }
    }

    // Update the spike count.
    spikes.size = spikeCount;

    return spikes;
}

float calcAvgFrequency(int spikeCount, int transient, int xEnd, float scale) {
    return ((float) spikeCount / (xEnd - transient)) * scale;
}

ISI calcISI(Points *spikes) {
    ISI isi = initISI(spikes->size - 1);

    // Find the difference between each spike's x value.
    for (int i = 0; i < spikes->size - 1; ++i) {
        isi.intervals[i] = spikes->x[i+1] - spikes->x[i];
    }

    return isi;
}

void writePoints(char filename[], Points *points) {
    // Open output file for writing.
    FILE *outfile;
    if ((outfile = fopen(filename, "w")) == NULL) {
        perror("Write Points");
        exit(EXIT_FAILURE);
    }
    
    // Begin writing (x - y).
    for (int i = 0; i < points->size; ++i) {
        fprintf(outfile, "%f\t%f\n", points->x[i], points->y[i]);
    }

    // Close ouput file.
    fclose(outfile);
}

void writeISI(char filename[], ISI *isi) {
    // Open output file for writing.
    FILE *outfile;
    if ((outfile = fopen(filename, "w")) == NULL) {
        perror("Write Inter-Spike Intervals");
        exit(EXIT_FAILURE);
    }

    // Begin writing (inter-spike intervals).
    for (int i = 0; i < isi->size; ++i) {
        fprintf(outfile, "%f\n", isi->intervals[i]);
    }  

    // Close ouput file.
    fclose(outfile);
}

void writeAvgFrequencies(char filename[], float avgFreqs[], int size) {
    // Open output file for writing.
    FILE *outfile;
    if ((outfile = fopen(filename, "w")) == NULL) {
        perror("Write Average Frequencies");
        exit(EXIT_FAILURE);
    }

    // Begin writing (neuron ID - avg freq).
    for (int i = 0; i < size; ++i) {
        fprintf(outfile, "%d\t%f\n", i, avgFreqs[i]);
    }  

    // Close ouput file.
    fclose(outfile);
}

void freePoints(Points *points) {
    // Free the x and y arrays.
    free(points->x);
    free(points->y);
}

void freeISI(ISI *isi) {
    // Free the intervals array.
    free(isi->intervals);
}