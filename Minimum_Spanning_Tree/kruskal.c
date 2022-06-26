#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

#include "../Utilities/format_file/format_file.h"
#include "../Utilities/Graphs/undirected_graph.h"
#include "../Utilities/data_structures/disjoint_sets.h"

#define KRUSKAL_THRESHOLD 220

float Kruskal(struct Graph* graph,struct EdgeList* edgelist,struct DisjointSets* ds) {
    
    float weight = 0;

    sortSerialEdgeList(edgelist,0,edgelist->num_edges-1);

    for(int i=0;i<edgelist->num_edges;i++) {

        if(findParent(ds,edgelist->edges[i].node1) != findParent(ds,edgelist->edges[i].node2)) {
            
            unionByRank(ds,edgelist->edges[i].node1,edgelist->edges[i].node2);
            weight += edgelist->edges[i].weight;
        }
    }

    return weight;
}

struct EdgeListPartition* partition(struct EdgeList* edgelist,float pivot) {

    struct EdgeListPartition* newPartition = (struct EdgeListPartition*)malloc(sizeof(struct EdgeListPartition));
    
    newPartition->LessThanPivotPartition = (struct EdgeList*)malloc(sizeof(struct EdgeList));
    newPartition->MoreThanPivotPartition = (struct EdgeList*)malloc(sizeof(struct EdgeList));

    int i = -1;
    for(int j=0;j<edgelist->num_edges;j++) {
        if(edgelist->edges[j].weight < pivot) {
            i++;
            swapEdgeList(edgelist,i,j);
        }
    }

    swapEdgeList(edgelist,i+1,edgelist->num_edges-1);

    newPartition->LessThanPivotPartition->num_edges = i+1;
    newPartition->MoreThanPivotPartition->num_edges = edgelist->num_edges - i - 1;

    struct Edge* less_than_edges = (struct Edge*)malloc((i+1)*sizeof(struct Edge));
    for(int k=0;k<=i;k++) {
        less_than_edges[k].node1 = edgelist->edges[k].node1;
        less_than_edges[k].node2 = edgelist->edges[k].node2;
        less_than_edges[k].weight = edgelist->edges[k].weight;
    }
    newPartition->LessThanPivotPartition->edges = less_than_edges;

    struct Edge* more_than_edges = (struct Edge*)malloc((edgelist->num_edges-i-1)*sizeof(struct Edge));
    for(int k=i+1;k<edgelist->num_edges;k++) {
        more_than_edges[k-i-1].node1 = edgelist->edges[k].node1;
        more_than_edges[k-i-1].node2 = edgelist->edges[k].node2;
        more_than_edges[k-i-1].weight = edgelist->edges[k].weight;

    }
    newPartition->MoreThanPivotPartition->edges = more_than_edges;

    return newPartition;

}

struct EdgeList* Filter(struct EdgeList* edgelist,struct DisjointSets* ds) {
    struct EdgeList* ret = (struct EdgeList*)malloc(sizeof(struct EdgeList));
    struct Edge* edges = (struct Edge*)malloc((edgelist->num_edges)*sizeof(struct Edge));

    int curr_ind = 0;

    for(int i=0;i<edgelist->num_edges;i++) {
        if(findParent(ds,edgelist->edges[i].node1) != findParent(ds,edgelist->edges[i].node2)) {
            edges[curr_ind].node1 = edgelist->edges[i].node1;
            edges[curr_ind].node2 = edgelist->edges[i].node2;
            edges[curr_ind].weight = edgelist->edges[i].weight;
            curr_ind++;
        }
    }

    edges = realloc(edges,curr_ind*(sizeof(struct Edge)));
    ret->edges = edges;
    ret->num_edges = curr_ind;

    return ret;
}

float FilterKruskal(struct Graph* graph,struct EdgeList* edgelist,struct DisjointSets* ds) {    
    
    if(edgelist->num_edges <= KRUSKAL_THRESHOLD) {
        return Kruskal(graph,edgelist,ds);
    }
    
    //Choose random edge
    float pivot = edgelist->edges[rand() % (edgelist->num_edges)].weight;
    
    //Partition
    struct EdgeListPartition * partitions = partition(edgelist,pivot);

    // Filter Kruskal on the 2 halfs
    float a = FilterKruskal(graph,partitions->LessThanPivotPartition,ds);

    partitions->MoreThanPivotPartition = Filter(partitions->MoreThanPivotPartition,ds);
    float b = FilterKruskal(graph,partitions->MoreThanPivotPartition,ds);


    return a+b;


}

int main() {
    char* oldFilename = "../../Minimum_Spanning_Tree/Undirected_Weighted_Graphs/Wiki_Conflict/out.wikiconflict";
    char* formattedFilename = "../../Minimum_Spanning_Tree/Undirected_Weighted_Graphs/Wiki_Conflict/out.wikiconflict_formatted";
    char* newFilename = "../../Minimum_Spanning_Tree/Undirected_Weighted_Graphs/Wiki_Conflict/out.wikiconflict_sanitized";
    formatUndirectedWeighted(oldFilename, formattedFilename);
    sanitizeUndirectedWeighted(formattedFilename, newFilename);
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
    printf("%f\n",FilterKruskal(graph,sorted,ds));
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    printf("Sequential SCC Time: %lf\n", time_overhead);
}