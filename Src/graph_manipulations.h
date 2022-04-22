/**
 * @file graph_manipulations.h
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief Header file for a program that manipulates graphs.
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef GRAPH_MANIPULATIONS
#define GRAPH_MANIPULATIONS

/**
 * @brief A graph structure consisting of an adjacency matrix.
 */
typedef struct {
    /**
     * @brief The 2D adjacency matrix. Access using adjMatrix[row][column].
     */
    float **adjMatrix;

    /**
     * @brief The number of vertices in the graph.
     */
    int vertexCount;
} Graph;

/**
 * @brief Reads a graph from a formatted file.
 * 
 * @param filename the name of the file to be read.
 * @return Graph - the read in graph.
 */
Graph readGraph(char *filename);

/**
 * @brief Frees the dynamic/heap memory allocated to a graph structure.
 * 
 * @param graph the graph to be freed.
 */
void freeGraph(Graph *graph);

#endif