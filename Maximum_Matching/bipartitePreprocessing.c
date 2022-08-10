#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<dirent.h>

#include "../Utilities/format_file/generate_graph.h"

int main() {
    
    char actualCurrPath [PATH_MAX+1];
    char* relativeCurrPath = "../../Maximum_Matching/Bipartite_Graphs";
    char *ptr;
    ptr = realpath(relativeCurrPath, actualCurrPath);

    DIR* currDir;
    currDir = opendir(relativeCurrPath);
    if(currDir == NULL) {
        printf("Error while printing the currDir\n");
    }

    // Preprocessing of target directoty
    char actualTargetPath [PATH_MAX+1];
    char* relativeTargetPath = "../../Maximum_Matching/Bipartite_Graphs_sanitized";
    ptr = realpath(relativeTargetPath, actualTargetPath);

    DIR* targetDir;
    targetDir = opendir(relativeTargetPath);
    if(targetDir == NULL) {
        printf("Error while printing while printing the targetDir\n");
    }

    // char targetFileName[PATH_MAX+1];
    // strcpy(targetFileName,actualTargetPath);
    // strcat(targetFileName,"/");
    // strcat(targetFileName,"temp.txt");
    // FILE* target_file = fopen(targetFileName, "w");

    generateBipartiteGraphs(currDir,targetDir,actualCurrPath,actualTargetPath);
}