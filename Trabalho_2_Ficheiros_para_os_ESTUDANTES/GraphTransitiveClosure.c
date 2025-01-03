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
#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "instrumentation.h"

Graph* GraphComputeTransitiveClosure(Graph* g) {
    assert(g != NULL);
    assert(GraphIsDigraph(g));
    assert(GraphIsWeighted(g) == 0);

    // Inicializando a instrumentação
    InstrName[0] = "memops";  // Contagem de operações de memória
    InstrName[1] = "adds";    // Contagem de operações de adição
    InstrCalibrate();
    InstrReset();

    unsigned int numVertices = GraphGetNumVertices(g);
    Graph* transitiveClosure = GraphCreate(numVertices, 1, 0);
    InstrCount[0] += 1;  // Incrementando operações de memória para GraphCreate

    for (unsigned int u = 0; u < numVertices; u++) {
        GraphBellmanFordAlg* bellmanFordResult = GraphBellmanFordAlgExecute(g, u);

        if (bellmanFordResult == NULL) {
            continue;
        }

        for (unsigned int v = 0; v < numVertices; v++) {
            if (u != v && GraphBellmanFordAlgReached(bellmanFordResult, v)) {
                GraphAddEdge(transitiveClosure, u, v);
                InstrCount[0] += 1;  // Incrementando operações de memória para GraphAddEdge
            }
        }

        GraphBellmanFordAlgDestroy(&bellmanFordResult);
        InstrCount[0] += 1;  // Incrementando operações de memória para GraphBellmanFordAlgDestroy
    }

    InstrPrint();
    return transitiveClosure;
}