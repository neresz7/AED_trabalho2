//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
    assert(g != NULL);
    assert(GraphIsDigraph(g));
    assert(GraphIsWeighted(g) == 0);

    InstrName[0] = "memops";  // Contagem de operações de memória
    InstrName[1] = "adds";    // Contagem de operações de adição
    InstrCalibrate();
    InstrReset();

    unsigned int numVertices = GraphGetNumVertices(g);
    Graph* transitiveClosure = GraphCreate(numVertices, 1, 0);

    for (unsigned int u = 0; u < numVertices; u++) {
        GraphBellmanFordAlg* bellmanFordResult = GraphBellmanFordAlgExecute(g, u);

        if (bellmanFordResult == NULL) {
            continue;
        }

        for (unsigned int v = 0; v < numVertices; v++) {
            if (GraphBellmanFordAlgReached(bellmanFordResult, v)) {
                GraphAddEdge(transitiveClosure, u, v);
                InstrCount[0] += 1;  // Memórias acessos
            }
        }

        GraphBellmanFordAlgDestroy(&bellmanFordResult);
    }

    InstrPrint();
    return transitiveClosure;
}
