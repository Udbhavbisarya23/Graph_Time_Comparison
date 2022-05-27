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

struct Graph* createGraph(int vertices);

void addToAdjacencyList(struct Graph* graph,int src,int dest,int weight);

void addEdge(struct Graph* graph,int src,int dest,int weight);

void printGraph(struct Graph* graph);

void readUndirectedWeightedGraph(struct Graph* graph,char* filename);

#endif