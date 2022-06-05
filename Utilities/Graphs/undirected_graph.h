#ifndef undirected_graph  
#define undirected_graph

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>


struct AdjacencyListNode {
    int curr;
    int edge_weight;
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
    int weight;
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
void addToAdjacencyList(struct Graph* graph,int src,int dest,int weight);
void addEdge(struct Graph* graph,int src,int dest,int weight);
void printGraph(struct Graph* graph);
struct Graph* readUndirectedWeightedGraph(char* filename);

struct EdgeList* sortUndirectedWeightedGraph(struct Graph* graph);
void printEdgeList(struct EdgeList* edgelist);

#endif