#ifndef undirected_graph  
#define undirected_graph

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<omp.h>


struct AdjacencyListNode {
    int curr;
    float edge_weight;
    struct AdjacencyListNode* next;
};

struct AdjacencyList {
    struct AdjacencyListNode* head;
};

struct Graph {
    int vertices;
    struct AdjacencyList* arr;
};

struct Edge {
    float weight;
    int node1;
    int node2;
};

struct EdgeList {
    int num_edges;
    struct Edge* edges;
};

struct EdgeListPartition {
    struct EdgeList* LessThanPivotPartition;
    struct EdgeList* MoreThanPivotPartition;
};

struct Graph* createGraph(int vertices);
void addToAdjacencyList(struct Graph* graph,int src,int dest,float weight);
void addEdge(struct Graph* graph,int src,int dest,float weight);
void printGraph(struct Graph* graph);
struct Graph* readUndirectedWeightedGraph(char* filename);

void merge(struct Edge* edges, int l, int m, int r);
void mergeSort(struct Edge* edges, int l, int r);
struct EdgeList* sortUndirectedWeightedGraph(struct Graph* graph);

void printEdgeList(struct EdgeList* edgelist);
void swapEdgeList(struct EdgeList* edgelist,int i,int j);
int partitionEdgeList(struct EdgeList* edgelist,int low, int high);
void sortSerialEdgeList(struct EdgeList* edgelist, int low, int high);
void sortParallelEdgeList(struct EdgeList* edgelist,int low,int high);


#endif