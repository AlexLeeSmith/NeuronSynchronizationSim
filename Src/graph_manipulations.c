/**
 * @file graph_manipulations.c
 * @author Alex Smith (alsmi14@ilstu.edu)
 * @brief This file implements the graph manipulations header file.
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 */

#include "graph_manipulations.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DELIMITER " \t\r\n\v\f" // The legal characters that may be used to separate the edges.
#define MAX_EDGE_CHARS 10       // The maximum amount of characters that the weight of an edge may be.

Graph readGraph(char *filename) {
    Graph graph;

    // Open graph file for reading.
    FILE *graphFile;
    if ((graphFile = fopen(filename, "r")) == NULL) {
        perror("Open Graph File");
        exit(EXIT_FAILURE);
    }

    // Get the vertex count.
    if (fscanf(graphFile, "# %d\n", &graph.vertexCount) != 1) {
        perror("Missing Vertex Count");
        exit(EXIT_FAILURE);
    }
    
    // Allocate the adjacency matrix memory.
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

    // Allocate the buffer memory for reading in the edges.
    long bufferSize = graph.vertexCount * MAX_EDGE_CHARS * sizeof(char);
    char *buffer = (char *) malloc(bufferSize);
    if ((buffer) == NULL) {
        perror("malloc() failure");
        exit(EXIT_FAILURE);
    }
    
    // Read in the edges.
    for (int row = 0; row < graph.vertexCount; ++row) {
        // Get a row from the file.
        if (fgets(buffer, bufferSize, graphFile) == NULL) {
            fprintf(stderr, "Vertex count > # of rows, exiting ...\n");
            exit(EXIT_FAILURE);
        }

        // Tokenize and add each edge to the adjacency matrix.
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

    // Close graph file, free buffer, and return.
    fclose(graphFile);
    free(buffer);
    return graph;
}

void freeGraph(Graph *graph) {
    // Free each row in the adjacency matrix.
    for (int i = 0; i < graph->vertexCount; ++i) {
        free(graph->adjMatrix[i]);
    }

    // Free the adjacency matrix itself.
    free(graph->adjMatrix);
}