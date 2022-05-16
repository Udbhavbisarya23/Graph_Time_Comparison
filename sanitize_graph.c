#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

struct HashItem {
    int key;
    int value;
};

int search(struct HashItem* mp,int key,int vertices) {
    
    for(int i=0;i<vertices;i++) {
        if(mp[i].key == key) {
            return mp[i].value;
        }
    }
    
    return -1;
}

void insert(struct HashItem* mp,int key,int value,int vertices) {
    for(int i=0;i<vertices;i++) {
        if(mp[i].key == -1) {
            mp[i].key = key;
            mp[i].value = value;
            return;
        }
    }
    return;
}

int main() {
    FILE* fp;
    char filename[] = "Graphs/as-caida20071105.txt";
    fp = fopen(filename,"r");
    if(fp == NULL) {
        printf("Error in opening file\n");
        return 0;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read = -1;
    int first = 0,second=0,third=0,fourth=0;
    int vertices;
    int count = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        if(first == 0) {
            first = 1;
            continue;
        }

        if(second == 0) {
            second = 1;
            continue;
        }

        if(third == 0) {
            char* garb_string = strtok(line," ");
            garb_string = strtok(NULL," ");
            char* num_vertices = strtok(NULL," ");
            printf("%s\n",num_vertices);
            vertices = atoi(num_vertices);

            first=0;
            second=0;
            break;
            
        }

    }
    
    // mp is a map to old file name
    struct HashItem mp[vertices];
    for(int i=0;i<vertices;i++) {
        mp[i].key = -1;
        mp[i].value = -1;
    }
    fclose(fp);
    fp = fopen(filename,"r");
    FILE *fp2;
    fp2 = fopen("Graphs/as-caida20071105_sanitized.txt","w");

    char s[250];  
    int val = 0;
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
            continue;;
        }

        if(third == 0) {
            third = 1;
            fputs(s,fp2);
            continue;
            
        }

        if(fourth == 0) {
            fourth = 1;
            fputs(s,fp2);
            continue;
        }
        int src=-1,dest=-1;

        char* token;
        if(strcmp(filename,"Graphs/twitter_combined.txt") == 0) {
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
            token = strtok(NULL," ");
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
        }

        char s1[50],s2[20];
        sprintf(s1,"%d",src);
        sprintf(s2,"%d",dest);
        strcat(s1,"\t");
        strcat(s2,"\n");
        strcat(s1,s2);
        fputs(s1,fp2);

    }

    for(int i=0;i<vertices;i++) {
        printf("New key val pairs are:- %d, %d\n",mp[i].key,mp[i].value);
    }


    fclose(fp);
    if (line)
        free(line);
    
    return 0;
    
}