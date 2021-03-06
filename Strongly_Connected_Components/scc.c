#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include "../Utilities/Graphs/directed_graph.h"
#include "../Utilities/data_structures/queue.h"

#define CAPACITY 100000

// Implementation of Kosaraju's algorithm and all related functions
void DFSTimes(int vertex, struct Graph* graph, int* visited, int* stack, int* time) {
    *time += 1;
    // start_time[vertex] = *time;
    visited[vertex] = 1;
    struct AdjacencyListNode* node = graph->arr[vertex].head;
    while(node != NULL) {
        if(visited[node->curr] == 0) {
            DFSTimes(node->curr,graph,visited,stack,time);
        }
        node = node->next;
    } 
    *time += 1;
    // end_time[vertex] = *time;
    for(int i=0;i<graph->vertices;i++) {
        if(stack[i]==-1) {
            stack[i] = vertex;
            return;
        }
    }
}

struct Graph* TransposeGraph(struct Graph* graph){
    struct Graph* fin_graph = createGraph(graph->vertices);
    for(int i=0;i<graph->vertices;i++) {
        struct AdjacencyListNode *node = graph->arr[i].head;
        while(node != NULL) {
            addEdge(fin_graph,node->curr,i);
            node = node->next;
        }
    }
    return fin_graph;
}

int scc_count = 0;

void DFS(int vertex, struct Graph* graph, int* visited, int* scc, int parent) {
    if(visited[vertex] == 1) {
        return;
    }
    visited[vertex] = 1;
    if(parent==-1) {
        scc[vertex] = vertex;
        scc_count += 1;
    } else {
        scc[vertex] = scc[parent];
    }

    struct AdjacencyListNode* node = graph->arr[vertex].head;
    while(node != NULL) {
        if(visited[node->curr] == 0) {
            DFS(node->curr,graph,visited,scc,vertex);
        }
        node = node->next;
    }
}

void BFS(int vertex, struct Graph* graph, int* visited, int* scc, int parent) {

    visited[vertex] = 1;

    struct Queue* mainQueue = createQueue(CAPACITY);
    
    struct AdjacencyListNode* node = graph->arr[vertex].head;
    enqueue(mainQueue, node);

    while(!isEmpty(mainQueue)) {
        
        node = dequeue(mainQueue);

        while(node != NULL) {
            if(visited[node->curr] == 0) {
                visited[node->curr] = 1;
                enqueue(mainQueue,node);
            }
            node = node->next;
        }
    }

}

void printSCC(int* scc,int vertices) {
    for(int i=0;i<vertices;i++) {
        printf("SCC of vertex %d :- %d\n",i,scc[i]);
    }    
}

void Kosaraju(struct Graph* graph){
    //STEP 1
    //initial DFS setup
    int vertices = graph->vertices;

    int stack[vertices];
    int visited[vertices];
    // int start_time[vertices];
    // int end_time[vertices];
    int time = 0;
    
    for(int i=0;i<vertices;i++) {
        visited[i] = 0;
        stack[i] = -1;

        // start_time[i] = -1;
        // end_time[i] = -1;
    }

    //Applying DFS on graph G and recording times
    for(int i=0;i<vertices;i++) {
        if(visited[i] == 0) {
            DFSTimes(i, graph, visited, stack, &time);
        }
    }

    // Printing start time and end time for each vertex. Also printing the stack
    // for(int i=0;i<vertices;i++) {
    //     printf("%d :\tStart time: %d\tEnd time: %d\n",i,start_time[i],end_time[i]);
    // }


    //STEP 2 
    //Graph Transpose
    struct Graph* graph_T;
    graph_T = TransposeGraph(graph);

    //STEP 3
    // DFS in the reverse order
    int scc[vertices];
    for(int i=0;i<vertices;i++) {
        visited[i] = 0;
        scc[i] = -1;
    }
    for(int i=vertices-1;i>=0;i--) {
        DFS(stack[i],graph_T,visited,scc,-1);
    }
};

int main() {
    FILE* fp;
    fp = fopen("../../Graphs/as-caida20071105_sanitized.txt","r");
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
    struct Graph* graph;

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
            addEdge(graph,src,dest);
        }

    }
    fclose(fp);
    if (line)
        free(line);

    struct timeval TimeValue_Start;
    struct timezone TimeZone_Start;
    struct timeval TimeValue_Final;
    struct timezone TimeZone_Final;
    long time_start, time_end;
    double time_overhead;

    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    Kosaraju(graph);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    printf("Sequential SCC Time: %lf\n", time_overhead);
    printf("Number of SCC's:- %d\n",scc_count);
}