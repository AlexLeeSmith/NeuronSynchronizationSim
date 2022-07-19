/**
 * @file spike_calculations.h
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief Header file for a program that implements calculations on equations that exhibit spiking behavior.
 * @version 0.1
 * @date 2022-02-22
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef SPIKE_CALCULATIONS
#define SPIKE_CALCULATIONS

/**
 * @brief A points structure of x and y arrays.
 */
typedef struct {
    /**
     * @brief The array of x values.
     */
    float *x;

    /**
     * @brief The array of y values.
     */
    float *y;

    /**
     * @brief The size of the x and y arrays.
     */
    int size;
} Points;

/**
 * @brief A structure of inter-spike intervals.
 */
typedef struct {
    /**
     * @brief The array of intervals.
     */
    float *intervals;

    /**
     * @brief The size of the intervals arrays.
     */
    int size;
} ISI;


/**
 * @brief Initializes and allocates memory for a points struture.
 * 
 * @param size the size of the x and y arrays.
 * @return Points - the initialized points struture.
 */
Points initPoints(int size);

/**
 * @brief Initializes and allocates memory for an ISI struture.
 * 
 * @param size the size of the intervals arrays.
 * @return ISI - the initialized ISI struture.
 */
ISI initISI(int size);

/**
 * @brief Finds the spikes within the given set of points above a specified treshold.
 * 
 * @param x an array of x coordinates.
 * @param y an array of y coordinates.
 * @param size the size of the x and y arrays (must be at least 3).
 * @param transient the x position in which the differential equation starts exhibiting its normal behavior.
 * @param threshold the minimum value a spike must reach.
 * @return Points - the points struture of the found spikes.
 */
Points findSpikes(float x[], float y[], int size, float transient, float threshold);

/**
 * @brief Calculates the average frequency.
 * 
 * @param spikeCount the number of spikes.
 * @param transient the x position in which the differential equation starts exhibiting its normal behavior.
 * @param xEnd The final x position.
 * @param scale the factor to scale the result by.
 * @return float - the calculated average frequency ((spikeCount / (xEnd - transient)) * scale).
 */
float calcAvgFrequency(int spikeCount, int transient, int xEnd, float scale);

/**
 * @brief Calculates the inter-spike intervals within the given set of spikes.
 * 
 * @param spikes the Points struture where the spikes are stored.
 * @return ISI - the ISI struture of the calculated intervals.
 */
ISI calcISI(Points *spikes);

/**
 * @brief Writes a set of points to a file.
 * 
 * @param filename the name of the file to write to.
 * @param points the Points struture to be written.
 */
void writePoints(char filename[], Points *points);

/**
 * @brief Writes an array of inter-spike intervals to a file.
 * 
 * @param filename the name of the file to write to.
 * @param isi the ISI struture to be written.
 */
void writeISI(char filename[], ISI *isi);

/**
 * @brief Writes an array of average frequencies to a file.
 * 
 * @param filename the name of the file to write to.
 * @param avgFreqs the array of frequencies to be written.
 * @param size the number of frequencies.
 */
void writeAvgFrequencies(char filename[], float avgFreqs[], int size);

/**
 * @brief Frees the heap memory allocated to a Points struture.
 * 
 * @param points the Points struture to be freed.
 */
void freePoints(Points *points);

/**
 * @brief Frees the heap memory allocated to an ISI struture.
 * 
 * @param isi the ISI struture to be freed.
 */
void freeISI(ISI *isi);

#endif