#ifndef directed_graph  
#define directed_graph

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct AdjacencyListNode {
    int curr;
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
void addEdge(struct Graph* graph,int src,int dest);
void printGraph(struct Graph* graph);

#endif