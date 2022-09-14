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

            //printf("Edge:-\tSrc:-%d\tDest:-%d\n",src,dest);
            addBipartiteEdge(graph,src,dest);
        }

    }

    fclose(fp);
    if (line)
        free(line);
    
    return graph;
}

void freeBipartiteGraph(struct BipartiteGraph* graph) {

    for(int i=0;i<graph->xVertices;i++) {
        struct AdjacencyListNodeBipartite* temp = graph->x[i].head;
        struct AdjacencyListNodeBipartite* curr = temp;
        struct AdjacencyListNodeBipartite* next;

        while(curr) {
            next = curr->next;
            free(curr);
            curr = next;
        }
    }

    for(int i=0;i<graph->yVertices;i++) {
        struct AdjacencyListNodeBipartite* temp = graph->y[i].head;
        struct AdjacencyListNodeBipartite* curr = temp;
        struct AdjacencyListNodeBipartite* next;

        while(curr) {
            next = curr->next;
            free(curr);
            curr = next;
        }
    }

    free(graph->x);
    free(graph->y);
    free(graph);
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

    struct HashItem* x = (struct HashItem*)malloc(sizeof(struct HashItem));
    matching->xMappings = x;
    matching->xMaxSize = 1;
    matching->xMappings[0].key = -1;
    matching->xMappings[0].value = -1;

    struct HashItem* y = (struct HashItem*)malloc(sizeof(struct HashItem));
    matching->yMappings = y;
    matching->yMaxSize = 1;
    matching->yMappings[0].key = -1;
    matching->yMappings[0].value = -1;

    matching->edges = (struct UnweightedEdge*)malloc(sizeof(struct UnweightedEdge));
    matching->currSize = 0;
    matching->maxSize = 1;

    return matching;
}

void printMatching(struct Matching* matching) {
    printf("Current Matching is as follows:-\n");
    for(int i=0;i<matching->currSize;i++) {
        printf("Edge:-%d\tSrc:-%d\tDest:-%d\n",i+1,matching->edges[i].source,matching->edges[i].dest);
    }
    printf("\n");
}

void addEdgeToMatching(struct Matching* matching, int src, int dest) {
    if(matching->maxSize == 0) {
        matching->edges = (struct UnweightedEdge*)malloc(sizeof(struct UnweightedEdge));
        matching->maxSize = 1;
        
        matching->xMappings = (struct HashItem*)malloc(sizeof(struct HashItem));
        matching->xMappings[0].key = -1;
        matching->xMappings[0].value = -1;

        matching->yMappings = (struct HashItem*)malloc(sizeof(struct HashItem));
        matching->yMappings[0].key = -1;
        matching->yMappings[0].value = -1;
    }

    if(matching->currSize == matching->maxSize) {
        matching->maxSize = matching->maxSize*2;
        matching->xMaxSize = matching->xMaxSize*2;
        matching->yMaxSize = matching->yMaxSize*2;

        matching->edges = (struct UnweightedEdge*)realloc(matching->edges,sizeof(struct UnweightedEdge)*matching->maxSize);
        
        matching->xMappings = (struct HashItem*)realloc(matching->xMappings,sizeof(struct HashItem)*matching->xMaxSize);
        for(int i=matching->currSize;i<matching->xMaxSize;i++) {
            matching->xMappings[i].key = -1;
            matching->xMappings[i].value = -1;
        }

        matching->yMappings = (struct HashItem*)realloc(matching->yMappings,sizeof(struct HashItem)*matching->yMaxSize);
        for(int i=matching->currSize;i<matching->yMaxSize;i++) {
            matching->yMappings[i].key = -1;
            matching->yMappings[i].value = -1;
        }
    }

    matching->edges[matching->currSize].source = src;
    matching->edges[matching->currSize].dest = dest;
    matching->currSize ++;

    insert(matching->xMappings,src,dest,matching->xMaxSize);
    insert(matching->yMappings,dest,src,matching->yMaxSize);
    matching->xVertices ++;
    matching->yVertices ++;
}

