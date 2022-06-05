#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "disjoint_sets.h"

struct DisjointSets* newDisjointSets(int n) {
    struct DisjointSets* newDs = (struct DisjointSets*)malloc(sizeof(struct DisjointSets));
    newDs->nodes = n;

    newDs->parent = (int *)malloc(sizeof(int)*n);
    newDs->rank = (int *)malloc(sizeof(int)*n);

    for(int i=0;i<n;i++) {
        newDs->parent[i] = i;
        newDs->rank[i] = 0;
    }

    return newDs;
}

void printDisjointSets(struct DisjointSets* ds) {
    printf("Number of nodes:- %d\n",ds->nodes);
    for(int i=0;i<ds->nodes;i++) {
        printf("Node:-%d \t Parent-%d \t Rank-%d\n",i,ds->parent[i],ds->rank[i]);
    }
}

int findParent(struct DisjointSets* ds,int node) {
    
    if(node != ds->parent[node]) {
        ds->parent[node] = findParent(ds,ds->parent[node]);
    }
    return ds->parent[node];
}

void unionByRank(struct DisjointSets* ds,int x,int y) {
    x = findParent(ds,x);
    y = findParent(ds,y);

    if(ds->rank[x] > ds->rank[y]) {
        ds->parent[y] = x;
    } else {
        ds->parent[x] = y;
    }

    if(ds->rank[x] == ds->rank[y]) {
        ds->rank[y] += 1;
    }
}