/**
 * Header file for a program that manipulates graphs.
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 3/31/22
 */

#ifndef GRAPH_MANIPULATIONS
#define GRAPH_MANIPULATIONS

/** Structures **/
typedef struct {
    float **adjMatrix;
    int vertexCount;
} Graph;

/** Forward Declarations **/
Graph readGraph(char *filename);
void freeGraph(Graph *graph);

#endif