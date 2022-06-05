#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

#include "undirected_graph.h"

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

void addToAdjacencyList(struct Graph* graph,int src,int dest,int weight) {
    //Creating the new Node to be added to the srcs adjacency list
    
    struct AdjacencyListNode* newNode = (struct AdjacencyListNode*)malloc(sizeof(struct AdjacencyListNode));
    newNode->curr = dest;
    newNode->edge_weight = weight;
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

void addEdge(struct Graph* graph,int src,int dest,int weight) {
    
    addToAdjacencyList(graph,src,dest,weight);
    addToAdjacencyList(graph,dest,src,weight);
    
}

void printGraph(struct Graph* graph) {
    int v;
    printf("In print graph function\n");
    for (v = 0; v < graph->vertices; ++v) {
        struct AdjacencyListNode* pCrawl = graph->arr[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl) {
            printf("-> %d,%d", pCrawl->curr,pCrawl->edge_weight);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

struct Graph* readUndirectedWeightedGraph(char* filename) {
    
    struct Graph* graph;
    
    FILE* fp;
    fp = fopen(filename,"r");
    if(fp == NULL) {
        printf("Error in opening file\n");
        return graph;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read = -1;
    int first = 0;
    int vertices;
    int count = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        if(first == 0) {
            first = 1;
            char* num_vertices = strtok(line,"\n");
            printf("%s\n",num_vertices);
            vertices = atoi(num_vertices);
            graph = createGraph(vertices);
            continue;
            
        }

        // For each node, the edges have to be read
        char* token = strtok(line,"\t");
        if(strlen(token) != 0) { 
            int src = atoi(token);
            token = strtok(NULL,"\t");
            int dest = atoi(token);
            if(src >= vertices || dest >= vertices) {
                count += 1;
            }
            token = strtok(NULL," ");
            int weight = atoi(token);
            addEdge(graph,src,dest,weight);
        }

    }

    fclose(fp);
    if (line)
        free(line);
    
    return graph;
}

struct EdgeList* sortUndirectedWeightedGraph(struct Graph* graph) {
    int num_edges = 0;

    for(int i=0;i<graph->vertices;i++) {
        struct AdjacencyListNode* pCrawl = graph->arr[i].head;
        while (pCrawl) {
            num_edges += 1;
            pCrawl = pCrawl->next;
        }
    }

    struct EdgeList* edgelist = (struct EdgeList*)malloc(sizeof(struct EdgeList));

    num_edges = num_edges/2;
    edgelist->num_edges = num_edges;

    struct Edge* edges = (struct Edge*)malloc(sizeof(struct Edge)*num_edges);
    int curr_ind = 0;
    for(int i=0;i<graph->vertices;i++) {
        struct AdjacencyListNode* pCrawl = graph->arr[i].head;
        while(pCrawl) {
            if(pCrawl->curr > i) {
                edges[curr_ind].node1 = i;
                edges[curr_ind].node2 = pCrawl->curr;
                edges[curr_ind].weight = pCrawl->edge_weight;
                curr_ind += 1;
            }
            pCrawl = pCrawl->next;
        }
    }

    //bubble sort
    for(int i=0;i<num_edges;i++) {
        for(int j=i+1;j<num_edges;j++) {
            if(edges[i].weight > edges[j].weight) {
                int tempNode1 = edges[i].node1;
                int tempNode2 = edges[i].node2;
                int tempWeight = edges[i].weight;

                edges[i].node1 = edges[j].node1;
                edges[i].node2 = edges[j].node2;
                edges[i].weight = edges[j].weight;

                edges[j].node1 = tempNode1;
                edges[j].node2 = tempNode2;
                edges[j].weight = tempWeight;
            }
        }
    }

    edgelist->edges = edges;

    return edgelist;
}

void printEdgeList(struct EdgeList* edgelist) {
    printf("Number of edges:- %d\n",edgelist->num_edges);
    for(int i=0;i<edgelist->num_edges;i++) {
        printf("Weight:-%d, Node1:-%d, Node2:-%d\n",edgelist->edges[i].weight,edgelist->edges[i].node1,edgelist->edges[i].node2);
    }
}