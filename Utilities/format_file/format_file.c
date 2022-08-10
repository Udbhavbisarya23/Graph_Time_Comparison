#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

#include "format_file.h"
#include "../Graphs/undirected_graph.h"
#include "../data_structures/map.h"

void formatUndirectedWeighted(char* filename, char* newFilename) {
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Error in opening file while formatting\n");
        return;
    }

    FILE *fp2;
    fp2 = fopen(newFilename,"w");
    if(fp2 == NULL) {
        printf("Error in opening file while formatting\n");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read = -1;
    int first = 0;

    while ((read = getline(&line, &len, fp)) != -1) {

        char* temp = line;
        for(size_t i=0;i<len;i++) {
            if(line[i] == ' ') {
                temp[i] = '\t';
            } else {
                temp[i] = line[i];
            }
        }
        fputs(temp,fp2);
    }

    fclose(fp);
    fclose(fp2);
    if (line)
        free(line);

}

void sanitizeUndirectedWeighted(char* oldFilename, char* newFilename) {
    FILE* fp;
    fp = fopen(oldFilename,"r");
    if(fp == NULL) {
        printf("Error in opening file\n");
        return;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read = -1;
    int first = 0,second=0,third=0,fourth=0;
    int vertices;
    int count = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        
        char* num_vertices = strtok(line," ");
        vertices = atoi(num_vertices);
        
        break;
    }    
    // mp is a map to old file name
    struct HashItem mp[vertices];
    for(int i=0;i<vertices;i++) {
        mp[i].key = -1;
        mp[i].value = -1;
    }
    fclose(fp);

    fp = fopen(oldFilename,"r");
    FILE *fp2;
    fp2 = fopen(newFilename,"w");

    char s[250];  
    int val = 0;
    // ((ch = fgetc(fp)))!= -1 ;
    while (fgets(s,sizeof(s),fp)) {
        if(first == 0) {
            first = 1;
            fputs(s,fp2);
            continue;
        }
        int src=-1,dest=-1;
        float weight = -1.0;

        char* token;
        if(strcmp(oldFilename,"../../Graphs/twitter_combined.txt") == 0) {
            token = strtok(s," ");
        } else {
            token = strtok(s,"\t");
        }
        if(strlen(token) != 0) { 
            
            int temp = atoi(token);
            int num = search(mp,temp,vertices);
            if(num == -1) {
                insert(mp,temp,val,vertices);
                num = val;
                val += 1;
            } 
            src = num;
            
            token = strtok(NULL,"\t");
            temp = atoi(token);
            num = search(mp,temp,vertices);
            if(num == -1) {
                insert(mp,temp,val,vertices);
                num = val;
                val += 1;
            }
            dest = num;

            token = strtok(NULL," ");
            float temp2 = atof(token);
            weight = temp2;
        }

        char s1[50],s2[20],s3[50];
        sprintf(s1,"%d",src);
        sprintf(s2,"%d",dest);
        sprintf(s3,"%f",weight);
        strcat(s1,"\t");
        strcat(s2,"\t");
        strcat(s3,"\n");
        strcat(s1,s2);
        strcat(s1,s3);
        fputs(s1,fp2);

    }

    fclose(fp);
    fclose(fp2);
    if (line)
        free(line);
        
}

void sanitizeBipartite(char* oldFilename, char* newFilename) {
    FILE* fp;
    fp = fopen(oldFilename,"r");
    if(fp == NULL) {
        printf("Error in opening file\n");
        return;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read = -1;
    int first = 0,second=0,third=0,fourth=0;
    int xVertices, yVertices;
    int count = 0;

    while ((read = getline(&line, &len, fp)) != -1) {

        if(first == 0) {
            first = 1;
            continue;
        }
        
        char* num_vertices = strtok(line,"\t");
        num_vertices = strtok(NULL,"\t");

        num_vertices = strtok(NULL,"\t");
        xVertices = atoi(num_vertices);

        num_vertices = strtok(NULL," ");
        yVertices = atoi(num_vertices);

        break;
        
    }    
    // mp is a map to old file name
    struct HashItem xMap[xVertices];
    for(int i=0;i<xVertices;i++) {
        xMap[i].key = -1;
        xMap[i].value = -1;
    }

    struct HashItem yMap[yVertices];
    for(int i=0;i<yVertices;i++) {
        yMap[i].key = -1;
        yMap[i].value = -1;
    }
    fclose(fp);

    fp = fopen(oldFilename,"r");
    FILE *fp2;
    fp2 = fopen(newFilename,"w");

    char s[250];  
    int xVal = 0, yVal=0;
    first = 0;
    // ((ch = fgetc(fp)))!= -1 ;
    while (fgets(s,sizeof(s),fp)) {
        if(first == 0) {
            first = 1;
            fputs(s,fp2);
            continue;
        }

        if(second == 0) {
            second = 1;
            fputs(s,fp2);
            continue;
        }

        int src=-1,dest=-1;
        float weight = -1.0;

        char* token;
        if(strcmp(oldFilename,"../../Graphs/twitter_combined.txt") == 0) {
            token = strtok(s," ");
        } else {
            token = strtok(s,"\t");
        }
        if(strlen(token) != 0) { 
            
            int temp = atoi(token);
            int num = search(xMap,temp,xVertices);
            if(num == -1) {
                insert(xMap,temp,xVal,xVertices);
                num = xVal;
                xVal += 1;
            } 
            src = num;
            
            token = strtok(NULL," ");
            temp = atoi(token);
            int destNum = search(yMap,temp,yVertices);
            if(destNum == -1) {
                insert(yMap,temp,yVal+xVertices,yVertices);
                destNum = xVertices+yVal;
                yVal += 1;
            }
            dest = destNum;
        }

        char s1[50],s2[20];
        sprintf(s1,"%d",src);
        sprintf(s2,"%d",dest);
        strcat(s1,"\t");
        strcat(s2,"\n");
        strcat(s1,s2);
        fputs(s1,fp2);

    }

    fclose(fp);
    fclose(fp2);
    if (line)
        free(line);
        
}