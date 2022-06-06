#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<omp.h>

#include "../Utilities/format_file/format_file.h"
#include "../Utilities/Graphs/undirected_graph.h"
#include "../Utilities/data_structures/disjoint_sets.h"

#define KRUSKAL_THRESHOLD 5

int Kruskal(struct Graph* graph,struct EdgeList* edgelist,struct DisjointSets* ds) {
    
    int weight = 0;

    sortParallelEdgeList(edgelist,0,edgelist->num_edges-1);

    for(int i=0;i<edgelist->num_edges;i++) {
        if(findParent(ds,edgelist->edges[i].node1) != findParent(ds,edgelist->edges[i].node2)) {
            
            // printf("\nWeight-%d \tNode1-%d \tNode2-%d\n",sorted->edges[i].weight,sorted->edges[i].node1,sorted->edges[i].node2);
            // printDisjointSets(ds);
                 
            unionByRank(ds,edgelist->edges[i].node1,edgelist->edges[i].node2);
            weight += edgelist->edges[i].weight;
        }
    }

    return weight;
}

struct EdgeListPartition* partition(struct EdgeList* edgelist,int pivot) {
    
    struct EdgeListPartition* newPartition = (struct EdgeListPartition*)malloc(sizeof(struct EdgeListPartition));
    
    newPartition->LessThanPivotPartition = (struct EdgeList*)malloc(sizeof(struct EdgeList));
    newPartition->MoreThanPivotPartition = (struct EdgeList*)malloc(sizeof(struct EdgeList));
    
    int less_than_ind=0,more_than_ind=0;
    
    struct Edge* less_than_edges = (struct Edge*)malloc(sizeof(struct Edge));
    struct Edge* more_than_edges = (struct Edge*)malloc(sizeof(struct Edge));
    
    for(int i=0;i<edgelist->num_edges;i++) {
        int curr_weight = edgelist->edges[i].weight;

        if(curr_weight <= pivot) {
            less_than_edges = realloc(less_than_edges,(less_than_ind+1)*sizeof(struct Edge));
            less_than_edges[less_than_ind].node1 = edgelist->edges[i].node1;
            less_than_edges[less_than_ind].node2 = edgelist->edges[i].node2;
            less_than_edges[less_than_ind].weight = edgelist->edges[i].weight;
            less_than_ind ++;
        } else {
            more_than_edges = realloc(more_than_edges,(more_than_ind+1)*sizeof(struct Edge));
            more_than_edges[more_than_ind].node1 = edgelist->edges[i].node1;
            more_than_edges[more_than_ind].node2 = edgelist->edges[i].node2;
            more_than_edges[more_than_ind].weight = edgelist->edges[i].weight;
            more_than_ind ++;

        }
    }
    
    newPartition->LessThanPivotPartition->edges = less_than_edges;
    newPartition->LessThanPivotPartition->num_edges = less_than_ind;

    newPartition->MoreThanPivotPartition->edges = more_than_edges;
    newPartition->MoreThanPivotPartition->num_edges = more_than_ind;

    return newPartition;

}

struct EdgeList* Filter(struct EdgeList* edgelist,struct DisjointSets* ds) {
    struct EdgeList* ret = (struct EdgeList*)malloc(sizeof(struct EdgeList));
    struct Edge* edges = (struct Edge*)malloc(edgelist->num_edges*sizeof(struct Edge));

    int curr_ind = 0;

    #pragma omp parallel default(none) shared(curr_ind,edges,ds,edgelist)
    {        
        #pragma omp for
        for(int i=0;i<edgelist->num_edges;i++) {

            int a = findParent(ds,edgelist->edges[i].node1);
            int b = findParent(ds,edgelist->edges[i].node2);

            if(a != b) {

                #pragma omp critical
                {
                    edges[curr_ind].node1 = edgelist->edges[i].node1;
                    edges[curr_ind].node2 = edgelist->edges[i].node2;
                    edges[curr_ind].weight = edgelist->edges[i].weight;
                    curr_ind++;
                }
            }


        }
    }

    edges = realloc(edges,curr_ind*(sizeof(struct Edge)));

    ret->edges = edges;
    ret->num_edges = curr_ind;

    return ret;
}

int FilterKruskal(struct Graph* graph,struct EdgeList* edgelist,struct DisjointSets* ds) {    
    
    if(edgelist->num_edges == 0) {
        return 0;
    }
    
    if(edgelist->num_edges <= KRUSKAL_THRESHOLD) {
        return Kruskal(graph,edgelist,ds);
    }
    
    //Choose random edge
    int pivot = edgelist->edges[rand() % (edgelist->num_edges + 1 - 0) + 0].weight;

    //Partition 
    struct EdgeListPartition * partitions = partition(edgelist,pivot);

    // Filter Kruskal on the 2 halfs
    int a = FilterKruskal(graph,partitions->LessThanPivotPartition,ds);

    partitions->MoreThanPivotPartition = Filter(partitions->MoreThanPivotPartition,ds);
    int b = FilterKruskal(graph,partitions->MoreThanPivotPartition,ds);


    return a+b;


}

int main() {
    char* oldFilename = "../../Minimum_Spanning_Tree/Undirected_Weighted_Graphs/sampleInput.txt";
    char* newFilename = "../../Minimum_Spanning_Tree/Undirected_Weighted_Graphs/sampleInput_sanitized.txt";
    formatUndirectedWeighted(oldFilename);
    sanitizeUndirectedWeighted(oldFilename, newFilename);
    struct Graph* graph = readUndirectedWeightedGraph(newFilename);

    struct EdgeList* sorted = sortUndirectedWeightedGraph(graph);
    srand(time(0));

    struct timeval TimeValue_Start;
    struct timezone TimeZone_Start;
    struct timeval TimeValue_Final;
    struct timezone TimeZone_Final;
    long time_start, time_end;
    double time_overhead;

    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    struct DisjointSets* ds = newDisjointSets(graph->vertices);
    printf("%d\n",FilterKruskal(graph,sorted,ds));
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    printf("Parallel SCC Time: %lf\n", time_overhead);
    #pragma omp parallel
    {
        printf("Thread ID:- %d\n",omp_get_thread_num());
    }
}