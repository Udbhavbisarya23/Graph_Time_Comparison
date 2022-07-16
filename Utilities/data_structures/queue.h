#ifndef queue  
#define queue

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Queue {
    int front, rear;
    int size;
    unsigned capacity;
    struct AdjacencyListNode **array;
};

struct Queue* createQueue(unsigned capacity);
int isFull(struct Queue* qu);
int isEmpty(struct Queue* qu);
void enqueue(struct Queue* qu, struct AdjacencyListNode* item);
struct AdjacencyListNode* dequeue(struct Queue* qu);
struct AdjacencyListNode* front(struct Queue* qu);
struct AdjacencyListNode* rear(struct Queue* qu);


#endif