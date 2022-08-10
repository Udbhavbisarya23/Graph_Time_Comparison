#ifndef generate_graph  
#define generate_graph

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<dirent.h>
#include<time.h>


void addRandomWeights(FILE* srcFile, char* buffer, FILE* targetFile);
void generateweightedGraphs(DIR* currDir, DIR* targetDir, char* actualCurrPath, char* actualTargetPath);
void generateBipartiteGraphs(DIR* currDir, DIR* targetDir, char* actualCurrPath, char* actualTargetPath);

#endif