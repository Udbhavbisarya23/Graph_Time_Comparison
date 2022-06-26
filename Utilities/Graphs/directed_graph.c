#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "directed_graph.h"


struct Graph* createGraph(int vertices) {
    //Allocation of graph memory    
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

    //Initializing graph vertices
    graph->vertices = vertices;

    //Allocation memory of Adjacency List
    graph->arr = (struct AdjacencyList*)malloc(sizeof(struct AdjacencyList)*vertices);
    for(int i=0;i<vertices;i++) {
        graph->arr[i].head = NULL;
    }

    return graph;
};

void addEdge(struct Graph* graph,int src,int dest) {
    //Creating the new Node to be added to the srcs adjacency list
    struct AdjacencyListNode* newNode = (struct AdjacencyListNode*)malloc(sizeof(struct AdjacencyListNode));
    newNode->curr = dest;
    newNode->next = NULL;

    
    if(graph->arr[src].head == NULL) {
        
        newNode->next = graph->arr[src].head;
        graph->arr[src].head = newNode;
        return;
    }
    struct AdjacencyListNode* temp = graph->arr[src].head;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;

    
}

void printGraph(struct Graph* graph) {
    int v;
    for (v = 0; v < graph->vertices; ++v) {
        struct AdjacencyListNode* pCrawl = graph->arr[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl) {
            printf("-> %d", pCrawl->curr);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}