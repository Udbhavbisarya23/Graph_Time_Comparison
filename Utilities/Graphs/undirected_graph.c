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

void readUndirectedWeightedGraph(struct Graph* graph,char* filename) {
    FILE* fp;
    fp = fopen(filename,"r");
    if(fp == NULL) {
        printf("Error in opening file\n");
        return 0;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read = -1;
    int first = 0,second=0,third=0,fourth=0;
    int vertices;
    int count = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        if(first == 0) {
            first = 1;
            continue;
        }

        if(second == 0) {
            second = 1;
            continue;
        }

        if(third == 0) {
            third = 1;
            char* garb_string = strtok(line," ");
            garb_string = strtok(NULL," ");
            char* num_vertices = strtok(NULL," ");
            printf("%s\n",num_vertices);
            vertices = atoi(num_vertices);
            graph = createGraph(vertices);
            continue;
            
        }
        
        if(fourth == 0) {
            fourth = 1;
            continue;
        }

        // For each node, the edges have to be read
        char* token = strtok(line,"\t");
        if(strlen(token) != 0) { 
            int src = atoi(token);
            token = strtok(NULL," ");
            int dest = atoi(token);
            if(src >= vertices || dest >= vertices) {
                count += 1;
                printf("Vertex index too high:- src - %d, dest - %d, vertices - %d\n",src,dest,vertices);
            }
            addEdge(graph,src,dest,1);
        }

    }
    fclose(fp);
    if (line)
        free(line);
    
    printGraph(graph);
}