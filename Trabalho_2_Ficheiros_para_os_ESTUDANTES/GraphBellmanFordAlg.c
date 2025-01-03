//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name : João Silva
// Student Number : 120172
// Student Name : Felipe Silva
// Student Number : 117127

/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Graph.h"
#include "IntegersStack.h"
#include "SortedList.h"  // Presumindo que as funções de lista estão definidas aqui
#include "instrumentation.h"

struct _Vertex {
    unsigned int id;
    unsigned int inDegree;
    unsigned int outDegree;
    List* edgesList;
};

struct _Edge {
    unsigned int adjVertex;
    double weight;
};

struct _GraphHeader {
    int isDigraph;
    int isComplete;
    int isWeighted;
    unsigned int numVertices;
    unsigned int numEdges;
    List* verticesList;
};

struct _GraphBellmanFordAlg {
    unsigned int* marked;  // Para marcar vértices quando alcançados pela primeira vez
    int* distance;  // O número de arestas no caminho a partir do vértice inicial
                    // distance[i] = -1, se nenhum caminho for encontrado do vértice inicial para i
    int* predecessor;  // O vértice predecessor no caminho mais curto
                       // predecessor[i] = -1, se nenhum predecessor existir
    Graph* graph;
    unsigned int startVertex;  // A raiz da árvore de caminhos mais curtos
};

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g, unsigned int startVertex) {
    assert(g != NULL);
    assert(startVertex < GraphGetNumVertices(g));
    assert(GraphIsWeighted(g) == 0);

    // Inicializando a instrumentação
    InstrName[0] = "memops";  // Contagem de operações de memória
    InstrName[1] = "adds";    // Contagem de operações de adição
    InstrCalibrate();
    InstrReset();

    GraphBellmanFordAlg* result = (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
    assert(result != NULL);
    InstrCount[0] += 1;  // Incrementando operações de memória

    result->graph = g;
    result->startVertex = startVertex;
    unsigned int numVertices = GraphGetNumVertices(g);

    // Inicializando as estruturas
    result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
    result->distance = (int*)malloc(numVertices * sizeof(int));
    result->predecessor = (int*)malloc(numVertices * sizeof(int));
    InstrCount[0] += 3;  // Incrementando operações de memória

    for (unsigned int i = 0; i < numVertices; i++) {
        result->distance[i] = INT_MAX;
        result->predecessor[i] = -1;
    }
    result->distance[startVertex] = 0;
    InstrCount[0] += numVertices * 3;  // Incrementando operações de memória

    // Relaxamento das arestas
    for (unsigned int i = 1; i < numVertices; i++) {
        ListMoveToHead(g->verticesList);
        for (unsigned int u = 0; u < numVertices; ListMoveToNext(g->verticesList), u++) {
            struct _Vertex* vertex = ListGetCurrentItem(g->verticesList);
            List* edges = vertex->edgesList;
            ListMoveToHead(edges);
            for (unsigned int j = 0; j < ListGetSize(edges); ListMoveToNext(edges), j++) {
                struct _Edge* edge = ListGetCurrentItem(edges);
                unsigned int v = edge->adjVertex;
                InstrCount[0] += 3;  // Operações de memória
                if (result->distance[u] != INT_MAX && result->distance[u] + 1 < result->distance[v]) {
                    result->distance[v] = result->distance[u] + 1;
                    result->predecessor[v] = u;
                    InstrCount[1] += 1;  // Operação de adição
                }
            }
        }
    }

    // Verificação de ciclos negativos
    ListMoveToHead(g->verticesList);
    for (unsigned int u = 0; u < numVertices; ListMoveToNext(g->verticesList), u++) {
        struct _Vertex* vertex = ListGetCurrentItem(g->verticesList);
        List* edges = vertex->edgesList;
        ListMoveToHead(edges);
        for (unsigned int j = 0; j < ListGetSize(edges); ListMoveToNext(edges), j++) {
            struct _Edge* edge = ListGetCurrentItem(edges);
            unsigned int v = edge->adjVertex;
            if (result->distance[u] != INT_MAX && result->distance[u] + 1 < result->distance[v]) {
                printf("Graph contains a negative-weight cycle\n");
                free(result->marked);
                free(result->distance);
                free(result->predecessor);
                free(result);
                InstrPrint();
                return NULL;
            }
        }
    }

    // Marcando os vértices alcançados
    for (unsigned int i = 0; i < numVertices; i++) {
        if (result->distance[i] != INT_MAX) {
            result->marked[i] = 1;
        }
    }

    InstrPrint();
    return result;
}

void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
    assert(*p != NULL);

    GraphBellmanFordAlg* aux = *p;

    free(aux->marked);
    free(aux->predecessor);
    free(aux->distance);

    free(*p);
    *p = NULL;
}

// Getting the paths information

int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
    assert(p != NULL);
    assert(v < GraphGetNumVertices(p->graph));

    return p->marked[v];
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
    assert(p != NULL);
    assert(v < GraphGetNumVertices(p->graph));

    return p->distance[v];
}

Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
    assert(p != NULL);
    assert(v < GraphGetNumVertices(p->graph));

    Stack* s = StackCreate(GraphGetNumVertices(p->graph));

    if (p->marked[v] == 0) {
        return s;
    }

    // Store the path
    for (unsigned int current = v; current != p->startVertex;
         current = p->predecessor[current]) {
        StackPush(s, current);
    }

    StackPush(s, p->startVertex);

    return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
    assert(p != NULL);
    assert(v < GraphGetNumVertices(p->graph));

    Stack* s = GraphBellmanFordAlgPathTo(p, v);

    while (StackIsEmpty(s) == 0) {
        printf("%d ", StackPop(s));
    }

    StackDestroy(&s);
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
    assert(p != NULL);

    Graph* original_graph = p->graph;
    unsigned int num_vertices = GraphGetNumVertices(original_graph);

    // The paths tree is a digraph, with no edge weights
    Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

    // Use the predecessors array to add the tree edges
    for (unsigned int w = 0; w < num_vertices; w++) {
        // Vertex w has a predecessor vertex v?
        int v = p->predecessor[w];
        if (v != -1) {
            GraphAddEdge(paths_tree, (unsigned int)v, w);
        }
    }

    // Display the tree in the DOT format
    GraphDisplayDOT(paths_tree);

    // Housekeeping
    GraphDestroy(&paths_tree);
}
