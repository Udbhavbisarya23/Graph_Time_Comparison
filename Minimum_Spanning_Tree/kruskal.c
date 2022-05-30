#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

#include "../Utilities/format_file/format_file.h"
#include "../Utilities/Graphs/undirected_graph.h"

int main() {
    struct Graph* graph;
    char* oldFilename = "../../Minimum_Spanning_Tree/Undirected_Weighted_Graphs/sampleInput.txt";
    char* newFilename = "../../Minimum_Spanning_Tree/Undirected_Weighted_Graphs/sampleInput_sanitized.txt";
    formatUndirectedWeighted(oldFilename);
    sanitizeUndirectedWeighted(oldFilename, newFilename);
    readUndirectedWeightedGraph(graph,newFilename);

    struct timeval TimeValue_Start;
    struct timezone TimeZone_Start;
    struct timeval TimeValue_Final;
    struct timezone TimeZone_Final;
    long time_start, time_end;
    double time_overhead;

    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    //Kruskal(graph);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    printf("Sequential SCC Time: %lf\n", time_overhead);
}