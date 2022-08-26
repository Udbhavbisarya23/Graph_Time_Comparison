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
void freeBipartiteGraph(struct BipartiteGraph* graph);

// Maximal Matching Implementation Specific
struct Matching {
    struct UnweightedEdge* edges;
    int currSize;
    int maxSize;

    struct HashItem *xMappings;
    int xVertices;
    int xMaxSize;

    struct HashItem *yMappings;
    int yVertices;
    int yMaxSize;
};

int calculateDegree(struct BipartiteGraph* graph, int node);
struct Matching* createMatching();
void printMatching(struct Matching* matching);
void addEdgeToMatching(struct Matching* matching, int src, int dest);
void updateMatching(struct Matching* matching, int x, int y);
void freeMatching(struct Matching* matching);

struct UnweightedEdgeList {
    int numEdges;
    int maxEdgeListSize;
    struct UnweightedEdge* edges;
};

struct UnweightedEdgeList* createUnweightedEdgeList(int numEdges);
void insertEdgeInEdgeList(struct UnweightedEdgeList* edgeList, int src, int dest);

struct Layer {
    int numVertices;
    int maxLayerSize;
    int* vertices;
};

struct Layer* createLayer();
void printLayer(struct Layer* layer);
void insertVertexInLayer(struct Layer* layer, int vertex);
int checkUnmatchedVertexInLayer(struct Layer* layer, struct Matching* matching);
void freeLayer(struct Layer* layer);

struct LayeredGraph {
    int numLayers;
    struct Layer* layerList;
    struct UnweightedEdgeList* graphEdgeList;
};

struct LayeredGraph* createLayeredGraph(int numLayers, struct Layer* layerLIst, struct UnweightedEdgeList* graphEdgeList);
void freeLayeredGraph(struct LayeredGraph* graph);

struct LayeredGraphAndLayer {
    struct Layer* layerk1;
    struct LayeredGraph* layeredGraph;
};

struct LayeredGraphAndLayer* createLayeredGraphAndLayer(struct Layer* layer, struct LayeredGraph* layeredGraph);
void freeLayeredGraphAndLayer(struct LayeredGraphAndLayer* lgLayer);

struct AugmentingPath {
    int numVertices;
    int maxPathSize;
    int* nodeSequence;
};

struct AugmentingPath* createAugmentingPath();
void printAugmentingPath(struct AugmentingPath* currPath);
void insertVertexInPath(struct AugmentingPath* augPath, int vertex);
void reverseAugmentingPath(struct AugmentingPath* augPath);

struct AugmentingPathList {
    int numAugPaths;
    int maxNumberOfPaths;
    struct AugmentingPath* pathList;
};

struct AugmentingPathList* createAugPathList();
void printAugPathList(struct AugmentingPathList* list);
void insertPathInPathList(struct AugmentingPath* currPath, struct AugmentingPathList* augPathList);
void freeAugPathList(struct AugmentingPathList* currPathList);

void xorMatchingAndPathList(struct Matching* matching, struct AugmentingPathList* augPathList, int xVertices);

#endif