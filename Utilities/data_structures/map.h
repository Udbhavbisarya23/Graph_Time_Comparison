#ifndef map  
#define map

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct HashItem {
    int key;
    int value;
};

int search(struct HashItem* mp,int key,int vertices);

void insert(struct HashItem* mp,int key,int value,int vertices);

#endif