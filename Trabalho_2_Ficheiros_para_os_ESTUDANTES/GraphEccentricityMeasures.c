//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"
#include <limits.h>

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
    assert(g != NULL);

    unsigned int numVertices = GraphGetNumVertices(g);
    GraphEccentricityMeasures* result = (GraphEccentricityMeasures*)malloc(sizeof(struct _GraphEccentricityMeasures));
    assert(result != NULL);

    result->graph = g;
    result->eccentricity = (int*)malloc(numVertices * sizeof(int));
    assert(result->eccentricity != NULL);
    result->graphRadius = INT_MAX;
    result->graphDiameter = 0;

    GraphAllPairsShortestDistances* allPairsDistances = GraphAllPairsShortestDistancesExecute(g);
    assert(allPairsDistances != NULL);

    // Calcular a excentricidade de cada vértice
    for (unsigned int v = 0; v < numVertices; v++) {
        int maxDistance = 0;
        for (unsigned int w = 0; w < numVertices; w++) {
            int distance = GraphGetDistanceVW(allPairsDistances, v, w);
            if (distance > maxDistance) {
                maxDistance = distance;
            }
        }
        result->eccentricity[v] = (maxDistance == -1) ? INT_MAX : maxDistance;

        if (result->eccentricity[v] < result->graphRadius) {
            result->graphRadius = result->eccentricity[v];
        }
        if (result->eccentricity[v] > result->graphDiameter) {
            result->graphDiameter = result->eccentricity[v];
        }
    }

    // Computar o conjunto de vértices centrais
    unsigned int numCentralVertices = 0;
    for (unsigned int v = 0; v < numVertices; v++) {
        if (result->eccentricity[v] == result->graphRadius) {
            numCentralVertices++;
        }
    }

    result->centralVertices = (unsigned int*)malloc((numCentralVertices + 1) * sizeof(unsigned int));
    assert(result->centralVertices != NULL);

    result->centralVertices[0] = numCentralVertices;
    unsigned int index = 1;
    for (unsigned int v = 0; v < numVertices; v++) {
        if (result->eccentricity[v] == result->graphRadius) {
            result->centralVertices[index++] = v;
        }
    }

    GraphAllPairsShortestDistancesDestroy(&allPairsDistances);

    return result;
}


void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
    assert(p != NULL);
    assert(p->centralVertices != NULL);

    unsigned int numCentralVertices = p->centralVertices[0];
    unsigned int* centralVerticesCopy = (unsigned int*)malloc((numCentralVertices + 1) * sizeof(unsigned int));
    assert(centralVerticesCopy != NULL);

    for (unsigned int i = 0; i <= numCentralVertices; i++) {
        centralVerticesCopy[i] = p->centralVertices[i];
    }

    return centralVerticesCopy;
}


// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
    assert(p != NULL);

    printf("Graph Radius: %d\n", p->graphRadius);
    printf("Graph Diameter: %d\n", p->graphDiameter);

    printf("Vertex Eccentricities:\n");
    for (unsigned int v = 0; v < GraphGetNumVertices(p->graph); v++) {
        printf("Vertex %u: %d\n", v, p->eccentricity[v]);
    }

    printf("Central Vertices (Eccentricity equal to Radius):\n");
    unsigned int numCentralVertices = p->centralVertices[0];
    for (unsigned int i = 1; i <= numCentralVertices; i++) {
        printf("Vertex %u\n", p->centralVertices[i]);
    }
}

