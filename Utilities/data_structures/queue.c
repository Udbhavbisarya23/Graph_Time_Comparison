#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "queue.h"
#include "../Graphs/directed_graph.h"

struct Queue* createQueue(unsigned capacity) {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->capacity = capacity;
    q->front = q->size = 0;

    q->rear = capacity-1;   
    q->array = (struct AdjacencyListNode**)malloc(capacity*sizeof(struct AdjacencyListNode*));
    return q;
}

int isFull(struct Queue* qu) {
    return (qu->size == qu->capacity);
}
  
int isEmpty(struct Queue* qu) {
    return (qu->size == 0);
}

void enqueue(struct Queue* qu, struct AdjacencyListNode* item) {
    if (isFull(qu))
        return;
    qu->rear = (qu->rear + 1)
                  % qu->capacity;
    qu->array[qu->rear] = item;
    qu->size = qu->size + 1;
}
  
// Function to remove an item from queue.
// It changes front and size
struct AdjacencyListNode* dequeue(struct Queue* qu) {
    if (isEmpty(qu))
        return NULL;
    struct AdjacencyListNode* item = qu->array[qu->front];
    qu->front = (qu->front + 1)
                   % qu->capacity;
    qu->size = qu->size - 1;
    return item;
}
  
// Function to get front of queue
struct AdjacencyListNode* front(struct Queue* qu) {
    if (isEmpty(qu))
        return NULL;
    return qu->array[qu->front];
}
  
// Function to get rear of queue
struct AdjacencyListNode* rear(struct Queue* qu) {
    if (isEmpty(qu))
        return NULL;
    return qu->array[qu->rear];
}