void updateMatching(struct Matching* matching, int x, int y) {
    delete(matching->xMappings,x,matching->xVertices);
    delete(matching->yMappings,y,matching->yVertices);

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
            break;

        }
    }

    matching->currSize = matching->currSize - count;    

}

void freeMatching(struct Matching* matching) {
    free(matching->xMappings);
    free(matching->yMappings);
    free(matching->edges);
    free(matching);
}

struct UnweightedEdgeList* createUnweightedEdgeList(int numEdges) {

    struct UnweightedEdgeList* edgeList = (struct UnweightedEdgeList*)malloc(sizeof(struct UnweightedEdgeList));
    edgeList->numEdges = numEdges;
    edgeList->maxEdgeListSize = numEdges;
    edgeList->edges = NULL;

    if(numEdges != 0) {
        edgeList->edges = (struct UnweightedEdge*)malloc(sizeof(struct UnweightedEdge)*numEdges);
    }

    return edgeList;
}

void insertEdgeInEdgeList(struct UnweightedEdgeList* edgelist, int src, int dest) {
    if(edgelist->numEdges == 0) {
        edgelist->edges = (struct UnweightedEdge*)malloc(sizeof(struct UnweightedEdge));
        edgelist->maxEdgeListSize = 1;
    }

    if(edgelist->numEdges == edgelist->maxEdgeListSize) {
        edgelist->maxEdgeListSize = edgelist->maxEdgeListSize * 2;
        edgelist->edges = (struct UnweightedEdge*)realloc(edgelist->edges,sizeof(struct UnweightedEdge)*edgelist->maxEdgeListSize);
    }

    edgelist->edges[edgelist->numEdges].source = src;
    edgelist->edges[edgelist->numEdges].dest = dest;

    edgelist->numEdges ++;
}

void freeUnweightedEdgeList(struct UnweightedEdgeList* edgeList) {
    free(edgeList->edges);
    free(edgeList);
}

struct Layer* createLayer() {

    struct Layer* layer;

    layer = malloc(sizeof *layer * 1);

    if(layer == NULL) {
        printf("Error while creating layer\n");
    }

    layer->numVertices = 0;
    layer->maxLayerSize = 1;

    layer->vertices = malloc(sizeof *layer->vertices * 1);
    
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

