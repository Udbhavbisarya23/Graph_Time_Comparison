#ifndef format_file  
#define format_file

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

#include "../Graphs/undirected_graph.h"
#include "../data_structures/map.h"

void formatUndirectedWeighted(char* filename);
void sanitizeUndirectedWeighted(char* oldFilename, char* newFilename);

#endif