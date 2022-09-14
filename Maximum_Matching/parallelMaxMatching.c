#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<omp.h>

#include "../Utilities/format_file/format_file.h"
#include "../Utilities/Graphs/bipartite_graph.h"

void matchAndUpdate(struct BipartiteGraph* graph, struct Matching* matching, 
                    int u, int* visited, 
                    int* degree) {
    
    if(__sync_fetch_and_add(&visited[u],1) == 0) {

        struct AdjacencyListNodeBipartite* v = graph->x[u].head;
        while(v) {            
            // If v is unvisited
            if(__sync_fetch_and_add(&visited[v->curr],1) == 0) {
                //Updating matching 
                #pragma omp critical (matchingUpdate)
                {
                    addEdgeToMatching(matching,u,v->curr);   
                }     
                
                // Go through all w (belonging to set X) adjacent to v(belonging to set Y)
                int yInd = v->curr - graph->xVertices;
                struct AdjacencyListNodeBipartite* w = graph->y[yInd].head;
                while(w) {
                    if(__sync_fetch_and_add(&degree[w->curr],-1) == 2) {
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
    #pragma omp parallel default(none) shared(totalVertices,visited)
    {
        #pragma omp for
        for(int i=0;i<totalVertices;i++) {
            visited[i] = 0;
        }
    }

    printf("In Karp Sipser\n");

    // Add vertices of degree 1 to Q1 and remaining to Q*
    int* degreeOneNodes = (int*)malloc(sizeof(int)*graph->xVertices);
    int* notDegreeOneNodes = (int*)malloc(sizeof(int)*graph->xVertices);
    int oneCount = 0, notOneCount = 0;

    for(int i=0;i<graph->xVertices;i++) {
        degree[i] = calculateDegree(graph,i);
    }

    for(int i=0;i<graph->xVertices;i++) {

        if(degree[i] == 1) {
            degreeOneNodes[oneCount] = i;
            oneCount ++;
        } else {
            notDegreeOneNodes[notOneCount] = i;
            notOneCount ++;
        }
    }

    printf("Nodes of degree 1:-%d\nNodes of other degrees:-%d\n",oneCount,notOneCount);
    printf("Before match and update\n");

    // Match degree=1 vertices, update degrees and look for new degree = 1 vertices
    #pragma omp parallel default(none) shared(graph,oneCount, degreeOneNodes, visited, degree, matching)
    {
        #pragma omp for
        for(int i=0;i<oneCount;i++) {
            matchAndUpdate(graph,matching,degreeOneNodes[i],visited,degree);
        }
    }

    // Match higher degree vertices, update degrees and look for new degree = 1 vertices
    #pragma omp parallel default(none) shared(graph,notOneCount, notDegreeOneNodes, visited, degree, matching)
    {
        #pragma omp for
        for(int i=0;i<notOneCount;i++) {
            matchAndUpdate(graph,matching,notDegreeOneNodes[i],visited,degree);
        }
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

    while(1) {

        struct Layer* layerK1 = createLayer(); // Y

        struct Layer* layerK2 = createLayer(); // X

        struct UnweightedEdgeList* edgeList = createUnweightedEdgeList(0);

        #pragma omp parallel default(none) shared(currMatching, graph, visited, layerK1, layerK2, layerList, edgeList, currLayerKSize, k)
        {

            struct Layer* localK1 = createLayer();
            struct Layer* localK2 = createLayer();
            struct UnweightedEdgeList* localEdgeList = createUnweightedEdgeList(0);
        
            int numThreads = omp_get_num_threads();
            int tid = omp_get_thread_num();

            int start = (currLayerKSize * tid)/numThreads;
            int end;
            if(tid == numThreads - 1) {
                end = currLayerKSize;
            } else {
                end = (currLayerKSize * (tid+1))/numThreads;
            }

            for(int i=start;i<end;i++) {
                int currNode = layerList[k].vertices[i];
                struct AdjacencyListNodeBipartite* v = graph->x[currNode].head;

                while(v != NULL) {

                    if(__sync_fetch_and_add(&visited[v->curr],1) == 0) {
                        insertVertexInLayer(localK1,v->curr);
                        insertEdgeInEdgeList(localEdgeList,currNode,v->curr);

                        int vertexMatch = search(currMatching->yMappings,v->curr,currMatching->yVertices);
                        if(vertexMatch != -1) {
                            insertEdgeInEdgeList(localEdgeList,v->curr,vertexMatch);
                            insertVertexInLayer(localK2,vertexMatch);
                        }
                    }

                    v = v->next;
                }
            }

            #pragma omp critical (layerUpdation)
            {
                for(int j=0;j<localK1->numVertices;j++) {
                    insertVertexInLayer(layerK1,localK1->vertices[j]);
                }

                for(int j=0;j<localK2->numVertices;j++) {
                    insertVertexInLayer(layerK2,localK2->vertices[j]);
                }

                for(int j=0;j<localEdgeList->numEdges;j++) {
                    insertEdgeInEdgeList(edgeList,localEdgeList->edges[j].source,localEdgeList->edges[j].dest);
                }
            }
            freeLayer(localK1);
            freeLayer(localK2);
            freeUnweightedEdgeList(localEdgeList);
        }
        
        if(layerK1->numVertices == 0 || checkUnmatchedVertexInLayer(layerK1,currMatching)) {
            k ++;
            layerList = (struct Layer*)realloc(layerList,sizeof(struct Layer)*(k+1));
            layerList[k] = *layerK1;
            struct LayeredGraph* layeredGraph = createLayeredGraph(k,layerList,edgeList);
            return createLayeredGraphAndLayer(layerK1,layeredGraph);
        } else {
            k = k+2;
            layerList = (struct Layer*)realloc(layerList,sizeof(struct Layer)*(k+1));
            layerList[k-1] = *layerK1;
            layerList[k] = *layerK2;
            currLayerKSize = layerK2->numVertices;
        }   
    }

}

struct AugmentingPath* dfs_tfs(struct BipartiteGraph* graph, int curr, int* visited, struct Matching* currMatching) {
    struct AdjacencyListNodeBipartite* adjU = graph->y[curr - graph->xVertices].head;

    while(adjU) {
        
        if( __sync_fetch_and_add(&visited[adjU->curr],1) == 0) {

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

    struct Matching* currMatching = karpSipser(graph);
    printf("Number of edges after Karp Sipser:- %d\n\n",currMatching->currSize);
    
    //struct Matching* currMatching = createMatching();
    struct AugmentingPathList* augPathList = createAugPathList();
    struct LayeredGraphAndLayer* finLayerGraph;

    while(1) {

        int totalVertices = graph->xVertices + graph->yVertices;
        int visited[totalVertices];
        for(int i=0;i<totalVertices;i++) {
            visited[i] = 0;
        }

        int currSize = 0;
        augPathList->numAugPaths = 0;
        augPathList->pathList = realloc(augPathList->pathList,sizeof(struct AugmentingPath));
        augPathList->maxNumberOfPaths = 1;


        printf("Before LayeredGraphTS\n");
        finLayerGraph = layeredGraphTS(graph,currMatching,visited);
        printf("After LayeredGraphTS\n");


        for(int i=0;i<totalVertices;i++) {
            visited[i] = 0;
        }

        //Retrieving the unmatchedVertices in the last layer
        int *unmatchedVertices = malloc(sizeof(int) * finLayerGraph->layerk1->numVertices);
        int numElements = 0;

        for(int i=0;i<finLayerGraph->layerk1->numVertices;i++) {
            int currVertex = finLayerGraph->layerk1->vertices[i];

            if(search(currMatching->yMappings,currVertex,currMatching->yVertices) == -1) {
                unmatchedVertices[numElements] = currVertex;
                numElements ++;
            }
        }
        unmatchedVertices = realloc(unmatchedVertices,sizeof(int) * numElements);

        #pragma omp parallel default(none) shared(unmatchedVertices, numElements, currMatching, graph, visited, augPathList)
        {
            #pragma omp for
            for(int i=0;i<numElements;i++) {
                int currVertex = unmatchedVertices[i];
                struct AugmentingPath* currPath = dfs_tfs(graph,currVertex,visited,currMatching);
                reverseAugmentingPath(currPath);
                insertPathInPathList(currPath,augPathList);                  
            }
        }
        xorMatchingAndPathList(currMatching,augPathList,graph->xVertices);
        // printMatching(currMatching);
        // printAugPathList(augPathList);

        free(unmatchedVertices);
        printf("Number of aug path list vertices:- %d\n",augPathList->numAugPaths);
        if(augPathList->numAugPaths == 0 || augPathList == NULL) {
            printf("List is NULL\n");
            break;
        }

    }

    int numberOfEdges = currMatching->currSize;
    freeMatching(currMatching);
    freeAugPathList(augPathList);
    freeLayeredGraphAndLayer(finLayerGraph);
    return numberOfEdges;
}

int main() {
    char* newFilename = "../../Maximum_Matching/Bipartite_Graphs_sanitized/out.bag-kos";
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
    printf("Parallel SCC Time: %lf\n", time_overhead);

    freeBipartiteGraph(graph);

    return 0;
}