    if(layer->numVertices == layer->maxLayerSize) {
        layer->maxLayerSize = layer->maxLayerSize * 2;
        layer->vertices = realloc(layer->vertices,sizeof(int)*layer->maxLayerSize);
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

void freeLayer(struct Layer* layer) {
    free(layer->vertices);
    free(layer);
}

struct LayeredGraph* createLayeredGraph(int numLayers, struct Layer* layerList, struct UnweightedEdgeList* graphEdgeList) {
    struct LayeredGraph* layeredGraph = (struct LayeredGraph*)malloc(sizeof(struct LayeredGraph));

    layeredGraph->numLayers = numLayers;
    layeredGraph->layerList = layerList;
    layeredGraph->graphEdgeList = graphEdgeList;

    return layeredGraph;
}

void freeLayeredGraph(struct LayeredGraph* graph) {
    free(graph);
}

struct LayeredGraphAndLayer* createLayeredGraphAndLayer(struct Layer* layer, struct LayeredGraph* layeredGraph) {
    struct LayeredGraphAndLayer* finalGraph = (struct LayeredGraphAndLayer*)malloc(sizeof(struct LayeredGraphAndLayer));

    finalGraph->layerk1 = layer;
    finalGraph->layeredGraph = layeredGraph;

    return finalGraph;
}

void freeLayeredGraphAndLayer(struct LayeredGraphAndLayer* lgLayer) {
    freeLayer(lgLayer->layerk1);
    freeLayeredGraph(lgLayer->layeredGraph);
    free(lgLayer);
}

struct AugmentingPath* createAugmentingPath() {
    struct AugmentingPath* path = (struct AugmentingPath*)malloc(sizeof(struct AugmentingPath));

    path->numVertices = 0;
    path->maxPathSize = 0;
    path->nodeSequence = NULL;

    return path;
}

void printAugmentingPath(struct AugmentingPath* currPath) {
    printf("Number of vertices in the path:- %d\n",currPath->numVertices);

    for(int i=0;i<currPath->numVertices;i++) {
        printf("%d ->",currPath->nodeSequence[i]);
    }
    printf("END\n");
}

void insertVertexInPath(struct AugmentingPath* augPath, int vertex) {
    if(augPath->numVertices == 0) {
        augPath->nodeSequence = (int *)malloc(sizeof(int));
        augPath->maxPathSize = 1;
    }

    if(augPath->numVertices == augPath->maxPathSize) {
        augPath->maxPathSize = augPath->maxPathSize * 2;
        augPath->nodeSequence = (int*)realloc(augPath->nodeSequence,sizeof(int)*augPath->maxPathSize);
    }

    augPath->nodeSequence[augPath->numVertices] = vertex;
    augPath->numVertices ++;
}

void reverseAugmentingPath(struct AugmentingPath* augPath) {

    if(augPath == NULL)
        return;

    if(augPath->numVertices == 0)
        return;
    
    int n = augPath->numVertices;
    for(int i=0;i<(n/2);i++) {
        int temp = augPath->nodeSequence[i];
        augPath->nodeSequence[i] = augPath->nodeSequence[n-i-1];
        augPath->nodeSequence[n-i-1] = temp;
    }
}

struct AugmentingPathList* createAugPathList() {
    struct AugmentingPathList* currPathList = (struct AugmentingPathList*)malloc(sizeof(struct AugmentingPathList));

    currPathList->numAugPaths = 0;

    currPathList->maxNumberOfPaths = 1;
    currPathList->pathList = (struct AugmentingPath*)malloc(sizeof(struct AugmentingPath));

    return currPathList;
}

void printAugPathList(struct AugmentingPathList* list) {
    
    printf("Current Path List can be shown as follows:-\n");

    for(int i=0;i<list->numAugPaths;i++) {
        printAugmentingPath(&list->pathList[i]);
        printf("\n");
    }
    printf("\n");
}

void insertPathInPathList(struct AugmentingPath* currPath, struct AugmentingPathList* augPathList) {
    
    if(currPath == NULL)
        return;
    
    if(augPathList == NULL) {
        augPathList = (struct AugmentingPathList*)malloc(sizeof(struct AugmentingPathList));
        augPathList->numAugPaths = 0;

        augPathList->maxNumberOfPaths = 1;
        augPathList->pathList = (struct AugmentingPath*)malloc(sizeof(struct AugmentingPath));
    }

    if(augPathList->numAugPaths == augPathList->maxNumberOfPaths) {
        augPathList->maxNumberOfPaths = augPathList->maxNumberOfPaths * 2;
        augPathList->pathList = (struct AugmentingPath*)realloc(augPathList->pathList,sizeof(struct AugmentingPath)*augPathList->maxNumberOfPaths);
    }
    
    augPathList->pathList[augPathList->numAugPaths].numVertices = currPath->numVertices;
    augPathList->pathList[augPathList->numAugPaths].nodeSequence = currPath->nodeSequence;
    augPathList->numAugPaths ++;
}

void freeAugPathList(struct AugmentingPathList* currPathList) {
    free(currPathList->pathList);
    free(currPathList);
}

void xorMatchingAndPathList(struct Matching* matching, struct AugmentingPathList* augPathList, int xVertices) {
    for(int i=0;i<augPathList->numAugPaths;i++) {

        struct AugmentingPath currPath = augPathList->pathList[i];

        // Go through the path, and remove all the matchings in the path
        for(int j=0;j<currPath.numVertices;j+=2) {

            int mappedEle = search(matching->yMappings,currPath.nodeSequence[j],matching->yVertices);
            if(mappedEle != -1 ) {
                updateMatching(matching,mappedEle,currPath.nodeSequence[j]);
                continue;
            }
        }
        for(int j=0;j<currPath.numVertices;j=j+2) {
            addEdgeToMatching(matching,currPath.nodeSequence[j+1],currPath.nodeSequence[j]);
        }

    }
}