#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

#include "../Utilities/format_file/format_file.h"
#include "../Utilities/Graphs/undirected_graph.h"
#include "../Utilities/data_structures/disjoint_sets.h"

#define KRUSKAL_THRESHOLD 5

int Kruskal(struct Graph* graph,struct EdgeList* sorted) {
    
    int weight = 0;
    struct DisjointSets* ds = newDisjointSets(graph->vertices);

    for(int i=0;i<sorted->num_edges;i++) {

        if(findParent(ds,sorted->edges[i].node1) != findParent(ds,sorted->edges[i].node2)) {
            
            // printf("\nWeight-%d \tNode1-%d \tNode2-%d\n",sorted->edges[i].weight,sorted->edges[i].node1,sorted->edges[i].node2);
            // printDisjointSets(ds);
            
            unionByRank(ds,sorted->edges[i].node1,sorted->edges[i].node2);
            weight += sorted->edges[i].weight;
        }
    }

    return weight;
}

int KruskalSupport(struct Graph* graph,struct EdgeList* sorted,struct DisjointSets* ds) {
    
    int weight = 0;

    for(int i=0;i<sorted->num_edges;i++) {

        if(findParent(ds,sorted->edges[i].node1) != findParent(ds,sorted->edges[i].node2)) {
            
            // printf("\nWeight-%d \tNode1-%d \tNode2-%d\n",sorted->edges[i].weight,sorted->edges[i].node1,sorted->edges[i].node2);
            // printDisjointSets(ds);
            
            unionByRank(ds,sorted->edges[i].node1,sorted->edges[i].node2);
            weight += sorted->edges[i].weight;
        }
    }

    return weight;
}

struct EdgeListPartition* partition(struct EdgeList* sorted,int pivot) {
    
    struct EdgeListPartition* newPartition = (struct EdgeListPartition*)malloc(sizeof(struct EdgeListPartition));
    
    newPartition->LessThanPivotPartition = (struct EdgeList*)malloc(sizeof(struct EdgeList));
    newPartition->MoreThanPivotPartition = (struct EdgeList*)malloc(sizeof(struct EdgeList));
    
    int less_than_ind=0,more_than_ind=0;
    
    struct Edge* less_than_edges = (struct Edge*)malloc(sizeof(struct Edge));
    struct Edge* more_than_edges = (struct Edge*)malloc(sizeof(struct Edge));
    
    for(int i=0;i<sorted->num_edges;i++) {
        int curr_weight = sorted->edges[i].weight;

        if(curr_weight <= pivot) {
            less_than_edges = realloc(less_than_edges,(less_than_ind+1)*sizeof(struct Edge));
            less_than_edges[less_than_ind].node1 = sorted->edges[i].node1;
            less_than_edges[less_than_ind].node2 = sorted->edges[i].node2;
            less_than_edges[less_than_ind].weight = sorted->edges[i].weight;
            less_than_ind ++;
        } else {
            more_than_edges = realloc(more_than_edges,(more_than_ind+1)*sizeof(struct Edge));
            more_than_edges[more_than_ind].node1 = sorted->edges[i].node1;
            more_than_edges[more_than_ind].node2 = sorted->edges[i].node2;
            more_than_edges[more_than_ind].weight = sorted->edges[i].weight;
            more_than_ind ++;

        }
    }
    
    newPartition->LessThanPivotPartition->edges = less_than_edges;
    newPartition->LessThanPivotPartition->num_edges = less_than_ind;

    newPartition->MoreThanPivotPartition->edges = more_than_edges;
    newPartition->MoreThanPivotPartition->num_edges = more_than_ind;

    return newPartition;

}

int FilterKruskal(struct Graph* graph,struct EdgeList* sorted,struct DisjointSets* ds) {    
    
    if(sorted->num_edges == 0) {
        return 0;
    }
    
    if(sorted->num_edges <= KRUSKAL_THRESHOLD) {
        return KruskalSupport(graph,sorted,ds);
    }
    
    //Choose random edge
    int pivot = sorted->edges[rand() % (sorted->num_edges + 1 - 0) + 0].weight;

    //Partition 
    struct EdgeListPartition * partitions = partition(sorted,pivot);

    printf("Pivot:- %d,Original edge list:- \n",pivot);
    printEdgeList(sorted);
    printf("\nNew edge lists:- \n");
    printEdgeList(partitions->LessThanPivotPartition);
    printf("\n");
    printEdgeList(partitions->MoreThanPivotPartition);

    // Filter Kruskal on the 2 halfs
    int a = FilterKruskal(graph,partitions->LessThanPivotPartition,ds);

    //paritions->MoreThanPivotPartition = filter(paritions->MoreThanPivotPartition);
    int b = FilterKruskal(graph,partitions->MoreThanPivotPartition,ds);

    
    printf("\nValue of a:- %d\n",a);
    printf("\nValue of b:- %d\n",b);

    return a+b;


}

int main() {
    char* oldFilename = "../../Minimum_Spanning_Tree/Undirected_Weighted_Graphs/sampleInput.txt";
    char* newFilename = "../../Minimum_Spanning_Tree/Undirected_Weighted_Graphs/sampleInput_sanitized.txt";
    formatUndirectedWeighted(oldFilename);
    sanitizeUndirectedWeighted(oldFilename, newFilename);
    struct Graph* graph = readUndirectedWeightedGraph(newFilename);

    struct EdgeList* sorted = sortUndirectedWeightedGraph(graph);

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
    printf("Sequential SCC Time: %lf\n", time_overhead);
}