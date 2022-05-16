#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
    FILE* fp;
    fp = fopen("Graphs/data.txt","r+w");
    if(fp == NULL) {
        printf("Error in opening file\n");
        return 0;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read = -1;
    int first = 0,second=0,third=0,fourth=0;
    int vertices;

    while((read = getline(&line, &len, fp)) != -1) {
        if(first == 0) {
            first = 1;
            continue;
        }

        if(second == 0) {
            second = 1;
            continue;
        }

        if(third == 0) {
            third = 1;
            continue;
            
        }
        
        if(fourth == 0) {
            fourth = 1;
            continue;
        }
        
        char ch = fgetc(fp);
        if(ch==' ')
        {
            printf("Char is :- %c\n", ch); 
            fseek(fp, -1, SEEK_CUR);
            fputc('\t', fp);
            fseek(fp, 0, SEEK_CUR);	
        }
        if(ch == EOF)
            break;
    }
    fclose(fp);
    if (line)
        free(line);
}