#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

#include "../Utilities/format_file/format_file.h"
#include "../Utilities/Graphs/bipartite_graph.h"

void matchAndUpdate(struct BipartiteGraph* graph, struct Matching* matching, 
                    int u, int* visited, 
                    int* degree) {
    
    if(visited[u] == 0) {
        visited[u] = 1;

        struct AdjacencyListNodeBipartite* v = graph->x[u].head;
        while(v) {            
            // If v is unvisited
            if(visited[v->curr] == 0) {
                //Update the visited array
                visited[v->curr] = 1;

                //Updating matching 
                addEdgeToMatching(matching,u,v->curr);        
                
                // Go through all w (belonging to set X) adjacent to v(belonging to set Y)
                int yInd = v->curr - graph->xVertices;
                struct AdjacencyListNodeBipartite* w = graph->y[yInd].head;
                while(w) {
                    degree[w->curr] --;
                    if(degree[w->curr] == 1) {
                        matchAndUpdate(graph,matching,w->curr,visited,degree);
                    }
                    w = w->next;
                }
                // Since {u,v} is matched the search is stopped and we break out of the function
                break;

            }
            
            v = v->next;
        }
    }
}


struct Matching* karpSipser(struct BipartiteGraph* graph) {
    struct Matching* matching = createMatching();
    
    int totalVertices = graph->xVertices + graph->yVertices;

    int* visited = (int*)malloc(sizeof(int)*totalVertices);
    int* degree = (int*)malloc(sizeof(int)*(graph->xVertices));

    // Setting visited of all nodes to 0
    for(int i=0;i<totalVertices;i++) {
        visited[i] = 0;
    }

    printf("In Karp Sipser\n");

    // Add vertices of degree 1 to Q1 and remaining to Q*
    int* degreeOneNodes = (int*)malloc(sizeof(int)*graph->xVertices);
    int* notDegreeOneNodes = (int*)malloc(sizeof(int)*graph->xVertices);
    int oneCount = 0, notOneCount = 0;
    for(int i=0;i<graph->xVertices;i++) {
        degree[i] = calculateDegree(graph,i);

        if(degree[i] == 1) {
            degreeOneNodes[oneCount] = i;
            oneCount ++;
        } else {
            notDegreeOneNodes[notOneCount] = i;
            notOneCount ++;
        }
    }

    printf("Before match and update\n");

    // Match degree=1 vertices, update degrees and look for new degree = 1 vertices
    for(int i=0;i<oneCount;i++) {
        matchAndUpdate(graph,matching,degreeOneNodes[i],visited,degree);
    }

    // Match higher degree vertices, update degrees and look for new degree = 1 vertices
    for(int i=0;i<notOneCount;i++) {
        matchAndUpdate(graph,matching,notDegreeOneNodes[i],visited,degree);
    }

    free(degreeOneNodes);
    free(notDegreeOneNodes);
    free(degree);
    free(visited);

    return matching;
}

struct LayeredGraphAndLayer* layeredGraphTS(struct BipartiteGraph* graph, struct Matching* currMatching, int* visited) {
    struct AugmentingPath* currPathList = NULL;

    struct Layer* temp = createLayer();
    struct Layer* layerList = temp;

    for(int i=0;i<graph->xVertices;i++) {
        
        if(search(currMatching->xMappings,i,currMatching->xVertices) == -1) {
            insertVertexInLayer(layerList,i);
        }
    }
    
    int k = 0;
    int currLayerKSize = layerList[0].numVertices;

    printLayer(layerList);

