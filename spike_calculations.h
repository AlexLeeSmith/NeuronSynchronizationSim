/**
 * Header file for a program that implements calculations on equations that exhibit spiking behavior.
 */

#ifndef SPIKE_CALCULATIONS
#define SPIKE_CALCULATIONS

/** Structures **/
typedef struct {
    float *x;
    float *y;
    int size;
} Points;

/** Forward Declarations **/
void findSpikes(Points *spikes, float x[], float y[], int size);
void getFrequency(Points *spikes);
void getInterSpikeInterval(Points *spikes);
void freeSpikes(Points *spikes);

#endif