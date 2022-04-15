/**
 * This file implements the graph manipulations header file.
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @date 3/31/22
 */

/** Preprocessor Directives **/
#include "graph_manipulations.h"

#include <stdlib.h>     // exit(), EXIT_SUCCESS, EXIT_FAILURE, malloc(), free(), strtod()
#include <stdio.h>      // fprintf(), FILE, fopen(), fclose(), perror()
#include <string.h>     // strtok()

#define DELIMITER " \t\r\n\v\f"
#define MAX_EDGE_CHARS 10

/** Functions **/
Graph readGraph(char *filename) {
    Graph graph;

    // Open output file for reading.
    FILE *infile;
    if ((infile = fopen(filename, "r")) == NULL) {
        perror("Open Graph File");
        exit(EXIT_FAILURE);
    }

    // Get the vertex count.
    if (fscanf(infile, "# %d\n", &graph.vertexCount) != 1) {
        perror("Missing Vertex Count");
        exit(EXIT_FAILURE);
    }
    
    // Allocate the buffer and adjacency matrix memory.
    long bufferSize = graph.vertexCount * MAX_EDGE_CHARS * sizeof(char);
    char *buffer = (char *) malloc(bufferSize);
    if ((buffer) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }
    if ((graph.adjMatrix = (float **) malloc(graph.vertexCount * sizeof(float *))) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < graph.vertexCount; ++i) {
        if ((graph.adjMatrix[i] = (float *) malloc(graph.vertexCount * sizeof(float))) == NULL) {
            perror("malloc() failure");
            exit(EXIT_FAILURE);
        }        
    }
    
    // Read in the edges.
    for (int row = 0; row < graph.vertexCount; ++row) {
        if (fgets(buffer, bufferSize, infile) == NULL) {
            fprintf(stderr, "Vertex count > # of rows, exiting ...\n");
            exit(EXIT_FAILURE);
        }

        int col = 0;
        for (char *token = strtok(buffer, DELIMITER); token != NULL; token = strtok(NULL, DELIMITER), ++col) {
            graph.adjMatrix[row][col] = strtod(token, NULL);

            if (col >= graph.vertexCount) {
                fprintf(stderr, "Vertex count < # of columns, exiting ...\n");
                exit(EXIT_FAILURE);
            }
        }
        if (col < graph.vertexCount) {
            fprintf(stderr, "Vertex count > # of columns, exiting ...\n");
            exit(EXIT_FAILURE);
        }
    }

    // Close input file.
    fclose(infile);
    free(buffer);
    return graph;
}

void freeGraph(Graph *graph) {
    for (int i = 0; i < graph->vertexCount; ++i) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
}