#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<omp.h>

#include "../data_structures/map.h"
#include "bipartite_graph.h"

struct BipartiteGraph* createBipartiteGraph(int xVertices, int yVertices) {
    //Allocation of graph memory    
    struct BipartiteGraph* graph = (struct BipartiteGraph*)malloc(sizeof(struct BipartiteGraph));

    //Initializing graph vertices
    graph->xVertices = xVertices;
    graph->yVertices = yVertices;

    //Allocation memory of Adjacency List
    graph->x = (struct AdjacencyListBipartite*)malloc(sizeof(struct AdjacencyListBipartite)*xVertices);
    for(int i=0;i<xVertices;i++) {
        graph->x[i].head = NULL;
    }

    graph->y = (struct AdjacencyListBipartite*)malloc(sizeof(struct AdjacencyListBipartite)*yVertices);
    for(int i=0;i<yVertices;i++) {
        graph->y[i].head = NULL;
    }

    return graph;
};

void addToAdjacencyListBipartite(struct BipartiteGraph* graph,int src,int dest) {
    //Creating the new Node to be added to the srcs adjacency list
    
    struct AdjacencyListNodeBipartite* newNode = (struct AdjacencyListNodeBipartite*)malloc(sizeof(struct AdjacencyListNodeBipartite));
    newNode->curr = dest;
    newNode->next = NULL;
    
    if(graph->x[src].head == NULL) {
        
        newNode->next = graph->x[src].head;
        graph->x[src].head = newNode;
    } else {
        struct AdjacencyListNodeBipartite* temp = graph->x[src].head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Now updating for destination
    struct AdjacencyListNodeBipartite* newNodeY = (struct AdjacencyListNodeBipartite*)malloc(sizeof(struct AdjacencyListNodeBipartite));
    newNodeY->curr = src;
    newNodeY->next = NULL;

    int yInd = dest - graph->xVertices;
    
    if(graph->y[yInd].head == NULL) {
        
        newNodeY->next = graph->y[yInd].head;
        graph->y[yInd].head = newNodeY;
        return;
    } else {

        struct AdjacencyListNodeBipartite* tempDest = graph->y[yInd].head;
        while(tempDest->next != NULL) {
            tempDest = tempDest->next;
        }
        tempDest->next = newNodeY;
    }

    return;
}

void addBipartiteEdge(struct BipartiteGraph* graph,int src,int dest) {
    if(src != dest) {
        addToAdjacencyListBipartite(graph,src,dest);
    }    
}

void printBipartiteGraph(struct BipartiteGraph* graph) {
    int v;
    printf("Adjacency List for all Nodes in X\n");
    for (v = 0; v < graph->xVertices; ++v) {
        struct AdjacencyListNodeBipartite* pCrawl = graph->x[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl) {
            printf("-> %d ", pCrawl->curr);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }

    printf("Adjacency List for all Nodes in Y\n");
    for (v = 0; v < graph->yVertices; ++v) {
        struct AdjacencyListNodeBipartite* pCrawl = graph->y[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl) {
            printf("-> %d ", pCrawl->curr);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

struct BipartiteGraph* readBipartiteGraph(char* filename) {
    
    struct BipartiteGraph* graph = NULL;
    
    FILE* fp;
    fp = fopen(filename,"r");
    if(fp == NULL) {
        printf("Error in opening file\n");
        return graph;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read = -1;
    int first = 0, second = 0, third = 0, fourth = 0;
    int vertices = 0;
    int xVertices, yVertices;
    int count = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        if(first == 0) {
            first = 1;
            continue;
        }

        if(second == 0) {
            char* num_vertices = strtok(line,"\t");
            num_vertices = strtok(NULL,"\t");

            num_vertices = strtok(NULL,"\t");
            vertices += atoi(num_vertices);
            xVertices = atoi(num_vertices);

            num_vertices = strtok(NULL," ");
            vertices += atoi(num_vertices);
            yVertices = atoi(num_vertices);

            graph = createBipartiteGraph(xVertices,yVertices);

            second = 1;
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

            printf("Edge:-\tSrc:-%d\tDest:-%d\n",src,dest);
            addBipartiteEdge(graph,src,dest);
        }

    }

    fclose(fp);
    if (line)
        free(line);
    
    return graph;
}

// Maximal Matching Implementation specific

int calculateDegree(struct BipartiteGraph* graph, int node) {
    struct AdjacencyListNodeBipartite* pCrawl = graph->x[node].head;
    int count = 0;
    while (pCrawl) {
        count ++;
        pCrawl = pCrawl->next;
    }

    return count;
}

struct Matching* createMatching() {
    struct Matching* matching = (struct Matching*)malloc(sizeof(struct Matching));
    matching->xVertices = 0;
    matching->yVertices = 0;

    struct HashItem* x = NULL;
    matching->xMappings = x;

    struct HashItem* y = NULL;
    matching->yMappings = y;

    struct UnweightedEdge* edges = NULL;

    matching->edges = edges;
    matching->currSize = 0;

    return matching;
}

void addEdgeToMatching(struct Matching* matching, int src, int dest) {
    if(matching->currSize == 0) {
        matching->edges = (struct UnweightedEdge*)malloc(sizeof(struct UnweightedEdge));
        
        matching->xMappings = (struct HashItem*)malloc(sizeof(struct HashItem));
        matching->xMappings[0].key = -1;
        matching->xMappings[0].value = -1;

        matching->yMappings = (struct HashItem*)malloc(sizeof(struct HashItem));
        matching->yMappings[0].key = -1;
        matching->yMappings[0].value = -1;
    }

    if(matching->currSize == (sizeof(matching->edges)/sizeof(struct UnweightedEdge))) {
        matching->edges = (struct UnweightedEdge*)realloc(matching->edges,sizeof(struct UnweightedEdge)*matching->currSize*2);
        
        matching->xMappings = (struct HashItem*)realloc(matching->xMappings,sizeof(struct HashItem)*matching->currSize*2);
        for(int i=matching->currSize+1;i<2*matching->currSize;i++) {
            matching->xMappings[i].key = -1;
            matching->xMappings[i].value = -1;
        }

        matching->yMappings = (struct HashItem*)realloc(matching->yMappings,sizeof(struct HashItem)*matching->currSize*2);
        for(int i=matching->currSize+1;i<2*matching->currSize;i++) {
            matching->yMappings[i].key = -1;
            matching->yMappings[i].value = -1;
        }
    }

    matching->edges[matching->currSize].source = src;
    matching->edges[matching->currSize].dest = dest;

    insert(matching->xMappings,src,dest,sizeof(matching->edges)/sizeof(struct UnweightedEdge));
    insert(matching->yMappings,dest,src,sizeof(matching->edges)/sizeof(struct UnweightedEdge));
    matching->currSize += 1;
    matching->xVertices ++;
    matching->yVertices ++;
}

void updateMatching(struct Matching* matching, int x, int y) {
    matching->xMappings = delete(matching->xMappings,x,matching->xVertices);
    matching->yMappings = delete(matching->yMappings,y,matching->yVertices);

    matching->xVertices --;
    matching->yVertices --;

    int count = 0; // number of edges with source x and destination y

    for(int i=0;i<matching->currSize;i++) {
        if(matching->edges[i].source == x && matching->edges[i].dest == y) {
            // swap with last element and realloc
            count ++;
            
            int temp = matching->edges[i].source;
            matching->edges[i].source = matching->edges[matching->currSize-count].source;
            matching->edges[matching->currSize-count].source = temp;

            temp = matching->edges[i].dest;
            matching->edges[i].dest = matching->edges[matching->currSize-count].dest;
            matching->edges[matching->currSize-count].dest = temp;

        }
    }

    matching->currSize = matching->currSize - count;
    matching->edges = (struct UnweightedEdge*)realloc(matching->edges,sizeof(struct UnweightedEdge)*matching->currSize);
    

}

struct UnweightedEdgeList* createUnweightedEdgeList(int numEdges) {

    struct UnweightedEdgeList* edgeList = (struct UnweightedEdgeList*)malloc(sizeof(struct UnweightedEdgeList));
    edgeList->numEdges = numEdges;
    edgeList->edges = NULL;

    if(numEdges != 0) {
        edgeList->edges = (struct UnweightedEdge*)malloc(sizeof(struct UnweightedEdge)*numEdges);
    }

    return edgeList;
}

void insertEdgeInEdgeList(struct UnweightedEdgeList* edgelist, int src, int dest) {
    if(edgelist->numEdges == 0) {
        edgelist->edges = (struct UnweightedEdge*)malloc(sizeof(struct UnweightedEdge));
    }

    if(edgelist->numEdges == sizeof(edgelist->edges)/sizeof(struct UnweightedEdge)) {
        edgelist->edges = (struct UnweightedEdge*)realloc(edgelist->edges,sizeof(struct UnweightedEdge)*edgelist->numEdges*2);
    }

    edgelist->edges[edgelist->numEdges].source = src;
    edgelist->edges[edgelist->numEdges].dest = dest;

    edgelist->numEdges ++;
}

struct Layer* createLayer() {

    struct Layer* layer;

    layer = (struct Layer*)malloc(sizeof(struct Layer));

    if(layer == NULL) {
        printf("Error while creating layer\n");
    }

    layer->numVertices = 0;
    layer->vertices = malloc(sizeof(int) * 1);
    
    return layer;
}

void printLayer(struct Layer* layer) {
    printf("Number of vertices in layer:- %d\n",layer->numVertices);
    for(int i=0;i<layer->numVertices;i++) {
        printf("%d ",layer->vertices[i]);
    }
    printf("\n");
}

void insertVertexInLayer(struct Layer* layer, int vertex) {

    if(layer->numVertices == sizeof(layer->vertices)/sizeof(int)) {
        layer->vertices = realloc(layer->vertices,sizeof(int)*layer->numVertices*2);
    }

    layer->vertices[layer->numVertices] = vertex;

    layer->numVertices ++;
}

int checkUnmatchedVertexInLayer(struct Layer* layer, struct Matching* matching) {
    for(int i=0;i<layer->numVertices;i++) {
        int currVertex = layer->vertices[i];

        if(search(matching->yMappings,currVertex,matching->yVertices) == -1) {
            return 1;
        }
    }

    return 0;
}

struct LayeredGraph* createLayeredGraph(int numLayers, struct Layer* layerList, struct UnweightedEdgeList* graphEdgeList) {
    struct LayeredGraph* layeredGraph = (struct LayeredGraph*)malloc(sizeof(struct LayeredGraph));

    layeredGraph->numLayers = numLayers;
    layeredGraph->layerList = layerList;
    layeredGraph->graphEdgeList = graphEdgeList;

    return layeredGraph;
}

struct LayeredGraphAndLayer* createLayeredGraphAndLayer(struct Layer* layer, struct LayeredGraph* layeredGraph) {
    struct LayeredGraphAndLayer* finalGraph = (struct LayeredGraphAndLayer*)malloc(sizeof(struct LayeredGraphAndLayer));

    finalGraph->layerk1 = layer;
    finalGraph->layeredGraph = layeredGraph;

    return finalGraph;
}

struct AugmentingPath* createAugmentingPath() {
    struct AugmentingPath* path = (struct AugmentingPath*)malloc(sizeof(struct AugmentingPath));

    path->numVertices = 0;
    path->nodeSequence = NULL;

    return path;
}

void insertVertexInPath(struct AugmentingPath* augPath, int vertex) {
    if(augPath->numVertices == 0) {
        augPath->nodeSequence = (int *)malloc(sizeof(int));
    }

    if(augPath->numVertices == sizeof(augPath->nodeSequence)/sizeof(int)) {
        augPath->nodeSequence = (int*)realloc(augPath->nodeSequence,sizeof(int)*2*augPath->numVertices);
    }

    augPath->nodeSequence[augPath->numVertices] = vertex;
    augPath->numVertices ++;
}

void reverseAugmentingPath(struct AugmentingPath* augPath) {
    if(augPath->numVertices == 0)
        return;
    
    int n = augPath->numVertices;
    for(int i=0;i<(n/2);i++) {
        int temp = augPath->nodeSequence[i];
        augPath->nodeSequence[i] = augPath->nodeSequence[n-i-1];
        augPath->nodeSequence[n-i-1] = temp;
    }
}

void insertPathInPathList(struct AugmentingPath* currPath, struct AugmentingPathList* augPathList) {
    if(augPathList == NULL) {
        augPathList = (struct AugmentingPathList*)malloc(sizeof(struct AugmentingPathList));
        augPathList->numAugPaths = 0;
        augPathList->pathList = (struct AugmentingPath*)malloc(sizeof(struct AugmentingPath));
    }

    if(augPathList->numAugPaths == sizeof(augPathList->pathList)/sizeof(struct AugmentingPath)) {
        augPathList->pathList = (struct AugmentingPath*)realloc(augPathList->pathList,sizeof(struct AugmentingPath)*2*augPathList->numAugPaths);
    }
    
    augPathList->pathList[augPathList->numAugPaths].numVertices = currPath->numVertices;
    augPathList->pathList[augPathList->numAugPaths].nodeSequence = currPath->nodeSequence;
    augPathList->numAugPaths ++;
}

void xorMatchingAndPathList(struct Matching* matching, struct AugmentingPathList* augPathList) {
    for(int i=0;i<augPathList->numAugPaths;i++) {

        struct AugmentingPath currPath = augPathList->pathList[i];

        for(int j=0;j<currPath.numVertices;j++) {
            int currVertex = currPath.nodeSequence[j];

            int matchedElement = search(matching->xMappings,currVertex,matching->xVertices);
            if(matchedElement != -1) {
                updateMatching(matching,currVertex,matchedElement);
            }

            matchedElement = search(matching->yMappings,currVertex,matching->yVertices);
            if(matchedElement != -1) {
                updateMatching(matching,matchedElement,currVertex);
            }
        }
    }
}