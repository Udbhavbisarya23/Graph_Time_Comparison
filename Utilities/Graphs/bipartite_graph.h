#ifndef bipartite_graph  
#define bipartite_graph

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<omp.h>

#include "../data_structures/map.h"


struct AdjacencyListNodeBipartite {
    int curr;
    struct AdjacencyListNodeBipartite* next;
};

struct AdjacencyListBipartite {
    struct AdjacencyListNodeBipartite* head;
};

struct BipartiteGraph {
    int xVertices;
    int yVertices;
    struct AdjacencyListBipartite* x;
    struct AdjacencyListBipartite* y;
};

struct UnweightedEdge {
    int source;
    int dest;
};

struct BipartiteGraph* createBipartiteGraph(int xVertices,int yVertices);
void addToAdjacencyListBipartite(struct BipartiteGraph* graph,int src,int dest);
void addBipartiteEdge(struct BipartiteGraph* graph,int src,int dest);
void printBipartiteGraph(struct BipartiteGraph* graph);
struct BipartiteGraph* readBipartiteGraph(char* filename);

// Maximal Matching Implementation Specific
struct Matching {
    struct UnweightedEdge* edges;
    struct HashItem *xMappings;
    struct HashItem *yMappings;
    int xVertices;
    int yVertices;
    int currSize;
};

int calculateDegree(struct BipartiteGraph* graph, int node);
struct Matching* createMatching();
void addEdgeToMatching(struct Matching* matching, int src, int dest);
void updateMatching(struct Matching* matching, int x, int y);

struct UnweightedEdgeList {
    int numEdges;
    struct UnweightedEdge* edges;
};

struct UnweightedEdgeList* createUnweightedEdgeList(int numEdges);
void insertEdgeInEdgeList(struct UnweightedEdgeList* edgeList, int src, int dest);

struct Layer {
    int numVertices;
    int* vertices;
};

struct Layer* createLayer();
void printLayer(struct Layer* layer);
void insertVertexInLayer(struct Layer* layer, int vertex);
int checkUnmatchedVertexInLayer(struct Layer* layer, struct Matching* matching);

struct LayeredGraph {
    int numLayers;
    struct Layer* layerList;
    struct UnweightedEdgeList* graphEdgeList;
};

struct LayeredGraph* createLayeredGraph(int numLayers, struct Layer* layerLIst, struct UnweightedEdgeList* graphEdgeList);

struct LayeredGraphAndLayer {
    struct Layer* layerk1;
    struct LayeredGraph* layeredGraph;
};

struct LayeredGraphAndLayer* createLayeredGraphAndLayer(struct Layer* layer, struct LayeredGraph* layeredGraph);

struct AugmentingPath {
    int numVertices;
    int* nodeSequence;
};

struct AugmentingPath* createAugmentingPath();
void insertVertexInPath(struct AugmentingPath* augPath, int vertex);
void reverseAugmentingPath(struct AugmentingPath* augPath);

struct AugmentingPathList {
    int numAugPaths;
    struct AugmentingPath* pathList;
};

void insertPathInPathList(struct AugmentingPath* currPath, struct AugmentingPathList* augPathList);

void xorMatchingAndPathList(struct Matching* matching, struct AugmentingPathList* augPathList);

#endif