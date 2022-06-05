#ifndef disjoint_sets  
#define disjoint_sets

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct DisjointSets {
    int nodes;
    int* parent;
    int* rank;
};

struct DisjointSets* newDisjointSets(int n);

void printDisjointSets(struct DisjointSets* ds);

int findParent(struct DisjointSets* ds, int node);

void unionByRank(struct DisjointSets* ds, int x, int y);

#endif