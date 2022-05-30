#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

#include "format_file.h"
#include "../Graphs/undirected_graph.h"
#include "../data_structures/map.h"

void formatUndirectedWeighted(char* filename) {
    FILE *fp;
    fp = fopen(filename, "r+w");
    if(fp == NULL) {
        printf("Error in opening file while formatting\n");
        return;
    }

    printf("Inside format function\n");

    char *line = NULL;
    size_t len = 0;
    ssize_t read = -1;
    int first = 0;

    while ((read = getline(&line, &len, fp)) != -1) {

        char ch = fgetc(fp);
        while(ch != '\n' && ch != EOF){
            if(ch==' ')
            {
                printf("Char is :- %c\n", ch); 
                fseek(fp, -1, SEEK_CUR);
                fputc('\t', fp);
                fseek(fp, 0, SEEK_CUR);	
            }
            ch = fgetc(fp);
        }
        if(ch == EOF)
            break;

    }

    fclose(fp);
    if (line)
        free(line);
    
    printf("Format function complete\n");

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
        printf("%s\n",num_vertices);
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
        int src=-1,dest=-1,weight=-1;

        char* token;
        if(strcmp(oldFilename,"../../Graphs/twitter_combined.txt") == 0) {
            token = strtok(s," ");
        } else {
            token = strtok(s,"\t");
        }
        if(strlen(token) != 0) { 
            
            int temp = atoi(token);
            int num = search(mp,temp,vertices);
            printf("%d\n",num);
            if(num == -1) {
                insert(mp,temp,val,vertices);
                num = val;
                printf("New key val pairs are:- %d, %d\n",temp,val);
                val += 1;
            } 
            src = num;
            
            token = strtok(NULL,"\t");
            temp = atoi(token);
            num = search(mp,temp,vertices);
            printf("%d\n",num);
            if(num == -1) {
                insert(mp,temp,val,vertices);
                num = val;
                val += 1;
                printf("New key val pairs are:- %d, %d\n",temp,val);
            }
            dest = num;

            token = strtok(NULL," ");
            temp = atoi(token);
            weight = temp;
        }

        char s1[50],s2[20],s3[50];
        sprintf(s1,"%d",src);
        sprintf(s2,"%d",dest);
        sprintf(s3,"%d",weight);
        strcat(s1,"\t");
        strcat(s2,"\t");
        strcat(s3,"\n");
        strcat(s1,s2);
        strcat(s1,s3);
        fputs(s1,fp2);

    }

    for(int i=0;i<vertices;i++) {
        printf("New key val pairs are:- %d, %d\n",mp[i].key,mp[i].value);
    }


    fclose(fp);
    if (line)
        free(line);
        
}