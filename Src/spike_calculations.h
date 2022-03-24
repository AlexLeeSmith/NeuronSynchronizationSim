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
void findSpikes(Points *spikes, float x[], float y[], int size, float transient);
double getAveFrequency(int spikeCount, int transient, int xEnd, float scale);
int getInterSpikeIntervals(Points *spikes, float intervals[]);
void writePoints(char filename[], Points *points);
void writeInterSpikeIntervals(char filename[], float *intervals, int size);
void freePoints(Points *points);

#endif