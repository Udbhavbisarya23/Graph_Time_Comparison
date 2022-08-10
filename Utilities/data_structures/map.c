#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "map.h"

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

void update(struct HashItem* mp,int key,int newVal, int vertices) {
    for(int i=0;i<vertices;i++) {
        if(mp[i].key == key) {
            mp[i].value = newVal;
            return;
        }
    }
}

struct HashItem* delete(struct HashItem* mp,int key, int vertices) {
    struct HashItem* newMap = (struct HashItem*)malloc(sizeof(struct HashItem)*(vertices-1));

    int i;
    for(i=0;i<vertices;i++) {
        if(mp[i].key == key) {
            break;
        }

        newMap[i].key = mp[i].key;
        newMap[i].value = mp[i].value;   
    }

    while(i<vertices) {
        newMap[i-1].key = mp[i].key;
        newMap[i-1].value = mp[i].value;
    }

    free(mp);
    return newMap;
}

void printMap(struct HashItem* mp,int vertices) {
    for(int i=0;i<vertices;i++) {
        printf("Key:- %d,Value:- %d\n",mp[i].key,mp[i].value);
    }
}