    while(1) {

        printf("Before create Layer\n");

        struct Layer* layerK1 = createLayer(); // Y

        struct Layer* layerK2 = createLayer(); // X

        printf("After create Layer\n");

        struct UnweightedEdgeList* localEdgeList = createUnweightedEdgeList(0);

        for(int i=0;i<currLayerKSize;i++) {
            int currNode = layerList[k].vertices[i];
            struct AdjacencyListNodeBipartite* v = graph->x[currNode].head;

            if(visited[v->curr] == 0) {
                insertVertexInLayer(layerK1,v->curr);
                insertEdgeInEdgeList(localEdgeList,currNode,v->curr);

                int vertexMatch = search(currMatching->yMappings,v->curr,currMatching->yVertices);
                if(vertexMatch != -1) {
                    insertEdgeInEdgeList(localEdgeList,vertexMatch,v->curr);
                    insertVertexInLayer(layerK2,vertexMatch);
                }
            }
        }

        if(layerK1->vertices == NULL || checkUnmatchedVertexInLayer(layerK1,currMatching)) {
            struct LayeredGraph* layeredGraph = createLayeredGraph(k+1,layerList,localEdgeList);
            k ++;
            layerList = (struct Layer*)realloc(layerList,sizeof(struct Layer)*k);
            layerList[k-1] = *layerK1;
            return createLayeredGraphAndLayer(layerK1,layeredGraph);
        } else {
            k = k+2;
            layerList = (struct Layer*)realloc(layerList,sizeof(struct Layer)*(k+1));
            layerList[k-2] = *layerK1;
            layerList[k-1] = *layerK2;
        }
        
    }

}

struct AugmentingPath* dfs_tfs(struct BipartiteGraph* graph, int curr, int* visited, struct Matching* currMatching) {
    struct AdjacencyListNodeBipartite* adjU = graph->y[curr - graph->xVertices].head;

    while(adjU) {
        
        if(visited[adjU->curr] == 0) {
            visited[adjU->curr] = 1;

            int matchedVertex = search(currMatching->xMappings,adjU->curr,currMatching->xVertices);
            if(matchedVertex == -1) {
                struct AugmentingPath* path = createAugmentingPath();
                insertVertexInPath(path,adjU->curr);
                insertVertexInPath(path,curr);
                return path;
            } else {
                struct AugmentingPath* path = dfs_tfs(graph,matchedVertex,visited,currMatching);
                if(path != NULL) {
                    insertVertexInPath(path,adjU->curr);
                    insertVertexInPath(path,curr);
                    return path;
                }
            }
        }

        adjU = adjU->next;        
    }

    return NULL;
}

int hopcraftKarp(struct BipartiteGraph* graph) {

    // struct Matching* currMatching = karpSipser(graph);
    struct Matching* currMatching = createMatching();

    printf("Initial matching generated\n");

    while(1) {

        int totalVertices = graph->xVertices + graph->yVertices;
        int visited[totalVertices];
        for(int i=0;i<totalVertices;i++) {
            visited[i] = 0;
        }

        struct AugmentingPathList* augPathList = NULL;
        int currSize = 0;

        printf("Before layeredGraphTS\n");
        struct LayeredGraphAndLayer* finLayerGraph = layeredGraphTS(graph,currMatching,visited);
        printf("After layeredGraphTS\n");

        for(int i=0;i<totalVertices;i++) {
            visited[i] = 0;
        }

        for(int i=0;i<finLayerGraph->layerk1->numVertices;i++) {
            int currVertex = finLayerGraph->layerk1->vertices[i];

            if(search(currMatching->yMappings,currVertex,currMatching->yVertices) == -1) {
                struct AugmentingPath* currPath = dfs_tfs(graph,currVertex,visited,currMatching);
                reverseAugmentingPath(currPath);
                insertPathInPathList(currPath,augPathList);
            }
        }

        xorMatchingAndPathList(currMatching,augPathList);

        if(augPathList == NULL) {
            printf("List is NULL\n");
            break;
        }

    }
    return currMatching->currSize;
}

int main() {
    char* newFilename = "../../Maximum_Matching/Bipartite_Graphs_sanitized/out.brunson_revolution_revolution";
    struct BipartiteGraph* graph = readBipartiteGraph(newFilename);
    //printBipartiteGraph(graph);
    //struct EdgeList* sorted = sortUndirectedWeightedGraph(graph);
    printf("Read completed\n");
    srand(time(0));

    struct timeval TimeValue_Start;
    struct timezone TimeZone_Start;
    struct timeval TimeValue_Final;
    struct timezone TimeZone_Final;
    long time_start, time_end;
    double time_overhead;

    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    //struct DisjointSets* ds = newDisjointSets(graph->vertices);
    printf("Edges in MaxMatching:- %d\n",hopcraftKarp(graph));
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    printf("Sequential SCC Time: %lf\n", time_overhead);
}
