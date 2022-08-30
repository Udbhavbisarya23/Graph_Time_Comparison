#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>
#include<dirent.h>
#include<errno.h>
#include<unistd.h>

#include "generate_graph.h"
#include "./format_file.h"

void addRandomWeights(FILE* srcFile, char* buffer, FILE* targetFile) {

    int count = 0;
    srand(time(0));

    while (fgets(buffer, BUFSIZ, srcFile) != NULL)
    {
        /* Use fprintf or fwrite to write some stuff into common_file*/
        printf("%s",buffer);
        if(count < 4) {
            fprintf(targetFile,buffer);
            count++;
            continue;
        }

        char* token;
        token = strtok(buffer,"\t");
        long long src = atoi(token);

        token = strtok(NULL,"\n");
        long long dest = atoi(token);

        char* line[BUFSIZ];
        int currRand = (rand() % 200) + 1;
        char number[BUFSIZ],source[BUFSIZ],destination[BUFSIZ];
        
        sprintf(number, "%d", currRand); 
        sprintf(source, "%d", src);
        sprintf(destination, "%d", dest);
        
        strcpy(line,source);
        strcat(line,"\t");
        strcat(line,destination);
        strcat(line,"\t");
        strcat(line,number);
        strcat(line,"\n");

        printf("%s\n",line);
        fprintf(targetFile,line);
    }

    return;
}


void generateweightedGraphs(DIR* currDir, DIR* targetDir, char* actualCurrPath, char* actualTargetPath) {
    struct dirent* in_file;
    int rc;
    char buffer[BUFSIZ];
    FILE *entry_file;

    if(currDir==NULL) {
        printf("Error! Unable to read directory");
        exit(1);
    }
    
    while ((in_file = readdir(currDir)) != NULL) {
        if (!strcmp (in_file->d_name, "."))
        continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;
        
        /* Open directory entry file for common operation */
        /* TODO : change permissions to meet your need! */
        char fileName[PATH_MAX+1]; 
        strcpy(fileName,actualCurrPath);
        strcat(fileName,"/");
        strcat(fileName,in_file->d_name);

        entry_file = fopen(fileName, "r");
        printf("%s\n", fileName);
        
        if (entry_file == NULL)
        {
            fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
            return;
        }

        /* Doing some struf with entry_file : */
        /* For example use fgets */
        // while (fgets(buffer, BUFSIZ, entry_file) != NULL)
        // {
        //     /* Use fprintf or fwrite to write some stuff into common_file*/
        // }

        char targetFileName[PATH_MAX+1];
        strcpy(targetFileName,actualTargetPath);
        strcat(targetFileName,"/");
        strcat(targetFileName,in_file->d_name);

        if(strcmp(in_file->d_name,"twitter_combined.txt")) {

            FILE* target_file = fopen(targetFileName, "w");
            addRandomWeights(entry_file,buffer,target_file);
            fclose(target_file);
        
        }

        /* When you finish with the file, close it */
        fclose(entry_file);
    }
    
    
    closedir(currDir);
    closedir(targetDir);
}

void generateBipartiteGraphs(DIR* currDir, DIR* targetDir, char* actualCurrPath, char* actualTargetPath) {
    struct dirent* in_file;
    int rc;
    char buffer[BUFSIZ];
    FILE *entry_file;

    if(currDir==NULL) {
        printf("Error! Unable to read directory");
        exit(1);
    }
    
    while ((in_file = readdir(currDir)) != NULL) {
        if (!strcmp (in_file->d_name, "."))
        continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;
        
        /* Open directory entry file for common operation */
        /* TODO : change permissions to meet your need! */
        char fileName[PATH_MAX+1]; 
        strcpy(fileName,actualCurrPath);
        strcat(fileName,"/");
        strcat(fileName,in_file->d_name);

        entry_file = fopen(fileName, "r");
        printf("%s\n", fileName);
        
        if (entry_file == NULL)
        {
            fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
            return;
        }
        fclose(entry_file);

        /* Doing some struf with entry_file : */
        /* For example use fgets */
        // while (fgets(buffer, BUFSIZ, entry_file) != NULL)
        // {
        //     /* Use fprintf or fwrite to write some stuff into common_file*/
        // }


        char intermediateFileName[PATH_MAX+1];
        strcpy(intermediateFileName,actualTargetPath);
        strcat(intermediateFileName,"/");
        strcat(intermediateFileName,in_file->d_name);
        strcat(intermediateFileName,"_temp");

        char targetFileName[PATH_MAX+1];
        strcpy(targetFileName,actualTargetPath);
        strcat(targetFileName,"/");
        strcat(targetFileName,in_file->d_name);

        if(access(targetFileName,F_OK) == 0) {
            continue;
        }

        if(strcmp(in_file->d_name,"twitter_combined.txt")) {
            formatUndirectedWeighted(fileName,intermediateFileName);
            sanitizeBipartite(intermediateFileName,targetFileName);   
            remove(intermediateFileName);     
        }

        /* When you finish with the file, close it */
    }
    
    
    closedir(currDir);
    closedir(targetDir);
}
